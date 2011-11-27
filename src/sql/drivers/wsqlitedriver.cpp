/*
    WORM - a DAL/ORM code generation framework
    Copyright (C) 2011  Erik Winn <sidewalksoftware@gmail.com>

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

#include "wsqliteresult.h"
#include "wsqlitedriver.h"
#include "wsqlforeignkey.h"

#include <iostream>
#include <boost/algorithm/string.hpp>

#define ROWENDTAG "-*-"

namespace WSql
{

/*!\class WSqliteDriver - a driver for SQlite3 databases
 * This class provides a driver for SQlite3 databases.
 * 
 * DANGER: USE AT YOUR OWN RISK!!  NOT RECOMMENDED.
 * 
 * \todo This driver is unfinished. Basic functionality is there but it is not, ah, elegant.
 * Metadata is incomplete, all results are cached locally and there may be strange behavior.
 * Basically I have done enough to get sufficient metadata for the ORM generator (except
 *  indices still) and query() will perform the query and return a result
 * via result(). tableNames() works and tableMetaData() works as well (with the above
 * caveats). Transactions are not supported and I seriously doubt that this is thread safe.
 * It quite is crude at the moment - your milage may vary. It _might_ work for you but
 * at this point I don't recommend it for production (10/10/2011)
 * 
 * (at a minimum) :
 * \li finish initializing the indices and fully initialize a WSqlTable.
 * \li clean up some of the logic and make use of query() rather than reproducing code
 * for queries.
 * \li break up query() and handle errors properly
 * \li see the note by query() about creating a result set so that fields exist and have
 * names even if the value is empty (ie. NULL). This is a valid return in SQL and should
 * be there!
 * \li implement transactions
 * \li if we _must_ parse the create statements .. this needs redone correctly - it is a bit
 * buggy, awkward and very inefficient (a quick fix). Alternately, figure out some way
 * to get sqlite to provide the metadata correctly and reliably at _any_ time (meaning, even
 * if a table happens to be empty ..).
 * \li redo the columns mapping mess .. std::map is overkill, a quick fix..
 * \li general safety checks, eliminate code duplication, memory checks, testing .. etc.
 * 
 * \ingroup WSql
 * \sa WSqlDriver WSqlMysqlDriver
 */    
    
/*! \internal - dont use this.*/
bool isForeignKeyDefinition(std::string sql)
{
    boost::to_upper( sql );
    return( sql.find("FOREIGN KEY") != std::string::npos );
}
/*! \internal - dont use this.*/
bool isIndexDefinition(std::string sql)
{
    boost::to_upper( sql );
    return (boost::istarts_with(sql,"UNIQUE")
    ||boost::istarts_with(sql,"PRIMARY KEY")
    ||boost::istarts_with(sql,"INDEX")    
    ||boost::istarts_with(sql,"CREATE INDEX")
    ||boost::istarts_with(sql,"CREATE PRIMARY KEY")
    ||boost::istarts_with(sql,"CREATE UNIQUE")
    ||boost::istarts_with(sql,"CONSTRAINT INDEX")
    ||boost::istarts_with(sql,"CONSTRAINT UNIQUE")
    ||boost::istarts_with(sql,"CONSTRAINT PRIMARY KEY")
    );
}

/*! \internal return a foreign key for \a sqldef */
WSqlForeignKey createForeignKey(std::string sqldef)
{
    WSqlForeignKey fkToReturn;
    size_t sz = sqldef.size();
    size_t pos = 0;
    while(pos < sz) //sqlite is case insensitive ..
        sqldef[pos] = tolower(sqldef[pos++]);
//    std::cerr << "FK def: *" << sqldef << "*" << std::endl;
    //constraint "fk_comment_post" foreign key ("post_id") references "post" ("id")
    boost::trim_if(sqldef, boost::is_any_of("\n\t "));
    
    size_t start = sqldef.find_first_of("(");
    size_t end = sqldef.find_first_of(")", start);
    if(std::string::npos != start && std::string::npos != end )
    {
        std::string tmp = sqldef.substr(start+1, (end - start)-1);
        boost::trim_if( tmp, boost::is_any_of(" '\"[]"));
//        std::cerr << " column: trimmed *" << tmp << "*" <<  std::endl;
        start = sqldef.find_first_of("(",end);
        end = sqldef.find_first_of(")", start);
        fkToReturn.setColumnName(tmp);
        if(std::string::npos != start && std::string::npos != end )
        {
            std::string tmp2 = sqldef.substr(start+1, (end - start) -1);
            boost::trim_if(tmp2, boost::is_any_of(" '\"[]"));
//            std::cerr << "target column: trimmed *" << tmp2 << "*" <<  std::endl;
            fkToReturn.setReferencedColumnName(tmp2);
        }
    }else
        return fkToReturn;
    
    start = sqldef.find("references");
    if(std::string::npos != start)
    {
        start += 10;
        end = sqldef.find("(",start);
        std::string tmp3 = sqldef.substr(start, (end-start)-1);
        boost::trim_if(tmp3, boost::is_any_of(" '\"[]"));
//        std::cerr << "== references table: trimmed *" << tmp3 << "*" <<  std::endl;
        fkToReturn.setReferencedTableName(tmp3);
    }else
        return fkToReturn;
    start = sqldef.find("constraint");
    if(std::string::npos != start)
    {
        start = sqldef.find_first_of("'\"");
        end = sqldef.find_first_of("'\"",start+1);
        std::string tmp4 = sqldef.substr(start, (end-start)-1);
        boost::trim_if(tmp4, boost::is_any_of(" '\"[]"));
//        std::cerr << "constraint name: trimmed *" << tmp4 << "*" <<  std::endl;
        fkToReturn.setKeyName(tmp4);
    }    
    return fkToReturn;
}

//ugly brute force, don't look ..
/*! \internal - dont use this.*/
void splitIntoDefinitions(std::vector<std::string> &vecToFill, std::string sql)
{
    size_t pos = 0;
    size_t max = sql.size();
    while(pos < max )
    {
        if(sql[pos] == '(')
            while(++pos < max && sql[pos] != ')')
                continue;
        if(sql[pos] == ',')
        {
            std::string part =sql.substr(0,pos);
            boost::trim(part);
            vecToFill.push_back(part);
            sql.erase(0,pos+1);
            boost::trim(sql);
            max = sql.size();
            pos=0;
        }else
            pos++;
    }
    if(!sql.empty())
        vecToFill.push_back(sql);
}
/*!\brief Translate a data type string from Sqlite to WSql ..
 * Note - this is roughly how sqlite does this according to their docs so its merely reversed here
 */
WSqlDataType::Type WSqliteDriver::sqlite3TypeToWSqlType( std::string tname )const
{
    boost::to_lower( tname );

    //note - this includes "bigint"; that would be stored as an "integer" in sqlite
    if(tname.find( "int" ) != std::string::npos)
        return WSqlDataType::INT;
    if(tname.find( "text" ) != std::string::npos)
        return WSqlDataType::TEXT;
    if(tname.find( "char" ) != std::string::npos)
        return WSqlDataType::TEXT;
    if(tname.find( "clob" ) != std::string::npos)
        return WSqlDataType::TEXT;
    if(tname.find( "blob" ) != std::string::npos)
        return WSqlDataType::BLOB;
    if(tname.find( "floa" ) != std::string::npos)
        return WSqlDataType::DECIMAL;
    if(tname.find( "real" ) != std::string::npos)
        return WSqlDataType::DOUBLE;
    if(tname.find( "doub" ) != std::string::npos)
        return WSqlDataType::DOUBLE;
    return WSqlDataType::DECIMAL; //sqlite's "NUMERIC", the default ..
}

WSqlDataType::Type WSqliteDriver::sqlite3TypeToWSqlType( int dtype )const
{
    switch ( dtype ) {
        case SQLITE_INTEGER:
            return WSqlDataType::INT;
        case SQLITE_FLOAT:
            return WSqlDataType::DECIMAL;
        case SQLITE_TEXT:
            return WSqlDataType::TEXT;
        case SQLITE_BLOB:
            return WSqlDataType::BLOB;
        default:
            return WSqlDataType::NOTYPE;
    }
}

WSqliteDriver::WSqliteDriver( WSqlDatabase* db ) : WSqlDriver( db )
{
    _isUtf8 = true; //todo:support utf16
}

WSqliteDriver::~WSqliteDriver()
{
    close();
}

bool WSqliteDriver::init()
{
    if ( isValid() ) return true;

    if ( !_database ) {
        setError( std::string( "WSqliteDriver: No database object set!" ), WSqlError::DRIVER,
                  WSqlError::FATAL, false );
        return false;
    }
    //!\todo in this case we just use name as a file name - RESOLVE: change hostname to uri in wsqldatabase..?
    if (_database->databaseName().empty() ) {
        setError( std::string( "WSqliteDriver: No database specified!" ), WSqlError::DRIVER,
                  WSqlError::FATAL, false );
        return false;
    }
    setIsValid( true );
    return true;
}

bool WSqliteDriver::open()
{
    if ( isOpen() ) return true;
    if ( !init() ) return false;

    int err = sqlite3_open( _database->databaseName().c_str(), &_objSqlite );
    if ( SQLITE_OK != err ) 
    {
        std::string msg( "WSqliteDriver - Error opening database: " );
        msg.append( sqlite3_errmsg( _objSqlite ) );
        setError( WSqlError( msg, err, WSqlError::DRIVER, WSqlError::FATAL ) );
        setHasError( true );
        setIsValid( false );
        return false;
    }
    setHasError( false );
    setIsOpen( true );
    setIsValid( true );
    return true;
}

void WSqliteDriver::close()
{
    if ( isValid() )
        sqlite3_close( _objSqlite );
    _columns_map.clear();
    _foreign_keys.clear();
    setIsOpen( false );
    setIsValid(false);
}

//WSqlResult WSqliteDriver::exec(const WSqlQuery &queryObject){}

/*!
 * @brief Send the query \a sql to the database
 * 
 * \todo refactor this using metadata to construct a proper result set ..
 sqlite doesn't return things in orderly fashion - if a field is empty
 (i.e. NULL) it just doesn't include it in the results (ARG!), i _think_
 and other strange random behavior is observed ..- so, we need
 to construct a skeleton result set from metadata and then fill it leaving
 the appropriate gaps such that the field name will _always_ be there.
 We can't simply refer to tableMetaData as it may be a join or such - which
 would mean parsing the query, etc..ugly The problem is that we cannot
 rely on sqlite to return metadata if, for instance, the query returns no results..
 Consider using sqlite3_column_metadata.
 Also, some of this might be better moved to result()
 
 * @param std::string sql containing the query string
 * @retval bool
 **/
bool WSqliteDriver::query(std::string sql)
{
    if ( !isOpen() ) {
        setError( std::string( "WSqliteDriver - connection not open! Call open() first. " ),
                  WSqlError::DRIVER, WSqlError::FATAL, false );
        return false;
    }
    WSqliteResult *result = new WSqliteResult( this );
    if ( 0 == result ) {
        setError( std::string( "WSqliteDriver - memory allocation error! Failed to create WSqlResult! " ),
                  WSqlError::DRIVER, WSqlError::FATAL, false );
        return false;
    }
    result->setIsCached( true );
    sqlite3_stmt *statement;
    int err = sqlite3_prepare_v2( _objSqlite, sql.c_str(), -1, &statement, 0 );
    if ( SQLITE_OK == err ) 
    {
        int cols = sqlite3_column_count( statement );
        while ( sqlite3_step( statement ) == SQLITE_ROW ) 
        {
            WSqlRecord record;
            for ( int col = 0; col < cols; col++ ) 
            {
                WSqlField fld;
                std::string text;
                std::string colname;
                std::string fldname;
                const unsigned char * safety = sqlite3_column_text( statement, col );
                if ( safety ) text = ( char * ) safety;
                const char * safety2 = sqlite3_column_origin_name( statement, col );
                if ( safety ) colname = ( char * ) safety2;
                const char * safety3 = sqlite3_column_name( statement, col );
                if ( safety ) fldname = ( char * ) safety3;

                fld.setData( text );
                if ( !fldname.empty() )
                    fld.setName( fldname );
                else
                    fld.setName( colname );
                fld.setColumnName( colname );

                record.append( fld );
            }
            result->addRecord( record );
        }
        delete _result;
        _result = result;
        sqlite3_finalize( statement );
        return true;
    }else {
        std::string msg( "WSqliteDriver - query failure, SQL: " + sql + "\n Server said: " );
        msg.append( sqlite3_errmsg( _objSqlite ) );
        setError( WSqlError( msg, err, WSqlError::QUERY, WSqlError::DANGER ) );
        setHasError( true );
        setIsValid( false );
        return false;
    }
}

WSqlResult* WSqliteDriver::result(bool iscached)
{
    if ( 0 == _result )
        _result = new WSqliteResult( this );
    return _result;
}

//!\todo support table type flag .. views, information schema
std::vector<std::string> WSqliteDriver::tableNames()
{
    std::vector<std::string> vecToReturn;
    if ( !isOpen() )
        return vecToReturn;
    sqlite3_stmt *statement;
    const char *sql = "SELECT name from sqlite_master WHERE type='table'";
    if ( sqlite3_prepare_v2( _objSqlite, sql, -1, &statement, 0 ) == SQLITE_OK ) 
    {
        while ( sqlite3_step( statement ) == SQLITE_ROW ) 
        {
            const unsigned char * safety = sqlite3_column_text( statement, 0 ); 
            std::string tname;
            if(NULL != safety)
                tname = ( char* )safety; 
            if (tname.empty() || tname.compare( "sqlite_sequence" ) == 0 )
                continue;
            vecToReturn.push_back(tname);
        }
        sqlite3_finalize( statement );
    }
    return vecToReturn;
}

WSqlTable WSqliteDriver::tableMetaData( const std::string &tableName )
{
    WSqlTable tblToReturn;
    if ( !_tables.empty() ) 
    {
        tblToReturn = findTable( tableName );
        if ( tblToReturn.isValid() )
            return tblToReturn;
    }
    tblToReturn.setName( tableName );
    if ( !isOpen() )//huh? .. nevermind..todo: something nicer
        return tblToReturn;   
    //parse and store info from the create statement ..
    std::string sqlToParse = fetchTableCreateStatement( tableName );
    parseSchema(sqlToParse);
    
    sqlite3_stmt *statement;
    std::string col_descript;
    std::string sql( "pragma table_info(" );
    sql.append( tableName );
    sql.append( ");" );
    if ( sqlite3_prepare_v2( _objSqlite, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK ) 
    {
        int numcols = sqlite3_column_count( statement );
        while ( sqlite3_step( statement ) == SQLITE_ROW ) 
        {
            WSqlColumn clm;
            for ( int i = 0; i < numcols; i++ ) 
            {
                const unsigned char * safety = sqlite3_column_text( statement, i );
                if ( !safety )
                    continue;
                col_descript = ( char * ) safety;
                switch ( i ) {
                    case 1:
                        clm.setColumnName( col_descript );
                        break;
                    case 2:
                        clm.setDataType( sqlite3TypeToWSqlType( col_descript ) );
                        break;
                    case 3:
                        clm.setCanBeNull( col_descript.compare( "0" ) ? false : true ); //careful..
                        break;
                    case 4:
                        clm.setDefaultValue( col_descript );
                        break;
                    case 5:
                        clm.setIsPrimaryKey( col_descript.compare( "0" ) ? true : false );
                        break;
                    default:
                        continue;
                }
            }
            clm.setIsAutoIncremented(columnIsAutoIncrement(clm.columnName()));
            tblToReturn.append( clm );
        }
        sqlite3_finalize( statement );
    }    
    tblToReturn.setIsValid(true);
    std::vector<WSqlForeignKey>::iterator it = _foreign_keys.begin();
    for(; it != _foreign_keys.end(); ++it)
    {
       it->setTableName(tableName);
        tblToReturn.addForeignKey(*it);
    }
    _tables.push_back( tblToReturn );
    _columns_map.clear();
    _foreign_keys.clear();
    return tblToReturn;
}

/*! \internal - dont use this.*/
bool WSqliteDriver::columnIsAutoIncrement( const std::string& columnname )const
{
    //!\todo - add arg for table name and look up in _tables if possible ..
    
    std::map<std::string, std::string>::const_iterator it = _columns_map.find(columnname);
    if(_columns_map.end() == it)
        return false;
    std::string col_definition = it->second;
    boost::to_lower(col_definition);
    if(col_definition.find("autoincrement") != std::string::npos
        || col_definition.find("auto_increment") != std::string::npos
        )
        return true;
    return false;
}

/*! \internal - dont use this.*/
std::string WSqliteDriver::fetchTableCreateStatement( const std::string& tablename )const
{
    sqlite3_stmt *statement;
    std::string sqlToReturn;
    std::string sql( "SELECT sql from sqlite_master WHERE tbl_name = '" );
    sql.append( tablename ); sql.append( "';" );

    if ( sqlite3_prepare_v2( _objSqlite, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK ) 
    {
        while ( sqlite3_step( statement ) == SQLITE_ROW ) 
        {
            const unsigned char * safety = sqlite3_column_text( statement, 0 );
            if ( safety && SQLITE_ERROR != *safety ) {
                sqlToReturn.append(( char* ) safety );
                sqlToReturn.append( ROWENDTAG );
            }
            else
            {//this is quite weird - empty rows .. 
                std::cerr << "fetchTableCreateStatement - sqlite weirdness: " << tablename << std::endl;
                //std::cerr <<  sqlite3_errmsg( _objSqlite ) << std::endl;
            }
            //??usleep();
        }
        sqlite3_finalize( statement );
    }
    return sqlToReturn;
}

/*! \internal - dont use this.*/
std::string WSqliteDriver::extractStatement( const std::string& sqlToParse, const char start, const char end ) const
{
    std::string sqlToReturn;
    size_t pos_start = 0; size_t pos_end = 0;
    pos_start = sqlToParse.find_first_of( start );
    pos_end = sqlToParse.find_last_of( end );
    if ( std::string::npos == pos_start || std::string::npos == pos_end )
        return sqlToReturn;
    sqlToReturn = sqlToParse.substr( pos_start+1, pos_end - (pos_start+1));
//    std::cerr << "extract Statement sqlToReturn: " << sqlToReturn << std::endl;
    return sqlToReturn;
}

//eh, not graceful but mostly works
/*! \internal - dont use this. This parses the create statement for a table ..*/
void WSqliteDriver::parseSchema( std::string& sql)
{
    typedef std::vector<std::string>::iterator Iter;
    std::vector<std::string> statements;  //complete statements in parens
    std::vector<std::string> definitions;  //comma separated column defs
    
    std::vector<std::string> columns;      //stored split off columns, constraints, indices
    std::vector<std::string> indices;
        
    boost::trim_if(sql, boost::is_any_of("-*"));
    boost::split( statements, sql,  boost::is_any_of( "-*" ), boost::token_compress_on );    
    Iter stmt_it = statements.begin();
    for ( ;stmt_it != statements.end();++stmt_it ) 
    {
        std::string cur_statement =  *stmt_it;
        boost::trim(cur_statement);
        //get statement between parens - but only for the first "CREATE"; 
        //thus avoid extracting primary key definitions like (x_id,y_id)
        if(boost::istarts_with(cur_statement,"CREATE TABLE"))
            cur_statement = extractStatement(cur_statement);
        //cut up by line -comma separated column definitions
        splitIntoDefinitions(definitions, cur_statement);        
        Iter defs_it = definitions.begin();
        for ( ; defs_it != definitions.end();++defs_it ) 
        {
            std::string cur_definition = *defs_it;
            boost::trim(cur_definition);
            if( isForeignKeyDefinition(cur_definition))
            {
/*                std::cout << "cur_definition: " << cur_definition << std::endl 
                    << "************** FOREIGN KEY ***************" << std::endl;*/
                WSqlForeignKey fk = createForeignKey(cur_definition);
                _foreign_keys.push_back(fk);
            }
            else if( isIndexDefinition(cur_definition) )
            {
/*                std::cout << "cur_definition: " << cur_definition << std::endl
                << "************** INDEX ***************" << std::endl;*/
                indices.push_back(cur_definition);
            }
            else
                columns.push_back(cur_definition);
        }
        definitions.clear();
    }
    //map to local map 
    mapColumns(columns);
}

/*! \internal - dont use this.*/
void WSqliteDriver::mapColumns(std::vector<std::string> &vecColumnDefinitions)
{
    std::vector<std::string>::const_iterator it = vecColumnDefinitions.begin();
    for(;it != vecColumnDefinitions.end();++it)
    {
        std::string cur_definition = *it;
        size_t pos = cur_definition.find_first_of("\t ");
        if(pos != std::string::npos)
        {
            std::string cur_colname = cur_definition.substr(0,pos);
            boost::trim_if(cur_colname, boost::is_any_of("\n\t []`'\""));
            _columns_map[cur_colname] = cur_definition.erase(0,pos+1); 
        }        
    }
}
}//namespace WSql
