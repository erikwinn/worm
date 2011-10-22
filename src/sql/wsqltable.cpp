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

namespace WSql {
    
/*!
    \class WSqlTable
    \brief WSqlTable is an abstraction for a single table, collection or view in a database.

    WSqlTable is an abstraction for a single table, collection or view in a database. It contains
    meta information about the columns as they exist in the database itself. It also contains
    information about indices, foriegn keys, primary keys, etc.  It is used primarily by the
    ORM generator.

    Tables to be generated may have any naming schema - but it is recommended that this
    is at least consistant. Worm will not alter the names but rather takes them exactly as they
    are from the the names in the database - so "my_table" will produce a class object 
    "class my_table", a column named firstName will be generated as firstName, First_Name
    as First_Name, etc.  If you are designing a schema this may be taken into consideration
    as it will effect the style of the generated code.

    Note: For NoSQL databases this class may not be useful for column metadata.
    \sa WSqlColumn WSqlDatum WSqlDataType
*/
/*! \enum WSqlTable::Type
    \brief A flag indicating the type of table this is
    Tables can be of various types:
    \li NORMAL - this is an average normal table, the default
    \li VIEW - this is an artificial table generated on demand by the server, these
    may or may not be update capable.
    \li JOIN - also known as an "association" table, maps many to many relationships
    These have two foriegn keys to other tables, they may optionally have other columns.
    To ensure that the ORM correctly accounts for these they should be named foo_assn
    and have the first two columns as the foriegn keys. This will make the analysis more
    reliable.
    \li TYPETABLE - subtle: this is a simple table with two columns, a name and an index
    it is like an extensible enum. Users can add or remove types, eg. a usergroup or role
    might have a type_id column foriegn key to a type table - this makes for fast indexing
    and lookup as well as flexible types that an administrator can alter without changing
    the first table. To make clear to the ORM analysis that a table is one of these they should
    be named name_type, eg. usergroup_type or product_type. Worm will generate a special
    kind of class for this containing only the types as a enum like class.  When the types are
    altered this class should be regenerated.
 
 */
/*!
    Constructs an empty table object.

    \sa isEmpty(), setName()
*/
WSqlTable::WSqlTable()
{
    _type = NORMAL;
    _isValid = false;
}
/*!
    Constructs an empty table object with the name \a name..

    \sa isEmpty(), setName()
*/
WSqlTable::WSqlTable( const std::string name )
{
    _type = NORMAL;
    _name = name;
    _isValid = false;
}

/*!
    Constructs a copy of \a other.
*/
WSqlTable::WSqlTable( const WSqlTable &other )
{
    _type = other._type;
    _isValid = other._isValid;
    _name = other._name;
    _columns = other._columns;
    _foreignKeys = other._foreignKeys;
    _referencedKeys = other._referencedKeys;
}
/*!
    Sets this table equal to \a other.
*/
WSqlTable& WSqlTable::operator=( const WSqlTable& other )
{
    _type = other._type;
    _isValid = other._isValid;
    _name = other._name;
    _columns = other._columns;
    _foreignKeys = other._foreignKeys;
    _referencedKeys = other._referencedKeys;
    return *this;
}
/*!
    Destroys the object and frees any allocated resources.
*/
WSqlTable::~WSqlTable()
{
    _columns.clear();
    _foreignKeys.clear();
    _referencedKeys.clear();
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
    if(_type != other._type)
        return false;
    if ( _columns.size() !=  other._columns.size()
            || _name.compare( other._name ) != 0 )
        return false;

    std::vector<WSqlColumn>::const_iterator it;
    std::vector<WSqlColumn>::const_iterator ito = other._columns.begin();
    for ( it = _columns.begin();it != _columns.end();++it ) 
     {
         if ( ito == other._columns.end() || *( it ) != *( ito ) )
            return false;
        else
            ito++;
    }
    std::vector<WSqlForeignKey>::const_iterator fit;
    std::vector<WSqlForeignKey>::const_iterator fito = other._foreignKeys.begin();
    for ( fit = _foreignKeys.begin(); fit != _foreignKeys.end(); ++fit ) 
    {
        if (  fito == other._foreignKeys.end() || *( fit ) != *( fito ) )
            return false;
        else
            fito++;
    }
    std::vector<WSqlReferencedKey>::const_iterator rfit;
    std::vector<WSqlReferencedKey>::const_iterator rfito = other._referencedKeys.begin();
    for ( rfit = _referencedKeys.begin();rfit != _referencedKeys.end();++rfit ) 
    {
        if ( rfito == other._referencedKeys.end() || *( rfit ) != *( rfito ) )
            return false;
        else
            rfito++;
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
    std::vector<WSqlColumn>::const_iterator it = _columns.begin();
    it += index;
    return it->columnName();
}

/*!
 Returns the WSqlColumn at postion \a index
 If the index is out of range it returns an empty WSqlColumn object.
 */
WSqlColumn WSqlTable::column( int index ) const
{
    WSqlColumn clmToReturn;
    int sz = _columns.size();
    if ( !sz || index < 0 || index > sz - 1 )
        return clmToReturn;
    std::vector<WSqlColumn>::const_iterator it = _columns.begin();
    it += index;
    return *it;
}

/*!
 Returns the WSqlColumn with (column) name \a fldname
 If the index is out of range it returns an empty WSqlColumn object.
 */
WSqlColumn WSqlTable::column( const std::string &fldname ) const
{
    WSqlColumn clmToReturn;
    if ( _columns.empty() )
        return clmToReturn;

    std::vector<WSqlColumn>::const_iterator it;
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
const std::vector<WSqlColumn>& WSqlTable::columns() const
{
    return _columns;
}
/*!
 Returns a list containing all the names of the columns in the table (which may be empty).
 */
std::vector<std::string> WSqlTable::columnNames() const
{
    std::vector<std::string> vecToReturn;
    std::vector<WSqlColumn>::const_iterator it;
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
    std::vector<WSqlColumn>::const_iterator it;
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
    std::vector<WSqlColumn>::iterator it = _columns.begin();
    it += pos;
    if ( replace )
        _columns.erase( it );
    _columns.insert( it, 1, column );
}

void WSqlTable::remove( int pos )
{
    int sz = _columns.size();
    if ( !sz || pos < 0 || pos > sz - 1 )
        return;
    std::vector<WSqlColumn>::iterator it = _columns.begin();
    it += pos;
    _columns.erase( it );
}

bool WSqlTable::isEmpty() const
{
    return _columns.empty();

}

WSqlForeignKey WSqlTable::foreignKey( const std::string columnname ) const
{
    std::vector<WSqlForeignKey>::const_iterator it = _foreignKeys.begin();
    for(;it != _foreignKeys.end(); ++it)
    {
        if (it->columnName().compare(columnname) == 0)
        {
            return *it;
        }
    }
    return WSqlForeignKey();
}

void WSqlTable::addForeignKey( const WSqlForeignKey& fk )
{
    _foreignKeys.push_back(fk);
}

const std::vector< WSqlForeignKey >& WSqlTable::foreignKeys() const
{
    return _foreignKeys;
}

void WSqlTable::removeForeignKey( const WSqlForeignKey& fk )
{
    std::vector<WSqlForeignKey>::iterator it = _foreignKeys.begin();
    for(;it != _foreignKeys.end(); ++it)
    {
        if (*(it) == fk)
        {
            _foreignKeys.erase(it);
            break;
        }
    }
}
void WSqlTable::removeForeignKey( int pos )
{
    int sz = _foreignKeys.size();
    if ( !sz || pos < 0 || pos > sz - 1 )
        return;
    std::vector<WSqlForeignKey>::iterator it = _foreignKeys.begin();
    it += pos;
    _foreignKeys.erase( it );
}
void WSqlTable::removeForeignKey( const std::string& keyname
 )
{
    if ( _foreignKeys.empty() )
        return;
    
    std::vector<WSqlForeignKey>::iterator it;
    for ( it = _foreignKeys.begin(); it != _foreignKeys.end(); ++it ) 
    {
        if ( it->keyName().compare( keyname ) == 0 ) 
        {
            _foreignKeys.erase(it);
            break;
        }
    }
}

WSqlReferencedKey WSqlTable::referencedKey( const std::string columnname ) const
{
    std::vector<WSqlReferencedKey>::const_iterator it = _referencedKeys.begin();
    for(;it != _referencedKeys.end(); ++it)
        if (it->columnName().compare(columnname) == 0)
            return *it;
    return WSqlReferencedKey(WSqlForeignKey());
}

void WSqlTable::addReferencedKey( const WSqlReferencedKey& fk )
{
    _referencedKeys.push_back(fk);
}

const std::vector< WSqlReferencedKey >& WSqlTable::referencedKeys() const
{
    return _referencedKeys;
}

void WSqlTable::removeReferencedKey( const WSqlReferencedKey& rk )
{
    std::vector<WSqlReferencedKey>::iterator it = _referencedKeys.begin();
    for(;it != _referencedKeys.end(); ++it)
    {
        if (*(it) == rk)
        {
            _referencedKeys.erase(it);
            break;
        }
    }
}
void WSqlTable::removeReferencedKey( int pos )
{
    int sz = _referencedKeys.size();
    if ( !sz || pos < 0 || pos > sz - 1 )
        return;
    std::vector<WSqlReferencedKey>::iterator it = _referencedKeys.begin();
    it += pos;
    _referencedKeys.erase( it );
}
void WSqlTable::removeReferencedKey( const std::string& columnname )
{
    if ( _referencedKeys.empty() )
        return;
    
    std::vector<WSqlReferencedKey>::iterator it;
    for ( it = _referencedKeys.begin(); it != _referencedKeys.end(); ++it ) 
    {
        if ( it->columnName().compare( columnname ) == 0 ) 
        {
            _referencedKeys.erase(it);
            break;
        }
    }
}

} // namespace WSql
