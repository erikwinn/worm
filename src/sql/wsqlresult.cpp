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

#include "wsqlresult.h"

namespace WSql {

/*!
    \class WSqlResult
    \brief The WSqlResult class provides a representation of the result set from a query
    
    WSqlResult represents the results of a query to the database as a vector of WSqlRecords.
    By default isCached() returns true and the vector is populated by the WSqlDriver. If the
    query was not a select or returned no results then size() will be 0 and hasOutValues()
    will return false.

    If you are implementing your own SQL driver (by subclassing WSqlDriver), you 
    must also subclass WSqlResult and implement the  all the pure virtual functions 
    and other virtual functions that you need. This is mostly to support databases that
    allow partial retrieval of a result set - in which case the implementation will offer
    the ability to setIsCached() and manage the fetching of records internally.
    
    Example usage:
\code

std::string sql = "select * from " + tablename + " limit 2";
if(!db.execute(sql)) exit();

WSqlResult *result = db.result();
WSqlRecord record = result->fetchFirst();
while(!record.empty())
{
    WSqlField fld;
    for(int i = 0;i < record.size();i++)
    {
        fld = record.field(i);
        std::cout << "Field " << fld.name() << ", Origin Column "<< fld.columnName() 
        << "Value: " << fld.data<std::string>() << std::endl;
    }
    record = result->fetchNext();
}
\endcode

    \sa WSqlRecord WSqlField WSqlDatabase
*/


/*!
    Creates an empty result object.
*/
WSqlResult::WSqlResult(const WSqlDriver *driver)
{
    _driver = driver;
	_isCached=true;
    _isValid = false;  // this should be set by the driver after initializing.
    const_cur_record_it = _records.begin();
}

WSqlResult::WSqlResult( const WSqlResult& other )
{
    _driver = other._driver;
    _isCached = other._isCached;
    _isValid = other._isValid;
    _records = other._records;
    const_cur_record_it = other.const_cur_record_it;
}

WSqlResult& WSqlResult::operator=( const WSql::WSqlResult& other )
{
    _driver = other._driver;
    _isCached = other._isCached;
    _isValid = other._isValid;
    _records = other._records;
    const_cur_record_it = other.const_cur_record_it;
    return *this;
}

/*!
    Destroys the object and frees any allocated resources.
*/
WSqlResult::~WSqlResult()
{
    _records.clear();
}

/*!
    \fn int WSqlResult::size()

Returns the number of records in the result returned from a \c SELECT
statement. Note that this will be 0 if no results were returned..

    \sa numRowsAffected()
*/


/*!
    \fn WSqlRecord WSqlResult::fetch(int index)

    Returns the record from an arbitrary (zero-based) row \a index.

	If the index is not in valid range an empty record is returned.

    \sa isActive(), fetchFirst(), fetchLast(), fetchNext(), fetchPrevious()
*/

/*!
    \fn WSqlRecord WSqlResult::fetchFirst()

    Returns the record from the first record (row 0) in the result.

    \sa fetch(), fetchLast()
*/

/*!
    \fn WSqlRecord WSqlResult::fetchLast()

    Returns the record from the last record (last row) in the result.

    \sa fetch(), fetchFirst()
*/

/*!    
    \fn WSqlRecord WSqlResult::fetchNext()

    Returns the record from the next available record (row) in the
    result.

    \sa fetch(), fetchPrevious()
*/
/*!
    \fn WSqlRecord WSqlResult::fetchPrevious()
    Returns the record from the previous record (row) in the result.

*/

WSqlRecord WSqlResult::current()
{
    if(_records.end() != const_cur_record_it)
        return *const_cur_record_it;
    else
        return WSqlRecord(); 
}
WSqlRecord WSqlResult::fetch(int pos)
{
    int sz = _records.size();
    if ( !sz || pos < 0 || pos +1 > sz || !seek(pos, false) )
        return WSqlRecord();
    return *const_cur_record_it;
}
WSqlRecord WSqlResult::fetchFirst()
{
    if(_records.empty() || !first())
        return WSqlRecord();
    return *const_cur_record_it;
}
WSqlRecord WSqlResult::fetchLast()
{
    if(_records.empty() || !last())
        return WSqlRecord();
    return *const_cur_record_it;
}
WSqlRecord WSqlResult::fetchNext()
{
    if(_records.empty() || !next())
        return WSqlRecord();
    return *const_cur_record_it;
}
WSqlRecord WSqlResult::fetchPrevious()
{
    if(_records.empty() || !previous())
        return WSqlRecord();
    return *const_cur_record_it;
}
bool WSqlResult::first()
{
    const_cur_record_it = _records.begin();
    return _records.end() != const_cur_record_it;
}
bool WSqlResult::last()
{
    if(_records.empty())
        return false;
    const_cur_record_it = _records.end();
    const_cur_record_it--;
    return true;
}
bool WSqlResult::next()
{
    return seek(1,true); 
}
bool WSqlResult::previous()
{
    return seek(-1, true);
}

//TODO: test this
bool WSqlResult::seek( int pos, bool relative )
{
    long curpos = (const_cur_record_it - _records.begin());
    int max = _records.size() - 1;
    if(pos < 0 || pos > max)
        return false;
    if(relative)
    {
        if(curpos + pos > max)
            return false;
    }else{
        const_cur_record_it = _records.begin();
    }
    const_cur_record_it += pos;
    return true;
}

} //namespace WSql
