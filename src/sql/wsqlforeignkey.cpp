/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Erik Winn <email>

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


#include "wsqlforeignkey.h"
namespace WSql{
    
WSqlForeignKey::WSqlForeignKey()
{
}

WSqlForeignKey::WSqlForeignKey( const WSqlForeignKey& other )
{
    _columnName=other._columnName;
    _foreignColumnName=other._foreignColumnName;
    _foreignTableName=other._foreignTableName;
}

WSqlForeignKey::~WSqlForeignKey()
{
}

WSqlForeignKey& WSqlForeignKey::operator=( const WSqlForeignKey & other )
{
    _columnName=other._columnName;
    _foreignColumnName=other._foreignColumnName;
    _foreignTableName=other._foreignTableName;
    return *this;
}

bool WSqlForeignKey::operator==( const WSqlForeignKey& other ) const
{
   return(  _columnName.compare(other._columnName)== 0
   && _foreignColumnName.compare(other._foreignColumnName)== 0
   && _foreignTableName.compare(other._foreignTableName)== 0);
}

}//namespace WSql