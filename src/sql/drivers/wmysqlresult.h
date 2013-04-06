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


#ifndef WMYSQLRESULT_H
#define WMYSQLRESULT_H

#include "wsqlresult.h"
#include <mysql/mysql.h>

namespace WSql
{
 
class WMysqlResult : public WSqlResult
{

    public:
        WMysqlResult( const WSql::WSqlDriver* driver );
//        WMysqlResult( const WMysqlResult& other );
        ~WMysqlResult();
//        WMysqlResult& operator=( const WMysqlResult& other );
//        bool operator==( const WMysqlResult& other ) const;
        
        void addRecord( WSqlRecord& r ) {_records.push_back( r );}
        void setIsCached( bool b ) { _isCached = b; }

        bool isCached() {return _isCached; }
        int size()const {return _records.size(); }
        int count()const {return size(); }
        bool empty()const { return _records.empty();}
        bool hasOutValues() const { return !empty();}

        void clear() { _records.clear(); }

        WSqlRecord current();
        WSqlRecord fetch( int i = -1 );
        WSqlRecord fetchFirst();
        WSqlRecord fetchLast();
        WSqlRecord fetchNext();
        WSqlRecord fetchPrevious();

    protected:
        
        bool seek( int i, bool relative = false );
        bool next();
        bool previous();
        bool first();
        bool last();


    private:
        friend class WMysqlDriver;
        void freeResult();
        void setResult(MYSQL_RES *res){_mysqlResult = res;}
        MYSQL_RES * getResult(){return _mysqlResult;};
        MYSQL_RES *_mysqlResult;
};

} //namespace WSql

#endif // WMYSQLRESULT_H
