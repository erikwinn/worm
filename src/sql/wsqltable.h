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


#ifndef WSQLTABLE_H
#define WSQLTABLE_H

#include <string>
#include <vector>

#include "wsqlcolumn.h"
#include "wsqlforeignkey.h"
#include "wsqlreferencedkey.h"

namespace WSql {

class WSqlTable
{

public:
    enum Type{
        NORMAL,
        VIEW,
        JOIN,
        TYPETABLE // tricky - see comments
    };
    
    WSqlTable();
	WSqlTable(const std::string name);
    WSqlTable(const WSqlTable& other);
    virtual ~WSqlTable();
    virtual WSqlTable& operator=(const WSqlTable& other);
    virtual bool operator==(const WSqlTable& other) const;
    inline bool operator!=(const WSqlTable &other) const { return !operator==(other); }

    const std::string& name()const {return _name;}
    const std::string& className()const {return _className;}
    bool isEmpty()const;
    bool isValid()const{return _isValid;}
    int count() const;
    int indexOf(const std::string &columnname) const;
    const Type type()const{return _type;}
    
    const std::string columnName(int i) const;
    WSqlColumn column(int i) const;
    WSqlColumn column(const std::string &name) const;
	const std::vector<WSqlColumn>& columns() const;
	std::vector<std::string> columnNames() const;
    	
	void setName(const std::string &n);
	void setClassName(const std::string &n){_className = n;}
	void setIsValid(bool b){_isValid = b;}
    void setType(Type t){_type=t;}
	void append(const WSqlColumn& column);
    void replace(int pos, const WSqlColumn& column);
    void insert(int pos, const WSqlColumn& column, bool replace=false);
    void remove(int pos);
    
    void addForeignKey(const WSqlForeignKey & fk);
    void removeForeignKey(const WSqlForeignKey & fk);
    void removeForeignKey(int pos);
    void removeForeignKey(const std::string& columnname);

    void addReferencedKey(const WSqlReferencedKey & rk);
    void removeReferencedKey(const WSqlReferencedKey & rk);
    void removeReferencedKey(int pos);
    void removeReferencedKey(const std::string& columnname);
    
    bool hasForeignKeys()const{return ! _foreignKeys.empty();}
    bool hasReferencedKeys()const{return ! _referencedKeys.empty();}
    WSqlForeignKey foreignKey(const std::string columnname)const;
    WSqlReferencedKey referencedKey(const std::string columnname)const;
    const std::vector<WSqlForeignKey>& foreignKeys() const;
    const std::vector<WSqlReferencedKey>& referencedKeys() const;
    
protected:
	std::vector<WSqlColumn> _columns;
    std::vector<WSqlForeignKey> _foreignKeys;
    std::vector<WSqlReferencedKey> _referencedKeys;
    std::string _name;
    std::string _className;
    bool _isValid;
	Type _type;
};

} //namespace WSql

#endif // WSQLTABLE_H
