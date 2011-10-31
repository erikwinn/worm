/*
 *    WORM - a DAL/ORM code generation framework
 *    Copyright (C) 2011  Erik Winn <erikwinnmail@yahoo.com>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WSQLRESULT_H
#define WSQLRESULT_H

#include <vector>
#include "wsql.h"
#include "wsqlrecord.h"

namespace WSql {

class WSqlDriver;

class WSqlResult
{

public:
	WSqlResult(const WSqlDriver *driver);
    WSqlResult( const WSql::WSqlResult& other);
    virtual ~WSqlResult();
    virtual WSqlResult& operator=(const WSqlResult& other);
//!\todo     virtual bool operator==(const WSqlResult& other) const;
//     inline bool operator!=( const WSqlResult &other ) const { return !operator==( other ); }

	void addRecord(WSqlRecord& r){_records.push_back(r);}
	void setIsCached(bool b){ _isCached = b; }	
	void setIsValid(bool b){_isValid=b;}
	
	bool isValid()const{return _isValid;}
	bool isCached()const{return _isCached; }
	
	virtual int size()const {return _records.size(); }
    virtual int count()const {return size(); }
    virtual bool empty()const{ return _records.empty();}
    virtual bool hasOutValues() const { return !empty();}
    
    virtual void clear(){ _records.clear(); }
    
    virtual WSqlRecord current();
    virtual WSqlRecord fetch(int i = -1);
    virtual WSqlRecord fetchFirst();
    virtual WSqlRecord fetchLast();
    virtual WSqlRecord fetchNext();
    virtual WSqlRecord fetchPrevious();

protected:
    virtual bool seek(int i, bool relative = false);
    virtual bool next();
    virtual bool previous();
    virtual bool first();
    virtual bool last();
    
    std::vector<WSqlRecord>::const_iterator const_cur_record_it;
    std::vector<WSqlRecord>::iterator record_it;
    const WSqlDriver *_driver;
	std::vector<WSqlRecord> _records;
    bool _isCached;
    private:
    bool _isValid;
};

} //namespace WSql

#endif // WSQLRESULT_H
