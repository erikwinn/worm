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

#ifndef WSQLFIELD_H
#define WSQLFIELD_H

#include <string>
#include "wsqldatum.h"
#include "wsqldatatype.h"

namespace WSql
{

class WSqlField
{
    public:

        WSqlField();
        WSqlField(const std::string& fieldName, const std::string& columnName=std::string());
/*        WSqlField( const std::string& columnName = std::string(),
                   WSqlDataType::Type typeflag = WSqlDataType::NOTYPE );*/
        
        WSqlField& operator=( const WSqlField& other );
        bool operator==( const WSqlField& other ) const;
        inline bool operator!=( const WSqlField &other ) const { return !operator==( other ); }
        ~WSqlField();

        void setData( std::string s ) { _data.setData<std::string>( s );}
        void setData( short sh ) { _data.setData<short>( sh );}
        void setData( int i ) { _data.setData<int>( i );}
        void setData( long l ) { _data.setData<long>( l );}
        void setData( float f ) { _data.setData<float>( f );}
        void setData( double d ) { _data.setData<double>( d );}

        template <typename T> T data()
        {
            return _data.data<T>();
        };

        inline const WSqlDatum& rawDatum() {return _data;}

        void clear();
        void setName( const std::string& fname ) {_name = fname;}
        void setColumnName( const std::string& cname ) {_columnName = cname;}
        void setIsDirty( bool b ) { _isDirty = b;}
        void setPrecision( int i ) {_precision = i;}

        inline const bool isDirty() const {return _isDirty;}
        inline const std::string& name()const {return _name;};
        inline const std::string& columnName() const {return _columnName;};
        inline const int precision()const {return _precision;}

    private:
        int _precision;
        bool _isDirty;
        std::string _columnName;
        std::string _name;
        WSqlDatum _data;
        //TODO: - maybe add a _little metadata? .. like, length, isNull and readOnly?
        // the real question is whether to stay light or subclass WSqlColumn??

};

} //namespace WSql


#endif // WSQLFIELD_H
