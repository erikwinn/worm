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

//!\todo FINISH ME .. comments
// also, tests ..
//!\todo CHECKME ..test

#include "wsqlcolumn.h"

namespace WSql
{

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
 metadata about columns but for actual interaction with the database use WSqlField.

   \ingroup WSql

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

WSqlColumn::WSqlColumn ( const WSqlColumn &other )
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
	//!\todo test this!!
	_default = other._default;
}

WSqlColumn::~WSqlColumn()
{

}
/*!
 *    Sets this column equal to \a other.
 */
WSqlColumn &WSqlColumn::operator= ( const WSqlColumn &other )
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
 *    Returns true if this column is identical to \a other; otherwise returns
 *    false.
 */
bool WSqlColumn::operator== ( const WSqlColumn &other ) const
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
 *    Sets the column's floating point \a precision. This only affects numeric columns.
 *
 * Note also that this does not (yet) change the actual definition in the database.
 * \todo the note above ..
 *
 *    \sa getPrecision()
 */
void WSqlColumn::setPrecision ( int precision )
{
	_precision = precision;
}

/*!
 * \fn  template <typename T> void setDefaultValue( const T value)
 *
 * Sets the default value used for this column to \a value. This function will
 * accept any native C++ numeric type as well as std::string - Note that the
 * \a value will be converted to a string for internal storage.
 *
 *    \sa defaultValue()
 */

/*!
 *    Sets the data type for the column.
 */
void WSqlColumn::setDataType ( WSqlDataType::Type type )
{
	_type = type;
}


/*! \brief Set the column name to be \a name
 *
 * This sets the column name to \a name. This refers to the name of
 * the column as defined in the database, eg. "first_name". Additionally
 * this method will set the _variableName as a transformed version of
 * the column name. Eg. "first_name" will be set as a variable name of
 * "firstName"
 *
 *    \sa columnName() setVariableName()
 */

void WSqlColumn::setColumnName ( const std::string &name )
{
	_columnName = name;
	_variableName = WSqlDataType::columnNameToVariable ( name );
}
/*! \brief Manually set the variableName to \a name
 *
 *  This sets the variable name to \a name. This is the name of the class variable
 * corresponding to the column name. So, for instance "first_name" will be a
 * variable firstName with ORM generated gettor firstName() and settor
 * setFirstName(string).
 *
 *  Note that manually setting the variable name is not normally used - it
 * is done automatically by setColumnName(). This is provided to override
 * the default behavior in cases like a view or alias where the column name
 * may be different from the field name.
 *
 *    \sa columnName()
 */

void WSqlColumn::setVariableName ( const std::string &name )
{
	_variableName = name;
}

/*!
 *    Returns the name of the column as defined in the database
 *    \sa getVariableName() setColumnName()
 */
const std::string &WSqlColumn::columnName() const
{
	return _columnName;
}
/*!
 *    Returns the name of the class variable that corresponds to the column.
 *    \sa getColumnName() setVariableName()
 */
const std::string &WSqlColumn::variableName() const
{
	return _variableName;
}

/*!
 *    Returns the column's type as stored in the database. Note that numerical
 * values are also stored as string to prevent precision loss (and since that is
 * how they are frequently received from the database) - and to facilitate easy conversion.
 *
 *    \sa setDefaultValue() defaultValue() setDataType()
 */
const WSqlDataType::Type WSqlColumn::type() const
{
	return _type;
}

/*!
 *    Returns true if this is not a required column and can be null; otherwise returns false.
 * In other words, "NOT NULL" has not been set for this column.Note that if the column
 * is AUTOINCREMENT this returns true as autoincrement columns should not have
 * values assigned.
 * \return bool true if column may be null
 */
bool WSqlColumn::canBeNull() const
{
	return ( _isAutoIncremented || _canBeNull );
}

/*!
 *    Returns the column's maximum length, ie. the number of characters
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
 *    Returns the column's precision; this is only meaningful for numeric
 * types. Note that the default is 2 which supports monetary format.
 *    \sa setPrecision()
 */
const int WSqlColumn::precision() const
{
	return _precision;
}
/*! \brief Returns a C++ type declaration
 * This method returns a string suitable for a type declaration of a variable in C++ code.
 */
std::string WSqlColumn::typeDeclaration() const
{
	std::string strToReturn;

	switch ( _type )
	{
		case WSqlDataType::NCHAR:
		case WSqlDataType::CHAR:
			strToReturn = "char";
			break;
		case WSqlDataType::TEXT:
		case WSqlDataType::TINYTEXT:
		case WSqlDataType::LONGTEXT:
		case WSqlDataType::MEDIUMTEXT:
		case WSqlDataType::VARCHAR:
		case WSqlDataType::NVARCHAR:
		case WSqlDataType::DATE:
		case WSqlDataType::DATETIME:
		case WSqlDataType::YEAR:
		case WSqlDataType::TIME:
		case WSqlDataType::TIMESTAMP:
		case WSqlDataType::TIMESTAMPTZ:
			strToReturn = "std::string";
			break;
		case WSqlDataType::TINYINT:
			strToReturn = "short";
			break;
		case WSqlDataType::SMALLINT:
		case WSqlDataType::MEDIUMINT:
		case WSqlDataType::INT:
			strToReturn = "int";
			break;
		case WSqlDataType::BIGINT:
			strToReturn = "long";
			break;
		case WSqlDataType::FLOAT:
			strToReturn = "float";
			break;
		case WSqlDataType::DECIMAL:
		case WSqlDataType::DOUBLE:
			strToReturn = "double";
			break;
		default:
			strToReturn = WSqlDataType::toString ( _type );
	}

	return strToReturn;
}
/*! \brief Returns true if the datatype of the column is supported by the ORM generator
 */
bool WSqlColumn::typeIsSupported() const
{
	switch ( _type )
	{
		case WSqlDataType::NCHAR:
		case WSqlDataType::CHAR:
		case WSqlDataType::TEXT:
		case WSqlDataType::TINYTEXT:
		case WSqlDataType::LONGTEXT:
		case WSqlDataType::MEDIUMTEXT:
		case WSqlDataType::VARCHAR:
		case WSqlDataType::NVARCHAR:
		case WSqlDataType::DATE:
		case WSqlDataType::DATETIME:
		case WSqlDataType::YEAR:
		case WSqlDataType::TIME:
		case WSqlDataType::TIMESTAMP:
		case WSqlDataType::TIMESTAMPTZ:
		case WSqlDataType::TINYINT:
		case WSqlDataType::SMALLINT:
		case WSqlDataType::MEDIUMINT:
		case WSqlDataType::INT:
		case WSqlDataType::BIGINT:
		case WSqlDataType::FLOAT:
		case WSqlDataType::DECIMAL:
		case WSqlDataType::DOUBLE:
			return true;
		default:
			return false;
	}
}

// template<> std::string WSqlColumn::defaultValue<std::string>()
// {
//     return _default.data<std::string>();
// };

/*!
 *  \fn template <typename T> T defaultValue()
 *    Returns the column's default value converted to the type specified .
 * Ex:
 *  double d = defaultValue<double>();
 *  int i = defaultValue<int>();
 *  string s = defaultValue<string>();
 *
 *    \sa setDefaultValue()
 */

}//namespace WSql
