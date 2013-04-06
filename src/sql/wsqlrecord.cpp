/*
 *    WORM - a DAL/ORM code generation framework
 *    Copyright (C) 2011  Erik Winn <sidewalksoftware@gmail.com>
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

#include <vector>
#include <string>

#include "wsqlrecord.h"
#include "wsqlfield.h"

namespace WSql
{

/*!
    \class WSqlRecord
    \brief The WSqlRecord class represents a database record.

    The WSqlRecord class represents a database record (AKA a "row") returned
    from a SELECT query as a vector of WSqlFields. WSqlRecord supports
    adding and removing fields as well as setting and retrieving field names,
    data values and some metadata.

    The data values of a record's fields can be set by name or position
    with setData(). To find the position of a field by name use indexOf(),
    and to find the name of a field at a particular position use at().name()
    Use field() to retrieve a WSqlField object for a field by name or position.
    To get a vector of all WSqlFields use fields().

    There are some convenience operators for getting the data from fields as well -
    operator[] supports fetching the data from a field by its name:
    \code
    std::string fieldname = "user_name";
    std::string username = record[fieldname];
    \endcode
    This can also be used with plain const char* strings:
    \code
    std::string username = record["user_name"];
    \endcode

    A record can have fields added with append() or insert(), replaced
    with replace(), and removed with remove(). All the fields can be
    removed with clear(). The number of fields is given by count().

    WSqlRecord is used to populate a WSqlResult set.

    \ingroup WSql
    \sa WSqlField WSqlResult.

*/
WSqlRecord::WSqlRecord()
{
	_isValid = false;
}

WSqlRecord::WSqlRecord ( const WSql::WSqlRecord &other )
{
	_isValid = other._isValid;
	_fields = other._fields;
}
WSqlRecord::~WSqlRecord()
{
//!\todo
}

WSqlRecord &WSqlRecord::operator= ( const WSql::WSqlRecord &other )
{
	_isValid = other._isValid;
	_fields = other._fields;
	return *this;
}
bool WSqlRecord::operator== ( const WSql::WSqlRecord &other ) const
{
	return false;
//!\todo
}
void WSqlRecord::setNull ( const std::string &name )
{
	setNull ( indexOf ( name ) );
}
void WSqlRecord::setNull ( int pos )
{
	_fields.at ( pos ).setData ( std::string() );
}

/*! \fn inline std::string WSqlRecord::operator[](const std::string fieldname)const
    \brief Operator to return the data in field \a fieldname
    This operator allows for easy access to field data - for example:
    \code
    std::string username = record[std::string("user_name")];
    \endcode
 */
/*!
    Returns the value of the field located at position \a index in
    the record. If \a index is out of bounds, an invalid WSqlDatum
    is returned.

    \sa fieldName() isNull()
*/

std::string WSqlRecord::data ( int pos ) const
{
	return field ( pos ).data<std::string>();
}

/*!
    \overload

    Returns the value of the field called \a name in the record. If
    field \a name does not exist an invalid WSqlDatum is returned.

    \sa indexOf()
*/
std::string WSqlRecord::data ( const std::string &colname ) const
{
	return field ( colname ).data<std::string>();
}

bool WSqlRecord::isNull ( const std::string &colname ) const
{
	return data ( colname ).empty();
}
bool WSqlRecord::isNull ( int index ) const
{
	return data ( index ).empty();
}

WSqlField WSqlRecord::field ( int pos ) const
{
	return at ( pos );
}

WSqlField WSqlRecord::at ( int pos ) const
{
	int sz = _fields.size();

	if ( !sz || pos < 0 || pos > sz - 1 )
		return WSqlField();

	return _fields.at ( pos );
}

WSqlField WSqlRecord::field ( const std::string &colname ) const
{
	return at ( indexOf ( colname ) );
}

int WSqlRecord::indexOf ( const std::string &name ) const
{
	int intToReturn = -1;

	if ( _fields.empty() )
		return intToReturn;

	int i = 0;
	std::vector<WSqlField>::const_iterator it;

	for ( it = _fields.begin(); it != _fields.end() ; ++it )
	{
		if ( it->name().compare ( name ) == 0 )
		{
			intToReturn = i;
			break;
		}

		++i;
	}

	return intToReturn;
}

void WSqlRecord::clear()
{
	_fields.clear();
}

} //namespace WSql
