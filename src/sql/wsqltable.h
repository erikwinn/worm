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


#ifndef WSQLTABLE_H
#define WSQLTABLE_H

#include <string>
#include <vector>
#include <list>

#include "wsqlcolumn.h"

namespace WSql {

class WSqlTable
{

public:
    WSqlTable();
	WSqlTable(const std::string name);
    WSqlTable(const WSqlTable& other);
    virtual ~WSqlTable();
    virtual WSqlTable& operator=(const WSqlTable& other);
    virtual bool operator==(const WSqlTable& other) const;
    inline bool operator!=(const WSqlTable &other) const { return !operator==(other); }

	const std::string& name()const {return _name;}

    const std::string columnName(int i) const;
    WSqlColumn column(int i) const;
    WSqlColumn column(const std::string &name) const;
	const std::list<WSqlColumn>& columns() const;
	std::vector<std::string> columnNames() const;

	int count() const;
	int indexOf(const std::string &columnname) const;
	
	void setName(const std::string &n){_name = n;}
    void setIsValid(bool b){_isValid = b;}
    
	void append(const WSqlColumn& column);
    void replace(int pos, const WSqlColumn& column);
    void insert(int pos, const WSqlColumn& column, bool replace=false);
    void remove(int pos);

    bool isEmpty()const;
    bool isValid()const{return _isValid;}
protected:
    //TODO: change this to a vector
	std::list<WSqlColumn> _columns;
	std::string _name;
    bool _isValid;
	
};

} //namespace WSql

#endif // WSQLTABLE_H
