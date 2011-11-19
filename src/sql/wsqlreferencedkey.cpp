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


#include "wsqlreferencedkey.h"
#include "wsqldatatype.h"
namespace WSql{
    /*! \class WSqlReferencedKey
     * \brief WSqlReferencedKey provides an abstraction of a referenced column in a database
     * 
     * This class provides a generic interface to a column referenced by a foreign key in databases
     * including information about tables and columns to which the foreign key refers and the
     * information about the refering key, column and table.
     * 
     * \ingroup WSql
     * \sa WSqlForeignKey
     */
WSqlReferencedKey::WSqlReferencedKey( const WSql::WSqlForeignKey& fk )
{
    _referingKey=fk;
}
    
WSqlReferencedKey::WSqlReferencedKey( const WSqlReferencedKey& other )
{
    _referingKey=other._referingKey;
}

WSqlReferencedKey::~WSqlReferencedKey()
{
}

WSqlReferencedKey& WSqlReferencedKey::operator=( const WSqlReferencedKey & other )
{
    _referingKey=other._referingKey;
    return *this;
}

bool WSqlReferencedKey::operator==( const WSqlReferencedKey& other ) const
{
    return _referingKey == other._referingKey;
}

std::string WSqlReferencedKey::referingClassName() const
{
    return WSqlDataType::tableNameToClass(_referingKey.tableName());
}
std::string WSqlReferencedKey::referingClassNamePlural() const
{
    return WSqlDataType::toPlural(referingClassName());
}
void WSqlReferencedKey::dump() const
{
    std::cerr << "**************  Referenced Key: **************" << std::endl;
    std::cerr << "columnName()" << columnName() << std::endl;
    std::cerr << "tableName()" << tableName() << std::endl;
    std::cerr << "schemaName()" << schemaName() << std::endl;
    std::cerr << "referingKeyName()" << referingKeyName() << std::endl;
    std::cerr << "referingColumnName()" << referingColumnName() << std::endl;
    std::cerr << "referingTableName()" << referingTableName() << std::endl;
    std::cerr << "referingSchemaName()" << referingSchemaName() << std::endl;
    std::cerr << "referingClassName()" << referingClassName() << std::endl;
    std::cerr << "referingClassNamePlural()" << referingClassNamePlural() << std::endl;    
    std::cerr << "*****************************************" << std::endl;
    
}

}//namespace WSql
