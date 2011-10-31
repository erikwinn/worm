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

#ifndef WSQLRECORD_H
#define WSQLRECORD_H

#include <string>

#include "wsqltable.h"
#include "wsqlfield.h"

namespace WSql
{

class WSqlRecord
{
    public:
        WSqlRecord();
        WSqlRecord( const WSqlRecord& other );
        WSqlRecord& operator=( const WSqlRecord& other );
        ~WSqlRecord();

        bool operator==( const WSqlRecord &other ) const;
        inline bool operator!=( const WSqlRecord &other ) const { return !operator==( other ); }
        inline std::string operator[](const std::string fieldname)const{return data(fieldname);}
        std::string operator[](const char* fieldname)const{return operator[](std::string(fieldname));}
        
        inline void append(WSqlField field){_fields.push_back(field);}
        
        WSqlField at( int pos ) const;
        WSqlField field( int pos ) const;
        WSqlField field( const std::string& colname ) const;
        std::string data( int pos ) const;
        std::string data( const std::string& colname ) const;
        bool isNull( int i ) const;
        bool isNull( const std::string& name ) const;
        int indexOf(const std::string& name)const;
        inline bool isValid()const{return _isValid;}
        inline bool empty(){return _fields.empty();}
        inline int size()const{return _fields.size();}
        inline int count()const{return _fields.size();}
        
//!\todo THIS maybe should be  const std::vector<WSqlField *>& fields() const {return _fields;};
        //JUST rushing .. fixme.
        const std::vector<WSqlField>& fields() const {return _fields;};
        
//!\todo         template <typename T> void setData( T t, int pos ) {
//             if ( pos > _fields.size() ) {
//                 return;
//             }
// 
//         };
//         template <typename T> void setData( T t, std::string colname ) {
//             if ( ! t ) {
//                 return;
//             }
// 
//         };

        void setNull( int i );
        void setNull( const std::string& name );

        void clear();
        
    private:
        bool _isValid;
        std::vector<WSqlField>::const_iterator const_cur_field_it;
        std::vector<WSqlField>::iterator field_it;

        //!\todo optimize - make this pointers .. but needs thinking so for now:
        std::vector<WSqlField> _fields;
};

} //namespace WSql

#endif // WSQLRECORD_H
