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

#include "wsqlfield.h"
#include "wsqldatum.h"

namespace WSql
{

/*!
 *    \class WSqlField
 *    \brief The WSqlField class represents a field in a record
 *    
 *    WSqlField represents the data contained a single field of a record in a result set
 *    returned from a query. Each field contains the value stored as a WSqlDatum and the
 *    name of the field associated is a string. Note that the name of the field corresponds to
 *    either the column name as defined in the database or the alias defined in the query.
 *    The column name defined in the database is available using columnName(). Drivers
 *    should initialize both of these when constructing a field for a result set; users should
 *    initialize at least name and preferable columnName if using the field for an insert or
 *    update.
 *    
 *    \note Inserts and updates using WSqlField are not yet supported (10/2011) .
 *    
 *    WSqlFields are used to populate a WSqlRecord in a WSqlResult. See below for an example.
 *    
 *    The data value is converted on demand to supported types (currently native C++ types
 *    and std::string) and can be retrieved using the function data(). Alternately, the full
 *    WSqlDatum can be retrieved using rawData(). For example:
 * 
 *    \code
 * 
 * //assume a record is available ..
 * WSqlField fldName = record.field(string("user_name"));
 * string name = fldName.data<string>();
 * 
 * //or
 * int idnumber = record.field(string("user_id")).data<int>();
 * 
 * //or
 * WSqlDatum datum = field.rawData();
 * //do something with the datum ..
 * 
 *    \endcode
 *        
 *    Other methods provide more infomation about the field. columnName() returns the
 *    name of the column as defined in the database table (or view), isDirty() returns true
 *    if the field has been modified - note that this must be set by the user.
 *        
 *    The value of the data can be set using setData() which provides several overloads to
 *    all setting the data with the supported types. Note that the types are coverted to string
 *    and stored in the WSqlDatum.
 *    
 *  \todo maybe should implement deep copy of _data ..? also, missing copy ctor .. this only
 * applies if WSqlDatum changes the internal storage from string to boost::any ..
 * 
 * 
 *   \ingroup WSql
 *   \sa data() name() columnName() setData()
 *   \sa WSqlRecord WSqlDatum WSqlDataType
 */

/*!
    Constructs an empty WSqlField

    \sa setName()
 */
WSqlField::WSqlField()
{
    _precision = 2;
    _isDirty = false;
}

/*!
    Constructs an empty WSqlField with the name set to \a fieldName and the
    column name set to \a columnName. If \a columnName is not supplied it
    is assumed to be the same as fieldName and set accordingly.

    \sa setName() setDataType()
 */
WSqlField::WSqlField(const std::string& fieldName, const std::string& columnName)
{
    _precision = 2;
    _isDirty = false;
    _name = fieldName;
    if(columnName.empty())
        _columnName = fieldName;
    else
    _columnName = columnName;
}

/*!
    Sets the field equal to \a other.
 */
WSqlField& WSqlField::operator=(const WSqlField& other)
{
    _precision = other._precision;
    _isDirty = other._isDirty;
    _columnName = other._columnName;
    _name = other._name;
    _data = other._data;
    return *this;
}


/*! \fn bool WSqlField::operator!=(const WSqlField &other) const
    Returns true if the field is unequal to \a other; otherwise returns
    false.
 */

/*!
    Returns true if the field is equal to \a other; otherwise returns
    false.
 */
bool WSqlField::operator==(const WSqlField& other) const
{
    return (
            _precision == other._precision
            && _isDirty == other._isDirty
            && _data == other._data
            && _name.compare(other._columnName) == 0
            && _columnName.compare(other._columnName) == 0
    );
}

/*!
    Destroys the object and frees any allocated resources.
 */

WSqlField::~WSqlField()
{
}

/*!
 * \fn void WSqlField::setPrecision(int precision)
 *    Sets the field's \a precision. This only affects numeric fields.

    \sa getPrecision()
 */

/*!
 * \fn void WSqlField::setData(std::string value)
 * \brief Functions for setting the value of the data.
 
    These functions set the value of the field to \a value. If the field is read-only
    (isReadOnly() returns true), nothing happens.
    The data type of \a value must be either a native C++ type
    (eg. int, long, double, etc ) or a std::string. The value is converted
    (if numeric) and stored as a string. The default floating
    point precision for setting the value is 2 - greater precision can be
    had by using setPrecision() before setting the value.
    
    \todo implement that last sentence . set precision is tbd.
    .
    To set the value to NULL, use clear().

    \sa data() setPrecision() isReadOnly()
*/

/*!
    Clears the value of the field and resets the precision to the default
    If the field is read-only, nothing happens.

    \sa setData()
 */
void WSqlField::clear()
{
    _data.clear();
}

/*!
 *    \fn void WSqlField::setName(const std::string& name)
 *    Sets the name of the field to \a name. This will correspond to the 
 * column name or be the alias defined in the query.
 * 
 *    \sa name()
 */

/*!
 *    \fn void WSqlField::setColumnName(const std::string& name)
 *    Sets the column name of the field to \a name. Note that this does
 *    not (yet) effect the actual column name in the database.
 * 
 *    \sa columnName()
 */

/*!
 * \fn std::string WSqlField::columnName() const
 *  \brief  Returns the name of the column as defined in the database..
 * 
 * Note that this is the name as it is in the actual database and not necessarily
 * the name of the field returned which may be an alias defined in the query.
 *    \sa name() setColumnName()
 */
/*!
 * \fn std::string WSqlField::name() const
 *  \brief  Returns the name of the field in the record
 * 
 * Note that this may be different from the actual name of the column defined in
 * the database if an alias was defined in the query.
 *    \sa columnName() setColumnName()
 */

/*!
    Returns the field's type as defined in the database.
\todo decide if WSqlField needs/supports this (and other metadata) - also readOnly and
so on ..
    \sa setDataType()
 */

/*!
 * \fn template <typename T> T WSqlField::data()
 * 
 *    Returns the value of the data in various formats. Valid return values
 * are native C++ types (eg. int, long, float, double, etc.) or std::string.
 *  Example:
 * \code
 * 
 *  WSqlField field;
 * // ... set field with some value
 * double d = 1223.34;
 * field.setData(d);
 * 
 * double dd = field.data<double>(); //dd contains 1223.34
 * int i = field.data<int>(); //i contains 1223
 * string s = field.data<string>(); //s contains "1223.34"
 * 
 * \endcode
 * 
 *    \sa setData() WSqlDatum
 */

} //namespace WSql
