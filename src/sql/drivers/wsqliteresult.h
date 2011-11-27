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


#ifndef WSQLITERESULT_H
#define WSQLITERESULT_H

#include "wsqlresult.h"

namespace WSql {

class WSqliteDriver;

class WSqliteResult : public WSqlResult
{

public:
    WSqliteResult(const WSqliteDriver* driver);
//    WSqliteResult(const WSqliteResult& other);
/*    ~WSqliteResult();
    WSqliteResult& operator=(const WSqliteResult& other);
    bool operator==(const WSqliteResult& other) const;*/
/*
	int size()const {return _records.size(); }
    int count()const {return size(); }
    bool hasOutValues() const { return ! _records.empty(); }	
	void clear(){ _records.clear(); }
*/
//     WSqlRecord current();
//     WSqlRecord fetch(int i = -1) ;
//     WSqlRecord fetchNext();
//     WSqlRecord fetchPrevious();
//     WSqlRecord fetchFirst();
//     WSqlRecord fetchLast();
// 
//     bool seek(int i, bool relative = false);
//     bool next();
//     bool previous();
//     bool first();
//     bool last();

};

}//namespace WSql

#endif // WSQLITERESULT_H
