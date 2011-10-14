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

//TODO: FINISH ME .. comments
// also, tests ..
//TODO: CHECKME ..test

#include "wsqlcolumn.h"

namespace WSql{

/*! 
 * \class WSqlColumn
    \brief The WSqlColumn class represents a column in SQL database tables.
 
    WSqlColumn represents the characteristics of a single column in a
    database table, collection or view, including the column and variable
    name and the type of datum or value stored in the column at a single 
    row, including the default defined in the database if possible which 
    may be retrieved using defaultValue() - this is stored as a string. 
    
    A WSqlColumn object provides meta-data about the database column, 
    for example: columnName(), dataType(), maxLength(), precision(),
    defaultValue(),  and isReadOnly().
 
 NOTE: This class is part of the metadata component - it is not used for 
 queries, results, etc.  As yet seting values here has no effect on the database
 - this is used primarily by the ORM generator. It may also be used to get 
 metadata about fields but for actual interaction with the database use WSqlField.
    
    \sa WSqlTable WSqlDataType WSqlRecord WSqlField
*/

/*!
    *    Constructs an empty WSqlColumn
    */    
WSqlColumn::WSqlColumn()
{
    _columnName = std::string();
    _variableName = std::string();
    _isAutoIncremented = false;
    _isPrimaryKey = false;
    _isUnsigned = false;
    _canBeNull = true; //most dbms default .. 
    _precision = 2;
    _maxLength = -1;
    _type = WSqlDataType::NOTYPE;
}

WSqlColumn::WSqlColumn( const WSqlColumn& other )
{
    _columnName = other._columnName;
    _variableName = other._variableName;
    _isAutoIncremented = other._isAutoIncremented;
    _isPrimaryKey = other._isPrimaryKey;
    _isUnsigned = other._isUnsigned;
    _canBeNull = other._canBeNull;
    _precision = other._precision;
    _maxLength = other._maxLength;
    _type = other._type;
    //TODO: test this!!
    _default = other._default;
}

WSqlColumn::~WSqlColumn()
{

}
/*!
 *    Sets this column equal to \a other.
 */
WSqlColumn& WSqlColumn::operator=( const WSqlColumn & other )
{
    _columnName = other._columnName;
    _variableName = other._variableName;
    _isAutoIncremented = other._isAutoIncremented;
    _isPrimaryKey = other._isPrimaryKey;
    _isUnsigned = other._isUnsigned;
    _canBeNull = other._canBeNull;
    _precision = other._precision;
    _maxLength = other._maxLength;
    _type = other._type;
    _default = other._default;
    return *this;
}

/*! \fn bool WSqlColumn::operator!=(const WSqlColumn &other) const
 *    Returns true if this column is not identical to \a other; otherwise returns
 *    false.
 */

/*!
 *    Returns true if the field is identical to \a other; otherwise returns
 *    false.
 */
bool WSqlColumn::operator==( const WSqlColumn& other ) const
{
    return ( _columnName == other._columnName
        && _variableName == other._variableName
        && _isAutoIncremented == other._isAutoIncremented
        && _isPrimaryKey == other._isPrimaryKey
        && _isUnsigned == other._isUnsigned
        && _canBeNull == other._canBeNull
        && _maxLength == other._maxLength
        && _precision == other._precision
        && _default == other._default
        && _type == other._type
    );
}

/*!
 *    Sets the field's floating point \a precision. This only affects numeric fields.
 * 
 * Note also that this does not (yet) change the actual definition in the database.
 * TODO: the note above ..
 * 
 *    \sa getPrecision()
 */
void WSqlColumn::setPrecision(int precision)
{
    _precision = precision;
}

/*!
 * \fn  template <typename T> void setDefaultValue( const T value)
 *    
 * Sets the default value used for this field to \a value. This function will
 * accept any native C++ numeric type as well as std::string - Note that the
 * \a value will be converted to a string for internal storage.
 * 
 *    \sa defaultValue()
 */

/*!
 *    Sets the data type for the field. 
 */
void WSqlColumn::setDataType(WSqlDataType::Type type)
{
    _type = type;
}


/*!
 *    Sets the column name to \a name. This is the name of the column
 * as defined in the database, eg. "first_name"
 * 
 *   TODO: translate this and set _variableName automatically ..
 * 
 *    \sa columnName() setVariableName()
 */

void WSqlColumn::setColumnName(const std::string& name)
{
    _columnName = name;
}
/*!
 *    Sets the variable name to \a name. This is the name of the class variable
 * corresponding to the column name. So, for instance "first_name" will be a
 * variable _firstName with gettor firstName() and settor setFirstName(string)
 * 
 *    \sa columnName()
 */

void WSqlColumn::setVariableName(const std::string& name)
{
    _variableName = name;
}

/*!
 *    Returns the name of the column as defined in the database 
 *    \sa getVariableName() setColumnName()
 */
const std::string& WSqlColumn::columnName() const
{
    return _columnName;
}
/*!
 *    Returns the name of the class variable that corresponds to the column.
 *    \sa getColumnName() setVariableName()
 */
const std::string& WSqlColumn::variableName() const
{
    return _variableName;
}

/*!
 *    Returns the field's type as stored in the database. Note that numerical 
 * values are also stored as string to prevent precision loss (and since that is
 * how they are frequently received from the database) - and to facilitate easy conversion.
 * 
 *    \sa setDefaultValue() defaultValue() setDataType()
 */
const WSqlDataType::Type WSqlColumn::dataType() const
{
    return _type;
}

/*!
 *    Returns true if this is not a required field and can be null; otherwise returns false.
 * In other words, "NOT NULL" has not been set for this column.Note that if the field
 * is AUTOINCREMENT this returns true as autoincrement fields should not have 
 * values assigned.
 * \return bool true if field may be null
 */
bool WSqlColumn::canBeNull() const
{
    return( _isAutoIncremented || _canBeNull);
}

/*!
 *    Returns the field's maximum length, ie. the number of characters
 *    (eg. VARCHAR(255)) as defined in the database. If there is no maximum
 *    it returns -1.
 *    
 *    \sa setMaxLength() getDataType() canBeNull() getPrecision()
 */
const int WSqlColumn::maxLength() const
{
    return _maxLength;
}

/*!
 *    Returns the field's precision; this is only meaningful for numeric
 * types. Note that the default is 2 which supports monetary format.
 *    \sa setPrecision()
 */
const int WSqlColumn::precision() const
{
    return _precision;
}

// template<> std::string WSqlColumn::defaultValue<std::string>()
// {
//     return _default.data<std::string>();
// };

/*!
 *  \fn template <typename T> T defaultValue()
 *    Returns the field's default value converted to the type specified .
 * Ex:
 *  double d = defaultValue<double>();
 *  int i = defaultValue<int>();
 *  string s = defaultValue<string>();
 * 
 *    \sa setDefaultValue()
 */

}//namespace WSql