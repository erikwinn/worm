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


#ifndef WSQLITEDRIVER_H
#define WSQLITEDRIVER_H

#include "wsqldriver.h"
#include <map>

#include <sqlite3.h>

namespace WSql
{

class WSqlDatabase;

class WSqliteDriver : public WSqlDriver
{
    public:
        //    WSqliteDriver();
        WSqliteDriver( WSqlDatabase* db );
        ~WSqliteDriver();
        WSqliteDriver& operator=( const WSqliteDriver& other );
        bool operator==( const WSqliteDriver& other ) const;
        bool open();
        void close();
        bool execute( std::string sql );
        WSqlResult* result(bool iscached=true);

        std::vector<std::string> tableNames();
        WSqlTable tableMetaData( const std::string &tableName );
        
    private:
        bool _isUtf8;
        sqlite3 *_objSqlite;
        
        bool init();

        WSqlDataType::Type sqlite3TypeToWSqlType( std::string tname) const;
        WSqlDataType::Type sqlite3TypeToWSqlType(int dtype)const;
        std::string fetchTableCreateStatement(const std::string& tablename)const;
        std::string extractStatement(const std::string& sql, const char start='(', const char end=')') const;        
        void parseSchema( std::string& sql);
        bool columnIsAutoIncrement(const std::string& columnname)const;
        void mapColumns(std::vector<std::string> &column_definitions);
        std::map<std::string, std::string> _columns_map;
        std::map<std::string, std::string> _constraints_map;
};

}//namespace WSql

#endif // WSQLITEDRIVER_H
