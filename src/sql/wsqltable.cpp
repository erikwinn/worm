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

#include "wsqltable.h"

namespace WSql
{
/*!
    \class WSqlTable
    \brief WSqlTable is an abstraction for a single table, collection or view in a database.

    WSqlTable is an abstraction for a single table, collection or view in a database. It contains
    meta information about the columns as they exist in the database itself. It also contains
    information about indices, foriegn keys, primary keys, etc.  It is used primarily by the
    ORM generator.

    Note: For NoSQL databases this class may not be useful for column metadata.
    \sa WSqlColumn
*/

/*!
    Constructs an empty table object.

    \sa isEmpty(), setName()
*/
WSqlTable::WSqlTable()
{
    _isValid = false;
}
/*!
    Constructs an empty table object with the name \a name..

    \sa isEmpty(), setName()
*/
WSqlTable::WSqlTable( const std::string name )
{
    _name = name;
    _isValid = false;
}

/*!
    Constructs a copy of \a other.
*/
WSqlTable::WSqlTable( const WSqlTable &other )
{
    _isValid = other._isValid;
    _name = other._name;
    _columns = other._columns;
}
/*!
    Sets this table equal to \a other.
*/
WSqlTable& WSqlTable::operator=( const WSqlTable& other )
{
    _isValid = other._isValid;
    _name = other._name;
    _columns = other._columns;
    return *this;
}
/*!
    Destroys the object and frees any allocated resources.
*/
WSqlTable::~WSqlTable()
{
    _columns.clear();
}
/*!
    \fn bool WSqlTable::operator!=(const WSqlTable &other) const

    Returns true if this object is not identical to \a other;
    otherwise returns false.

    \sa operator==()
*/
/*!
    Returns true if this object is identical to \a other (i.e., has
    the same columns in the same order); otherwise returns false.

    \sa operator!=()
*/

bool WSqlTable::operator==( const WSqlTable &other ) const
{
    if(_isValid != other._isValid)
        return false;
    if ( _columns.size() !=  other._columns.size()
            || _name.compare( other._name ) != 0 )
        return false;

    std::list<WSqlColumn>::const_iterator it;
    std::list<WSqlColumn>::const_iterator ito = other._columns.begin();
    for ( it = _columns.begin(); it != _columns.end(); ++it ) {
        if ( *( it ) != *( ito ) )
            return false;
        else
            ito++;
    }
    return true;
}

/*!
 Returns the name of the column at postion \a index, ie. the column name in the database.
 If the index is out of range it returns an empty string.
 */
const std::string WSqlTable::columnName( int index ) const
{
    std::string strToReturn;
    int sz = _columns.size();
    if ( !sz || index < 0 || index > sz - 1 )
        return strToReturn;
    std::list<WSqlColumn>::const_iterator it = _columns.begin();
    for ( int i = 0; i != index; ++i )
        ++it;
    return it->columnName();
}

/*!
 Returns the WSqlColumn at postion \a index
 If the index is out of range it returns an empty string.
 */
WSqlColumn WSqlTable::column( int index ) const
{
    WSqlColumn clmToReturn;
    int sz = _columns.size();
    if ( !sz || index < 0 || index > sz - 1 )
        return clmToReturn;
    std::list<WSqlColumn>::const_iterator it = _columns.begin();
    for ( int i = 0; i != index; ++i )
        ++it;
    return *it;
}

/*!
 Returns the WSqlColumn with (column) name \a fldname
 If the index is out of range it returns an empty string.
 */
WSqlColumn WSqlTable::column( const std::string &fldname ) const
{
    WSqlColumn clmToReturn;
    if ( _columns.empty() )
        return clmToReturn;

    std::list<WSqlColumn>::const_iterator it;
    for ( it = _columns.begin(); it != _columns.end(); ++it ) 
    {
        if ( it->columnName().compare( fldname ) == 0 ) 
        {
            clmToReturn = *it;
            break;
        }
    }
    return clmToReturn;
}

/*!
 Returns a list containing all the WSqlColumns in the table (which may be empty).
 */
const std::list<WSqlColumn>& WSqlTable::columns() const
{
    return _columns;
}
/*!
 Returns a list containing all the names of the columns in the table (which may be empty).
 */
std::vector<std::string> WSqlTable::columnNames() const
{
    std::vector<std::string> vecToReturn;
    std::list<WSqlColumn>::const_iterator it;
    for ( it = _columns.begin(); it != _columns.end(); ++it )
        vecToReturn.push_back( it->columnName() );
    return vecToReturn;
}

int WSqlTable::count() const
{
    return _columns.size();
}

int WSqlTable::indexOf( const std::string &columnname ) const
{
    int intToReturn = -1;
    if ( _columns.empty() )
        return intToReturn;
    int i = 0;
    std::list<WSqlColumn>::const_iterator it;
    for ( it = _columns.begin(); it != _columns.end() ; ++it ) {
        if ( it->columnName().compare( columnname ) == 0 ) {
            intToReturn = i;
            break;
        }
        ++i;
    }
    return intToReturn;
}

void WSqlTable::append( const WSqlColumn& column )
{
    _columns.push_back( column );
}

void WSqlTable::replace( int pos, const WSqlColumn& column )
{
    insert( pos, column, true );
}

void WSqlTable::insert( int pos, const WSqlColumn& column, bool replace )
{
    int sz = _columns.size();
    if ( !sz || pos < 0 || pos > sz - 1 )
        return;
    std::list<WSqlColumn>::iterator it = _columns.begin();
    int i = 0;
    while ( i++ != pos )
        ++it;
    if ( replace )
        _columns.remove( *it );
    _columns.insert( it, 1, column );
}

void WSqlTable::remove( int pos )
{
    int sz = _columns.size();
    if ( !sz || pos < 0 || pos > sz - 1 )
        return;
    std::list<WSqlColumn>::iterator it = _columns.begin();
    int i = 0;
    while ( i++ != pos )
        ++it;
    _columns.remove( *it );
}

bool WSqlTable::isEmpty() const
{
    return _columns.empty();

}

} // namespace WORM
