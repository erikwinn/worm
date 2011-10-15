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
        
        void setColumnName(std::string name){_columnName=name;}
        void setForeignTableName(std::string name){_foreignTableName=name;}
        void setForeignColumnName(std::string name){_foreignColumnName=name;}
        const std::string& columnName()const{return _columnName;}
        const std::string& foreignColumnName()const{return _foreignColumnName;}
        const std::string& foreigTableName()const{return _foreignTableName;}
        
    private:
        std::string _columnName;
        std::string _foreignTableName;
        std::string _foreignColumnName;
};

}//namespace WSql
#endif // WSQLFOREIGNKEY_H
