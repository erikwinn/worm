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


#ifndef WMYSQLDRIVER_H
#define WMYSQLDRIVER_H

#include "wsqldriver.h"
#include <mysql/mysql.h>


namespace WSql
{
    
    
class WMysqlDriver : public WSqlDriver
{

    public:
        WMysqlDriver( WSqlDatabase* db );
        ~WMysqlDriver();
        bool open();
        void close();
        bool doQuery(std::string sql );
//        bool execute(std::string sql );
        WSqlResult* getResult(bool iscached=true);
        // WSqlResult exec(const WSqlQuery &queryObject);
        std::vector<std::string> tableNames();
        WSqlTable tableMetaData( const std::string &tableName );
        bool isOpen();
        
    private:
        bool init();
        std::string local_escape_string( std::string& escapeme);
        void initColumnType(WSqlColumn &clm, std::string description);
        std::string escape_mysql_keyword( std::string escapeme );
		
        MYSQL * _mysql;

};


} // namespace WSql

#endif // WMYSQLDRIVER_H
