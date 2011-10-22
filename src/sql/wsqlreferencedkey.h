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


#ifndef WSQLREFERENCEDKEY_H
#define WSQLREFERENCEDKEY_H
#include <string>
#include "wsqlforeignkey.h"

namespace WSql{
    
class WSqlReferencedKey
{

public:
    WSqlReferencedKey(const WSqlForeignKey &fk);
    WSqlReferencedKey(const WSqlReferencedKey& other);
    virtual ~WSqlReferencedKey();
    virtual WSqlReferencedKey& operator=(const WSqlReferencedKey& other);
    virtual bool operator==(const WSqlReferencedKey& other) const;
    inline bool operator!=( const WSqlReferencedKey& other ) const {
        return !operator==( other );
    }
        
    const std::string& columnName()const{return _referingKey.referencedColumnName();}
    const std::string& referingKeyName()const{return _referingKey.keyName();}
    const std::string& referingColumnName()const{return _referingKey.columnName();}
    const std::string& referingTableName()const{return _referingKey.tableName();}
    const std::string& referingSchemaName()const{return _referingKey.schemaName();}
    
private:
    WSqlForeignKey _referingKey;
    
};

}//namespace WSql
#endif // WSQLREFERENCEDKEY_H
