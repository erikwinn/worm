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

#include "wsqldatum.h"

namespace WSql{    

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
