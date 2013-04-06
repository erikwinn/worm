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

#include "wsqldatum.h"

namespace WSql{    
    /*!
     *    \class WSqlDatum
     *    \brief The WSqlDatum class represents an abstraction of a data element retrieved from a database
     *    
     *    This class provides a flexible type container for a single value found in a database column. It can
     *    be instatiated to hold the types supported by WSqlDataType. It includes methods for conversion
     *   to various types on demand.
     * 
     * \todo - resolve: whether to use boost::any instead of string for internal storage .. or, revert to the 
     * previous void pointer version?  why? unsure of data integrity, eg. blob to string, longlong, etc .. contrary
     * to initial assumptions, some dbms may return types in the api (sqlite?) rather than string reps thereof.
     * regardless, blob is another issue, may be huge .. or streamable .. think on this. 
     *    
     *    \ingroup WSql
     *    \sa WSqlDataType WSqlField WSqlColumn
     */
    
WSqlDatum::WSqlDatum()
{
}

WSqlDatum::WSqlDatum( const WSqlDatum& other )
{
    _data = other._data;
}

WSqlDatum::~WSqlDatum()
{
}

WSqlDatum& WSqlDatum::operator=( const WSqlDatum & other )
{
    _data = other._data;
    return *this;
}

bool WSqlDatum::operator==( const WSqlDatum& other ) const
{
    return ( _data.compare(other._data) == 0 );
}


/*!
 *  \fn template <typename T> T data()
 *    Returns the data value converted to the type specified .
 * Ex:
 *  double d = data<double>();
 *  int i = data<int>();
 *  string s = data<string>();
 * 
 *    \sa setDefaultValue()
 */
/*!
 *    Erases the value stored..
 */
void WSqlDatum::clear()
{
    _data.erase();
}

    
}//namespace WSql
