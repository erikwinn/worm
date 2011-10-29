/*
 *    WORM - a DAL/ORM code generation framework
 *    Copyright (C) 2011  Erik Winn <erikwinnmail@yahoo.com>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "wsqldatabase.h"

#include "wsqldriver.h"
#include "drivers/wsqlitedriver.h"
#include "drivers/wmysqldriver.h"

namespace WSql {

/*!
    \class WSqlDatabase
    \brief The WSqlDatabase class represents a single database.

    The WSqlDatabase class provides an interface for accessing a specific
    database through a connection. An instance of WSqlDatabase
    represents the information relevant to connecting to the database,
	including the name of the database, the host, the username, etc.
    
    WSqlDatabase should be instantiated with a driver type (or another
    valid database object) - when constructed it will try to obtain the requested
    driver from WSqlDriverFactory(todo - currently just creates one ..). If this 
    fails isValid() will return false.
 
    Drivers are derived from WSqlDriver, see WSql::DriverType for supported 
    database drivers.
    
    Example:
    \code
    
WSql::WSqlDatabase db( WSql::WMYSQL );
db.setDatabaseName( std::string( "sakila" ) );
db.setUserName( "root" );

if ( !db.open() ) {
    std::cerr << "Failed to open: " << db.error().text() << std::endl;
    return 1;
}
std::string sql = "select * from " + sometable + " limit 2;";
if(!db.execute(sql))
    std::cout << "Query Failed: " << db.error().text() << std::endl;
}else{
    WSql::WSqlResult *result = db.result();
    std::cout << "Number of rows: " << result->size() << std::endl;
    WSql::WSqlRecord record = result->fetchFirst();
    while(!record.empty())
    {
        int numcols = record.count(); // or record.size();
        std::cout << "Number of columns: " << record.size() << std::endl;
        WSql::WSqlField fld;
        for(int i = 0;i < record.size();i++)
        {
            //no danger - if there is nothing there we still get an empty field object 
            fld = record.field(i);
            std::cout << "Field " << fld.name()
            << ", Origin Column "<< fld.columnName() 
            << "Value: " << fld.data<std::string>() << std::endl;
        }
        record = result->fetchNext();
    }
}

\endcode
    
    \sa WSqlResult WSqlRecord WSqlField WSql::DriverType WSqlDriver
    \addtogroup WSql
*/

/*!  \brief Creates a WSqlDatabase object with a driver.
 * 
 *This constructs a database object with a driver of \a type - the
  object will attempt to create a driver - if successful  isValid()
  will return true.
  \param WSql::DriverType type - the type of driver to use
 */
WSqlDatabase::WSqlDatabase(const WSql::DriverType &type)
{
    _driver=0;
	_driverType=type;
    _isValid=false;
    init();
}

/*!
    \brief Creates an empty, invalid WSqlDatabase object. 
*/
WSqlDatabase::WSqlDatabase()
{
	_driver=0;
	_isValid=false;
}

/*!  \brief Creates a copy of \a other.
    
    This constructs a new database object initialized with the values of  \a other
     *    WARNING: This also creates a new driver! If \a other
     *    is destroyed result() is invalid until the next execute() is called!    

    \param WSqlDatabase other - database to copy
*/
WSqlDatabase::WSqlDatabase(const WSqlDatabase &other)
{
    _databaseName = other._databaseName;
    _userName = other._userName;
    _password = other._password;
    _hostname = other._hostname;
    _port = other._port;
    _connectionOptions = other._connectionOptions;
    _driverType = other._driverType;
    init();
}

/*!\brief Destroys the object and frees any allocated resources.
 * 
Note that the driver is also destroyed at this time - any WSqlResults
obtained from result() will be invalid after this.
    \sa close()
*/

WSqlDatabase::~WSqlDatabase()
{
	if(isValid()){
		close();
	}
	delete _driver;
}

/*!\brief Copies the values of \a other to this object.
 * 
    WARNING: This also creates a new driver! If \a other
    is destroyed result() is invalid until the next execute()!

    \param WSqlDatabase other - database to copy
*/
WSqlDatabase &WSqlDatabase::operator=(const WSqlDatabase &other)
{
    if(_driver)
        delete _driver;
    _driver=0;
    
    _databaseName = other._databaseName;
    _userName = other._userName;
    _password = other._password;
    _hostname = other._hostname;
    _port = other._port;
    _connectionOptions = other._connectionOptions;
    _driverType = other._driverType;
    init();
    return *this;
}

/*!
    \internal
    Initializes the driver and state
*/
void WSqlDatabase::init()
{
    if(_driver)
        delete _driver;
    
    _isValid = initDriver();
}

/*!
 *    \internal
 *    Attempts to create a driver - returns true on success
 */
bool WSqlDatabase::initDriver()
{
    bool blnToReturn = false;
    std::string errmsg;
    
    switch(_driverType)
    {
        case WSql::NONE:
            errmsg = "WSqlDatabase: initDriver failed - no driver type set!";
            break;
        case WSql::WSQLITE :
            _driver = new WSqliteDriver(this);
            if(0 == _driver)
                errmsg = "WSqlDatabase: initDriver failed - memory allocation error!";
            else
                blnToReturn = true;
            break;
        case WSql::WMYSQL :
            _driver = new WMysqlDriver(this);
            if(0 == _driver)
                errmsg = "WSqlDatabase: initDriver failed - memory allocation error!";
            else
                blnToReturn = true;
            break;
        case WSql::WMONGO :
        case WSql::WBERKLEYDB :
        case WSql::WPSQL :
        default:
            errmsg = "WSqlDatabase: initDriver failed - driver unsupported!";
    }
    if(!errmsg.empty())
        _errorStack.push_back(WSqlError(errmsg,1, WSqlError::SYSTEM, WSqlError::WARNING));    
    return blnToReturn;
}

/*! \brief Open a connection
 * 
    Opens the database connection using the current connection
    values. Returns true on success; otherwise returns false. Error
    information can be retrieved using error().

    \sa error() setDatabaseName() setUserName() setPassword()
    \sa setHostName() setPort() setConnectOptions()
*/

bool WSqlDatabase::open()
{
    return _driver->open();
}

/*!\brief Open a connection using \a username and \a password

    \overload

    Opens the database connection using the given \a user name and \a
    password. Returns true on success; otherwise returns false. Error
    information can be retrieved using the error() function.
    \sa error()
*/

bool WSqlDatabase::open(const std::string& username, const std::string& password)
{
    setUserName(username);
	setPassword(password);
    return _driver->open();
}

/*!
    Closes the database connection.
*/

void WSqlDatabase::close()
{
    _driver->close();
}

/*!
    Returns true if the database connection is currently open
*/

bool WSqlDatabase::isOpen() const
{
    return _driver->isOpen();
}

/*!
    Returns true if there is an error available.    
    \sa error()
*/

bool WSqlDatabase::hasError() const
{
    //TODO: resolve this - there is also the local errorStack ..
    return _driver->hasError();
}


/*! \brief Set the the database name
 * 
    Sets the connection's database name to \a name. To have effect,
    the database name  must be set \em before opening the connection with open()
    Alternately, you can close() the connection, set the database name, and call open() 
    again
    There is no default value.
    \param std::string name - the name of the database
    \sa databaseName()
*/
void WSqlDatabase::setDatabaseName(const std::string& name)
{
    _databaseName = name;
}

/*! \brief Set the the user name
 * 
 *    Sets the connection's user name to \a name. To have effect, the
    user name  must be set \em before opening the connection with open()
    Alternately, you can close() the connection, set the user name, and call open() 
    again.

    There is no default value.
    \param std::string name - the name of the user
    \sa userName()  open() close()
*/

void WSqlDatabase::setUserName(const std::string& name)
{
    _userName = name;
}

/*! \brief Set the the password
 * 
 *    Sets the connection's password to \a password. To have effect, the
    password must be set \e{before} the connection is \l{open()}
    {opened}.  Alternatively, you can close() the connection, set the
    password, and call open() again.

    There is no default value.
    \param std::string password - the password to use
\sa password() open() close()
*/

void WSqlDatabase::setPassword(const std::string& password)
{
    _password = password;
}

/*! \brief Sets the connection's host name
 * 
    This method sets the connection's host name to \a hostname. To have effect, the
    host name must be set \em before opening the connection with open()
    Alternately, you can close() the connection, set the host name, and call open() again.

    There is no default value.
    \param std::string hostname - the hostname to use
    \sa hostName() open() close()
*/

void WSqlDatabase::setHostName(const std::string& hostname)
{
    _hostname = hostname;
}

/*! \brief Sets the port number for the connection
 * 
    This method sets the connection's port number to \a port. To have effect, the
    port number  must be set \em before opening the connection with open()
    Alternately, you can close() the connection, set the port number, and call open() again

    The default value is -1.

    \sa port() open() close()
    \param int port number to use
*/

void WSqlDatabase::setPort(int port)
{
    _port = port;
}

/*!
    Returns the connection's database name, which may be empty.
    \sa setDatabaseName()
*/
std::string WSqlDatabase::databaseName() const
{
    return _databaseName;
}

/*!
    Returns the connection's user name; it may be empty.

    \sa setUserName()
*/
std::string WSqlDatabase::userName() const
{
    return _userName;
}

/*!
    Returns the connection's password. If the password was not set an empty 
    string is returned.
*/
std::string WSqlDatabase::password() const
{
    return _password;
}

/*!
    Returns the connection's host name; it may be empty.

    \sa setHostName()
*/
std::string WSqlDatabase::hostName() const
{
    return _hostname;
}

/*!
   \fn int WSqlDatabase::port()const
   \brief Returns the connection's port number.
   
   The value is -1 if the port number has not been set.
    \sa setPort()
*/
/*!\brief Return a pointer to the database driver - expert only.

This returns a pointer to the database driver used to access the database
    connection. \em Caution! This is not meant to be used directly - use open().
    close(), execute() and result() for interaction with the driver instead.  
    
    !This may be removed in future.

    \sa open() close() execute() result()
*/

WSqlDriver* WSqlDatabase::driver() const
{
    return _driver;
}

/*!
 *    Returns the last error that occurred on the database or in the driver. 
 *    \sa WSqlError
 */

WSqlError WSqlDatabase::error() const
{
    return _driver->error();
}

/*!
 *    Returns all the errors that have occurred on the database or in the 
 * driver as a vector of strings.
 * TODO .. decide if we use this ..
 *    \sa WSqlError
 */

// WSqlError WSqlDatabase::errors() const
// {
//     return _driver->getError();
// }


/*!\brief Returns a vector of the database's table names
 * 
  This function returns a vector of strings containing the names tables and views 
  as specified by the parameter \a type. The vector is cached for quick reference -
  if empty it is initialized by a request to the driver for the names. If the names
  are not available an empty vector is returned.
  
  \warning If the table metadata has not been initialized yet this method will invalidate 
  any previous WSqlResult pointer returned - in this case nesting calls to this method inside 
  of a loop iterating over WSqlResults WILL NOT WORK. Obtain the WSqlTable \em first and 
  \em then execute() a query and fetch the result set using result() or use initMetaData()
  to initialize the metadata for all tables at once.
  
TODO: Use the table type - currently does nothing.

  \sa WSql::TableType
  \param WSql::TableType
  \retval std::vector<std::string>& - the table names in this database
*/
const std::vector<std::string>& WSqlDatabase::tableNames(WSql::TableType type)
{
	if(_tableNames.empty())
        _tableNames = _driver->tableNames();
    return _tableNames;
}

/*!\brief Fetch metadata for table \a tableName
 * 
    This method returns a WSqlTable object populated with WSqlColumns that
    contain metadata for the columns in the given table \a tableName.
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
* }
 
 \endcode
 
 \warning If the table metadata has not been initialized yet this method will invalidate 
 any previous WSqlResult pointer returned - in this case nesting calls to this method inside 
 of a loop iterating over WSqlResults WILL NOT WORK. Obtain the WSqlTable \em first and 
 \em then execute() a query and fetch the result set using result() or use initMetaData()
 to initialize the metadata for all tables at once.
 
 \param string the name of the table to use
 \retval WSqlTable an object containing metadata 
 \sa WSqlTable WSqlColumn
*/
WSqlTable WSqlDatabase::tableMetaData( const std::string& tablename ) const
{
    if(!isValid())
        return WSqlTable();
    return _driver->tableMetaData(tablename);
}

/*!  \brief Initializes the metadata for all tables in the database
 
 This method can be used to initialize all the metadata for the the database
 at once - this is convenient if one wishes to then use table metadata while
 also conducting queries. If this is called before tableMetaData() the metadata
 for all tables is cached in the driver and will be returned for a given table from
 the cache.
 
 \note This also initializes the referenced tables WSqlReferencedKeys - used
 by the ORM generation.  If you need access to this kind of metadata you must
 use this method to initialize the referenced key lists - if you initialize table
 metadata individually the reference keys will be omitted.
 
 \sa tableMetaData() tableNames()
*/

void WSqlDatabase::initMetaData()
{
    //load all tablenames
    tableNames();
    //init metadata for all tables
    std::vector<std::string>::const_iterator it = _tableNames.begin();
    for(;it != _tableNames.end();++it )
        tableMetaData(*it);
    std::vector<WSqlTable>::const_iterator tbl_it = _driver->_tables.begin();
    //add referenced keys
    for(;tbl_it != _driver->_tables.end();++tbl_it)
    {
        std::vector<WSqlForeignKey>::const_iterator fk_it = tbl_it->foreignKeys().begin();
        for(;fk_it != tbl_it->foreignKeys().end();++fk_it)
        {
            WSqlTable *table = _driver->getTable( fk_it->referencedTableName());
            if(table)
            {
                WSqlReferencedKey rfk(*fk_it);
                table->addReferencedKey(rfk);
            }
        }
    }
}


/*! \brief Sets the connection options for this database server
    
    Sets database-specific \a options. This must be done before the
    connection is opened or it has no effect (or you can close() the
    connection, call this function and open() the connection again).
    
    Note that the options are specific to a database server - drivers should handle
    this string appropriately.
    
    \param std::string& options - connection options to pass to the driver.
    \sa getConnectionOptions()
*/

void WSqlDatabase::setConnectOptions(const std::string &options)
{
     _connectionOptions = options;
}

/*!
    Returns the connection options string used for this connection.
    The string may be empty.

    \sa setConnectOptions()
 */
std::string WSqlDatabase::connectionOptions() const
{
    return _connectionOptions;
}

/*!
    Returns true if the WSqlDatabase has a valid driver.
*/
bool WSqlDatabase::isValid() const
{
    return (_isValid && 0 != _driver && _driver->isValid());
}
/*!
 * Executes the SQL in string \a sql - returns true on success.
 */
bool WSqlDatabase::execute(const std::string& sql)
{
    return _driver->execute(sql);
}
/*! \brief Returns a pointer to the result set from the most recent query
 * 
 * This method returns a pointer to a WSqlResult object containing
 * the most recent result set from a query - it may be empty if no results
 * were returned. You must not delete this pointer yourself - it is owned
 * by the driver.
 * 
 * The parameter \a iscached may be set to true to indicate a non-cached result
 * set that will be fetched row by row - the default is true and results are cached.
 * 
 * \note Only use this \em after an execute() query! Do not use twice in a row as it will delete
 * the previous result and return a newly created object. Example:
 * 
 * \code
 * WSqlDatabase db;
 * if (!db.open()) 
 *    dosomeerror();
 * if (!db.execute(std::string("select foo from bar")))
 *    dosomeerror();
 * WSqlResult *result = db.result();
 * //WSqlResult *result2 = db.result(); <- wrong 
 * //...iterate over results ..._then repeat:
 * if (!db.execute(std::string("select baz from bar")))
 *    dosomeerror();
 * WSqlResult *result = db.result();
 * ..etc.
 * \endcode
 * \sa WSqlResult WSqlRecord WMysqlDriver WSqliteDriver
 * 
 * \param bool iscached - if true (the default) fetches entire result set at once.
 * \retval WSqlResult* - the result set 
 */
WSqlResult * WSqlDatabase::result(bool iscached)
{
    return _driver->result(iscached);
}

    
} //namespace WSql
