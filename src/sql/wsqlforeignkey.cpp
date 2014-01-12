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


#include "wsqlforeignkey.h"
#include "wsqldatatype.h"
namespace WSql
{

/*! \class WSqlForeignKey
 * \brief WSqlForeignKey provides an abstraction of a foreign key in a database
 *
 * This class provides a generic interface to foreign key relationships in databases
 * including information about tables and columns to which the foreign key refers.
 *
 * \ingroup WSql
 * \sa WSqlReferencedKey
 */

WSqlForeignKey::WSqlForeignKey()
{
}

WSqlForeignKey::WSqlForeignKey ( const WSqlForeignKey &other )
{
	_keyName = other._keyName;
	_tableName = other._tableName;
	_schemaName = other._schemaName;
	_columnName = other._columnName;
	_referencedColumnName = other._referencedColumnName;
	_referencedTableName = other._referencedTableName;
	_referencedSchemaName = other._referencedSchemaName;
}

WSqlForeignKey::~WSqlForeignKey()
{
}

WSqlForeignKey &WSqlForeignKey::operator= ( const WSqlForeignKey &other )
{
	_keyName = other._keyName;
	_tableName = other._tableName;
	_schemaName = other._schemaName;
	_columnName = other._columnName;
	_referencedColumnName = other._referencedColumnName;
	_referencedTableName = other._referencedTableName;
	_referencedSchemaName = other._referencedSchemaName;
	return *this;
}

bool WSqlForeignKey::operator== ( const WSqlForeignKey &other ) const
{
	return ( _keyName.compare ( other._keyName ) == 0
			 && _tableName.compare ( other._tableName ) == 0
			 && _schemaName.compare ( other._schemaName ) == 0
			 && _columnName.compare ( other._columnName ) == 0
			 && _referencedColumnName.compare ( other._referencedColumnName ) == 0
			 && _referencedSchemaName.compare ( other._referencedSchemaName ) == 0
			 && _referencedTableName.compare ( other._referencedTableName ) == 0 );
}

std::string WSqlForeignKey::referencedClassName() const
{
	return WSqlDataType::tableNameToClass ( _referencedTableName );
}

std::string WSqlForeignKey::referingVariableName() const
{
		//CRUDE HACK: FIXME ASAP .. rfind, check if at end ..			
	std::string strToReturn = columnName();
// 			std::cout << "strToReturn 0: " << strToReturn << std::endl;

	size_t pos = WSqlDataType::iFind(strToReturn, "_id");
	if(std::string::npos != pos)
	{
		strToReturn = strToReturn.erase(pos);
	}
// 			std::cout << "strToReturn 1: " << strToReturn << std::endl;
	strToReturn = WSqlDataType::columnNameToVariable(strToReturn);	
// 			std::cout << "strToReturn 2: " << strToReturn << std::endl;
	strToReturn[0] = toupper(strToReturn[0]);
// 			std::cout << "strToReturn 3: " << strToReturn << std::endl;
	
	//\HACK

    return strToReturn;
}

std::string WSqlForeignKey::referencedVariableName() const
{
    return WSqlDataType::columnNameToVariable(referencedColumnName());
}

std::string WSqlForeignKey::referencedClassNamePlural() const
{
	return WSqlDataType::toPlural ( referencedClassName() );
}
void WSqlForeignKey::dump() const
{
	std::cerr << "*****************  Foreign Key: *****************" << std::endl;
	std::cerr << "keyName()" << keyName() << std::endl;
	std::cerr << "tableName() " << tableName() << std::endl;
	std::cerr << "schemaName() " << schemaName() << std::endl;
	std::cerr << "columnName() " << columnName() << std::endl;
	std::cerr << "referencedColumnName() " << referencedColumnName() << std::endl;
	std::cerr << "referencedTableName() " << referencedTableName() << std::endl;
	std::cerr << "referencedSchemaName() " << referencedSchemaName() << std::endl;
	std::cerr << "referencedclassName() " << referencedClassName() << std::endl;
	std::cerr << "referencedclassNamePlural() " << referencedClassNamePlural() << std::endl;
	std::cerr << "*****************************************" << std::endl;
}

}//namespace WSql
