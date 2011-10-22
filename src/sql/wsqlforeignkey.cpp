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
    _keyName=other._keyName;
    _tableName=other._tableName;
    _schemaName=other._schemaName;
    _columnName=other._columnName;
    _referencedColumnName=other._referencedColumnName;
    _referencedTableName=other._referencedTableName;
    _referencedSchemaName=other._referencedSchemaName;
}

WSqlForeignKey::~WSqlForeignKey()
{
}

WSqlForeignKey& WSqlForeignKey::operator=( const WSqlForeignKey & other )
{
    _keyName=other._keyName;
    _tableName=other._tableName;
    _schemaName=other._schemaName;
    _columnName=other._columnName;
    _referencedColumnName=other._referencedColumnName;
    _referencedTableName=other._referencedTableName;
    _referencedSchemaName=other._referencedSchemaName;
    return *this;
}

bool WSqlForeignKey::operator==( const WSqlForeignKey& other ) const
{
   return( _keyName.compare(other._keyName) == 0 
   && _tableName.compare(other._tableName)== 0
   && _schemaName.compare(other._schemaName)== 0
   && _columnName.compare(other._columnName)== 0
   && _referencedColumnName.compare(other._referencedColumnName)== 0
   && _referencedSchemaName.compare(other._referencedSchemaName)== 0
   && _referencedTableName.compare(other._referencedTableName)== 0);
}

}//namespace WSql