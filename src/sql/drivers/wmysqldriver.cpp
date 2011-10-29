/*
    WORM - a DAL/ORM code generation framework
    Copyright (C) 2011  Erik Winn <erikwinnmail@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wmysqlresult.h"
#include "wmysqldriver.h"

namespace WSql {

/*!    \class WMysqlDriver
*    \brief A driver for MySQL databases.
* 
*  This class provides a driver for MySQL databases. It supports:
* \li simple queries
* \li (only) cached result sets
* \li metadata retrieval
* 
* TODO: uncached results, transactions, prepared statements .. and
* fix string escaping...
* 
*/
    
/*! \brief Constructs a WMysqlDriver with the database \a db

This constructor initializes the mysql libraries and prepares for establishing
a connection.
*/        
WMysqlDriver::WMysqlDriver( WSqlDatabase* db ): WSqlDriver( db )
{
    _mysql = 0;
    setIsOpen(false);
    //this _should_ make us thread safe ..
    int err =mysql_library_init(0, NULL, NULL); 
    if (err) 
    {
        std::string msg( "WMysqlDriver - WARNING! Error inilizing MySQL library!: " );
        setError( WSqlError( msg, 99, WSqlError::DRIVER, WSqlError::FATAL ) );
        setIsValid(false);
    }else
        init();    
}


WMysqlDriver::~WMysqlDriver()
{
    close();
    mysql_library_end();
}
/*! \brief Initialize the database connection
 * 
 This method sets up the basic environment prior to opening a connection.
 \return bool true on success
 */
bool WMysqlDriver::init()
{
    if ( isValid() ) 
        return true;

    //crude garbage collection: passing null tells library to free this after mysql_close()
    _mysql = mysql_init(NULL);
    if(NULL == _mysql)
    {
        setError( std::string( "WMysqlDriver: Memory allocation error!" ), WSqlError::DRIVER,
                  WSqlError::FATAL, false );
        return false;
    }
    setIsValid( true );
    return true;
}


/*! \brief Closes the connection and deallocates MYSQL objects
 * 
 This method closes the connection and frees the MYSQL (_mysql) memory.
 WARNING: Uncached result sets in a WSqlResult will be INVALID after calling this!
 It is recommended that one leave this to the destructor - it is called automatically 
 when the WMysqlDriver object is destroyed.
 */
void WMysqlDriver::close()
{
    if( !isValid() || !isOpen() )
        return;
    mysql_close(_mysql);
    if(! _result->isCached() )
        _result->setIsValid(false);
    setIsOpen(false);
}

/*! \brief Opens the connection to the database
 * 
 This method opens the connection to the database - it must be called before
 any queries are sent. Note that it may be called more than once - calling this
 if the connection is still open from a previous call will close and reopen the
 connection, equivilant to a reset() function.
  This returns true if the connection was successful otherwise returns false,
  sets isValid() and hasError() to return false and sets an error if possible.
  
 \note If this is called after a previous call and the results were not cached 
 any results in the previous WSqlResult will be INVALID.
 \returns bool - true on success
 */
bool WMysqlDriver::open()
{
    //this is equivilant to resetting if called more than once ..
    if ( isOpen() )
        close();
    if ( !init() ) 
        return false;
//TODO: set connection options here, esp. utf* and friends
// - CLIENT_MULTI_STATEMENTS if we intend to CALL any procedures
// but for now kiss, worm doesn't need much ..
    mysql_options(_mysql,MYSQL_READ_DEFAULT_GROUP,"worm");
    MYSQL *retval = mysql_real_connect( _mysql, 
                            _database->hostName().empty()? NULL : _database->hostName().c_str(),
                            _database->userName().empty()? NULL : _database->userName().c_str(),
                            _database->password().empty()? NULL : _database->password().c_str(),
                            _database->databaseName().empty()? NULL : _database->databaseName().c_str(),
                            _database->port()<1? 0 : _database->port(),
                            NULL, //socket
                            0 // client flags ..
                            );
    if(NULL == retval)
    {
        std::string msg( "WMysqlDriver - Error opening database: " );
        msg.append( mysql_error(_mysql) );        
        setError( WSqlError( msg, mysql_errno(_mysql), WSqlError::DRIVER, WSqlError::FATAL ) );
        return false;        
    }
    setIsValid(true); //just to be sure ..
    setIsOpen(true);
    return true;
}
/*!\brief Sends the SQL in \a sqlstring to the server for execution
 * 
    This method sends the SQL given in \a sqlstring to the server to be
    executed. If the query was successful it returns true, if not it sets an
    error and returns false. If the query should return results (eg. a SELECT)
    they will be available from result().
    
    \returns bool on success
*/
bool WMysqlDriver::execute(std::string sqlstring )
{
    if(!isOpen())
        return false;
    
// TODO: RESOLVE - this also escapes LIKE quotes .. need a way to handle this.
// for now it is disabled
// FIXME       std::string sql = local_escape_string(sqlstring);
        
    std::string sql = sqlstring;
    if(sql.empty() )
        setError("WARNING: WMysqlDriver string escape failed.");
    
    if(mysql_real_query(_mysql, sql.c_str(),sql.length())) //should be zero
    {
        std::string msg( "WMysqlDriver - Error executing query: '" );
        msg.append(sql + "', \n Server error: " + mysql_error(_mysql) );
        setError( WSqlError( msg, mysql_errno(_mysql), WSqlError::QUERY, WSqlError::WARNING ) );
        return false;
    }
    return true;
}

/*!\brief Return the result from the most recent query
 * 
 This method returns a pointer to a WSqlResult object containing
 the most recent result set from a query - it may be empty if no results
 were returned. You must not delete this pointer yourself - it is owned
 by the driver.
 
 The parameter \a iscached may be set to true to indicate a non-cached result
 set that will be fetched row by row - the default is true and results are cached.
 
 \note Only use this \em after an execute() query! Do not use twice in a row as it will delete
 the previous result and return a newly created object. Example:

\code
 WSqlDatabase db;
if (!db.open()) 
    dosomeerror();
if (!db.execute(std::string("select foo from bar")))
    dosomeerror();
WSqlResult *result = db.result();
//WSqlResult *result2 = db.result(); <- wrong 
//...iterate over results ..._then repeat:
if (!db.execute(std::string("select baz from bar")))
    dosomeerror();
WSqlResult *result = db.result();
..etc.
\endcode

\param bool iscached - if true (the default) fetches entire result set at once.
 \return WSqlResult* contains the result set
 */
WSqlResult* WMysqlDriver::result( bool iscached )
{
    delete _result;
    _result = new WMysqlResult(this);
    if(!iscached)//TODO
        std::cerr << "Warning: WMysqlDriver noncached results unimplemented!" << std::endl;
    //local result to avoid annoying static_cast ..
    MYSQL_RES *res = mysql_store_result(_mysql);
    if(NULL == res){
        std::cout << "res is null: " << mysql_error(_mysql) << std::endl;
        int errno = mysql_errno(_mysql);
        if(errno){
            std::string msg = "WMysqlDriver: Error fetching result set. \n Server error: ";
            msg.append(mysql_error(_mysql));
            setError( WSqlError( msg, errno, WSqlError::QUERY, WSqlError::WARNING ) );
            _result->setIsValid(false);
        }
    }else{
        MYSQL_ROW row; 
        ulong numfields = mysql_num_fields(res);
        while((row = mysql_fetch_row(res)))
        {
            ulong *lengths = mysql_fetch_lengths(res);
            WSqlRecord record;
            for(ulong fld_pos=0; fld_pos < numfields; ++ fld_pos)
            {
                MYSQL_FIELD *m_field = mysql_fetch_field_direct(res, fld_pos);
                WSqlField fld;
                fld.setName(m_field->name);
                fld.setColumnName(m_field->org_name);
                if(0 == lengths[fld_pos])
                    if(NULL == lengths[fld_pos])
                        fld.setData(std::string("NULL"));
                    else
                        fld.setData(std::string(""));
                else
                    fld.setData(row[fld_pos]);
                record.append(fld);
            }
            _result->addRecord(record);
        }
       static_cast<WMysqlResult *>(_result)->setResult(res);
       _result->setIsValid(true);
    }
    return _result;
}

/*!\brief Returns a list of the table names in the current database
 * 
This method will return a vector<string> containing the names
of the tables in the current database - the database name must be
set first.

\note WARNING: This method will invalidate previous WSqlResults returned - nesting
calls to this method inside of a loop iterating over WSqlResults will not work. Obtain
the list \em first and \em aftwards execute a query and fetch the result set using
result().
\retval vector<string> a list of table names
 */
std::vector< std::string > WMysqlDriver::tableNames()
{
    std::vector<std::string>vecToReturn;
    if(!isOpen())    {
        setError("tableNames: Connection not open - use open()");
        return vecToReturn;
    }
    if(_database->databaseName().empty())    {
        setError("tableNames: You must set a database name first!");
        return vecToReturn;
    }
    std::string query = "show tables";
    execute(query);
    result();
    int numtables = _result->count();
    for(int i=0;i<numtables;++i)
        vecToReturn.push_back(_result->fetch(i).field(0).data<std::string>());
    return vecToReturn;
}
/*!\brief Fetch metadata for table \a tableName
 * 
    This method returns a WSqlTable object populated with WSqlColumns that
    contain metadata for the columns in the given table \a tableName.
    \note It is recommended that this not be used directly but rather from a
    WSqlDatabase object. Note also the warning below - unless you really only 
    need metadata for a single table it is better to initialize all the tables at once
    with WSqlDatabase::initMetaData() and avoid the possibility of invalidating
    a result set.
    
    Example:

\code
    meta_table = db.tableMetaData(*it);
    numflds = meta_table.count();
    for (int i=0; i < numflds; ++i) 
    {
        WSql::WSqlColumn column = meta_table.column(i);
        
        std::cout << "Column " << i << " = " << column.columnName() << std::endl;
        std::cout << "  * Data type: " << WSql::WSqlDataType::toString(column.dataType()) << std::endl;
        std::cout << "  * Max Length:  " << column.maxLength() << std::endl;
        std::cout << "  * Unsigned:  " << (column.isUnsigned() ? "true" : "false") << std::endl;
        std::cout << "  * Can be null:  " << (column.canBeNull() ? "true" : "false") << std::endl;
        std::cout << "  * Primary key:  " << (column.isPrimaryKey() ? "true" : "false") << std::endl;
        std::cout << "  * Autoincrement:  "  << (column.isAutoIncremented()?"true" : "false") << std::endl;
        std::cout << "  * default value:  "  << column.defaultValue<std::string>() << std::endl;
    }

    \endcode
    
    \warning If the table metadata has not been initialized yet this method will invalidate 
    any previous WSqlResult pointer returned - in this case nesting calls to this method inside 
    of a loop iterating over WSqlResults WILL NOT WORK. Obtain the WSqlTable \em first and 
    \em then execute() a query and fetch the result set using result() or use WSqlDatabase::initMetaData()
    to initialize the metadata for all tables at once.
    
    \param string the name of the table to use
    \return WSqlTable an object containing metadata 
    \sa WSqlTable WSqlColumn
 */
WSqlTable WMysqlDriver::tableMetaData( const std::string& tableName )
{
    WSqlTable tblToReturn;
    if ( !_tables.empty() ) 
    {
        tblToReturn = findTable( tableName );
        if ( tblToReturn.isValid() )
            return tblToReturn;
    }
    tblToReturn.setName(tableName);
    
    std::vector<std::string> column_names;
    std::vector<std::string>::const_iterator column_names_it;
    std::string sql("show columns in ");sql.append(tableName);
    execute(sql);
    result();
    WSqlColumn clm;
    WSqlRecord record = _result->fetchFirst();
    while(!record.empty())
    {
/*        clm.setColumnName(record.field("Field").data<std::string>());
        initColumnType(clm, record.field("Type").data<std::string>());
        bool nullable = ! (record.field("Null").data<std::string>().compare("NO") == 0);
        bool primarykey = (record.field("Key").data<std::string>().compare("PRI") == 0);
        clm.setDefaultValue( record.field("Default").data<std::string>());
        bool isauto = (record.field("Extra").data<std::string>().find("auto_increment") != std::string::npos);*/
        clm.setColumnName(record.field(0).data<std::string>());
        column_names.push_back(clm.columnName());
        initColumnType(clm, record.field(1).data<std::string>());
        bool nullable = (record.field(2).data<std::string>().compare("NO") == 0);
        bool primarykey = (record.field(3).data<std::string>().compare("PRI") == 0);
        clm.setDefaultValue( record.field(4).data<std::string>());
        bool isauto = (record.field(5).data<std::string>().find("auto_increment") != std::string::npos);
        clm.setCanBeNull(nullable);
        clm.setIsPrimaryKey(primarykey);
        clm.setIsAutoIncremented(isauto);        
        tblToReturn.append(clm);
        record = _result->fetchNext();
    }
    //initialize foreign keys ..
    WSqlForeignKey fk;
    for(column_names_it = column_names.begin();column_names_it != column_names.end(); column_names_it++)
    {
        
        sql = "select constraint_name, referenced_table_schema,  referenced_table_name,"
        " referenced_column_name from information_schema.key_column_usage "
        " where table_name like '" + tableName + "' and column_name like '" + *column_names_it + "'";
        
        execute(sql);
        result();
        WSqlRecord record = _result->fetchFirst();
        while(!record.empty())
        {
            if( record["referenced_column_name"].compare("NULL") == 0)
            {
                record = _result->fetchNext();
                continue;
            }
            fk.setColumnName(*column_names_it);
            fk.setTableName(tblToReturn.name());
            fk.setKeyName(record["constraint_name"]);
            fk.setReferencedColumnName(record["referenced_column_name"]);
            fk.setReferencedSchemaName(record["referenced_schema_name"]);
            fk.setReferencedTableName(record["referenced_table_name"]);
            tblToReturn.addForeignKey(fk);
            record = _result->fetchNext();
        }        
    }
    tblToReturn.setIsValid(true);
    _tables.push_back(tblToReturn);
    return tblToReturn;
}

bool WMysqlDriver::isOpen()
{
    return isValid() && WSqlDriver::isOpen();
}

/*! \internal Initialize a column with data type and such ..*/
void WMysqlDriver::initColumnType( WSqlColumn &clm, std::string description )
{
    std::string size;
    std::string tname;
    size_t pos = description.find("unsigned");
    if ( pos != std::string::npos)
    {
        clm.setIsUnsigned(true);
        description.erase(pos, 8);
    }
    pos = description.find_first_of("(");
    if(pos != std::string::npos)
    {
        size_t endpos = description.find_last_of(")");
        size = description.substr(pos+1, endpos - (pos+1));
        clm.setMaxLength(atoi(size.c_str()));
        description.erase(pos, endpos - pos );
    }else{
        pos = description.find_first_of(" ");
    }
    if(pos != std::string::npos)
        tname = description.substr(0,pos);
    else
        tname = description;
    clm.setDataType(WSqlDataType::toType(tname));    
}

//!use mysql to escape some things .. TODO: something more robust.
std::string WMysqlDriver::local_escape_string(std::string& escapeme)
{
    if(escapeme.empty())
        return escapeme;
    ulong inlength = escapeme.length();
    char *buffer = new char[(inlength * 2) + 1];
    if(NULL == buffer) {
        setError("Warning: escape string memory allocation error! String not escaped.");
        return std::string();
    }
    ulong retlen = mysql_real_escape_string(_mysql, buffer, escapeme.c_str(), inlength);
    if(retlen < inlength){
        setError("Warning: escape string size error! String not escaped.");
        delete[] buffer;
        return escapeme;
    }
    std::string ret = buffer;
    delete[] buffer;
    return ret;
}

}//namespace WSql
