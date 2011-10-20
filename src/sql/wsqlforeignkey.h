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


#ifndef WSQLFOREIGNKEY_H
#define WSQLFOREIGNKEY_H

#include <string>

namespace WSql{
    
class WSqlForeignKey
{

    public:
        WSqlForeignKey();
        WSqlForeignKey( const WSqlForeignKey& other );
        virtual ~WSqlForeignKey();
        virtual WSqlForeignKey& operator=( const WSqlForeignKey& other );
        virtual bool operator==( const WSqlForeignKey& other ) const;
        inline bool operator!=( const WSqlForeignKey& other ) const {
            return !operator==( other );
        }
        
        void setKeyName(std::string name){_keyName=name;}
        void setColumnName(std::string name){_columnName=name;}
        void setReferencedTableName(std::string name){_referencedTableName=name;}
        void setReferencedColumnName(std::string name){_referencedColumnName=name;}
        void setReferencedSchemaName(std::string name){_referencedSchemaName=name;}
        const std::string& keyName()const{return _keyName;}
        const std::string& columnName()const{return _columnName;}
        const std::string& referencedColumnName()const{return _referencedColumnName;}
        const std::string& referencedTableName()const{return _referencedTableName;}
        const std::string& referencedSchemaName()const{return _referencedSchemaName;}
        
    private:
        std::string _keyName;
        std::string _columnName;
        std::string _referencedTableName;
        std::string _referencedColumnName;
        std::string _referencedSchemaName;
};

}//namespace WSql
#endif // WSQLFOREIGNKEY_H
