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

#include "wsqldriver.h"
#include "wsqlerror.h"
#include "wsqlfield.h"

namespace WSql {

/*!
    \class WSqlDriver
    \brief The WSqlDriver class is an abstract base class for database drivers

    This class can not be used directly. Use either WSqlDriverFactory to create
    a driver for a specific database (eg. SQLite or PostgreSQL ) or WSqlDatabase
    to access the database (recommended).

	This class must be subclassed by specific driver classes for any supported
	database server.
	\sa WSqlDatabase WSqliteDriver WMysqlDriver
*/
/*!\brief Construct a driver with the given database \a db
 * 
Constructs a driver with a pointer to the database - note that this is the only
way to create a driver; all connection information (eg. database name, user name, etc. )
is managed by WSqlDatabase - the driver will obtain what it needs to know from there.

\param WSqlDatabase* db - the database using this driver.
 */
WSqlDriver::WSqlDriver(WSqlDatabase* db)
{
	_database=db;
    _result=0;
	_isOpen=false;
	_isValid=false;
	_hasError=false;
}

/*!\brief Destroys the driver and created resources
 * 
 * Note that the result pointer obtained from result() is \em invalid after the 
 * object is destroyed! This means you do not need to (and should not) delete 
 * a result from the driver and that you also must be careful not to use it after 
 * a WSqlDriver object has been destroyed.
 */
WSqlDriver::~WSqlDriver()
{
    delete _result;
    _result = 0;
    _tables.clear();
    _errors.clear();
}

/*!\brief Set an error with \a text and types and also set isValid

   A convenience function for creating and setting the error to an error with
 the message in \a text of ErrorType \a type and ErrorSeverity \a severity. 
 Also sets hasError to be true and isValid to be \a isvalid - Note: the default for 
 isvalid is "true".
 \param std::string text - the message
 \param WSqlError::ErrorType type - what kind of error
 \param WSqlError::ErrorSeverity severity - how severe the error is
 \param bool isvalid - set the isValid flag of the driver to isvalid (default true).
 */
void WSqlDriver::setError(const std::string& text, WSqlError::ErrorType type, 
						  WSqlError::ErrorSeverity severity, bool isvalid)
{
	setError(WSqlError(text,-1,type,severity)); 
	setIsValid(isvalid);
	setHasError(true);
}
/*! \brief Sets the the current error to \a error
 * 
 *    This method sets the current error and pushes any previous error
 *    onto the error stack.
 *    \param WSqlError error - the error to set
 */
void WSqlDriver::setError(const WSqlError& error ) 
{
    if(_hasError)  
        _errors.push_back(_error);
    _error = error; 
    _hasError = true;
}

/*! \brief Locate the metadata table for \a tablename in the cache
 * 
 * This returns a WSqlTable of metadata for a given table by looking
 * it up in the local cache. If the table has been initialized it will be returned
 * if not an invalid (ie. empty) WSqlTable object will be returned.
 *     \param std::string tablename - the name of the table to find
 *     \retval WSqlTable valid if found in cache
 */
WSqlTable WSqlDriver::findTable( std::string tablename ) const
{
    if ( !_tables.empty() ) {
        std::vector<WSqlTable>::const_iterator it = _tables.begin();
        for ( ; it != _tables.end();++it )
            if ( it->name().compare( tablename ) == 0 )
                return *it;
    }
    return WSqlTable();
}

/*! \brief Return a reference to the metadata table for \a tablename in the cache
 * 
 * This returns a reference to the WSqlTable of metadata for a given table by looking
 * it up in the local cache. If the table has been initialized it will be returned
 * if not  a null (0) pointer will be returned. This function is used internally to modify 
 * tables - it is not recommended for use otherwise.
 * 
 *     \param std::string tablename - the name of the table to find
 *     \retval WSqlTable* valid if found in cache
 */
WSqlTable* WSqlDriver::getTable( const std::string& tablename )
{
    WSqlTable* ptrToReturn = 0;
    if ( !_tables.empty() ) {
        std::vector<WSqlTable>::iterator it = _tables.begin();
        for ( ; it != _tables.end();++it )
            if ( it->name().compare( tablename ) == 0 )
            {
                ptrToReturn = &(*it);
                break;
            }
    }
    return ptrToReturn;
}

/*!
    \fn bool WSqlDriver::open()

    Derived classes must reimplement this pure virtual function to
    open a database connection on the database

    The function must return true on success and false on failure.

    \sa setOpen()
*/

/*!
    \fn bool WSqlDriver::close()

    Derived classes must reimplement this pure virtual function in
    order to close the database connection. Return true on success,
    false on failure.

    \sa open(), setOpen()
*/

/*! 
    \fn bool WSqlDriver::execute(const std::string &sql)

    Executes the SQL statement in \a sql and returns true if there were no
    errors.  Note that if the query was a SELECT expecting results the 
    WSqlResult is initiated before return and can be accesses using result().
    If there were no records returned result() will return an empty result.
    \sa WSqlResult result()
*/

/*!
    \fn  bool WSqlDriver::isOpen() const
    
    Returns true if the database connection is open; otherwise returns
    false.
*/

/*!
	\fn bool WSqlDriver::hasError() const
	
	Returns true if the there was an error during the last action, otherwise returns false.
*/

/*!
 * \fn WSqlError WSqlDriver::error() const

    Returns a WSqlError object which contains information about the
    last error that occurred on the database.
*/
/*!
    \fn std::vector<std::string> WSqlDriver::tableNames(WSql::TableType) const
    
    Returns a list of the names of the tables in the database. 

	\a tableType  defines what types of tables to return in the list,
	meaning: normal tables, views, system tables or all of the above.
	
	\sa WSql::TableType
*/

/*!
    \fn WSqlTable tableMetaData(const std::string tableName)
    
    Returns a WSqlTable initialized with metadata for the table \a tableName.
    If table \a tableName does not exist it returns an empty table.

    \sa WSqlTable WSqlColumn
 */
    
} //namespace WSql
