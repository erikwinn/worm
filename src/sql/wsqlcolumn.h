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


#ifndef WSQLCOLUMN_H
#define WSQLCOLUMN_H


#include <string>

#include "wsqldatatype.h"
#include "wsqldatum.h"

namespace WSql{

class WSqlColumn
{

    public:
        WSqlColumn();
        WSqlColumn( const WSqlColumn& other );
        virtual ~WSqlColumn();
        virtual WSqlColumn& operator=( const WSqlColumn& other );
        virtual bool operator==( const WSqlColumn& other ) const;
        inline bool operator!=(const WSqlColumn &other) const { return !operator==(other); }
        
        template <typename T> void setDefaultValue( const T t)
        {
            _default.setData(t);
        };
        template <typename T> T defaultValue()
        {
            return _default.data<T>();
        };
        
        void setVariableName( const std::string& name );
        void setColumnName( const std::string& name );
        void setMaxLength( int length ){_maxLength=length;}
        void setPrecision( int precision );
        void setDataType( WSqlDataType::Type type );
        void setIsAutoIncremented( bool b ) { _isAutoIncremented = b; }
        void setCanBeNull( bool b ) {_canBeNull = b;}
        void setIsPrimaryKey( bool b ) {_isPrimaryKey = b;}
        void setIsUnsigned(bool b){_isUnsigned=b;}
        
        const std::string& columnName() const;
        const std::string& variableName() const;
        std::string typeDeclaration()const;
        const int maxLength() const;
        const int precision() const;
        const WSqlDataType::Type type() const;
        bool isAutoIncremented() const {return _isAutoIncremented; }
        bool canBeNull() const;
        bool isPrimaryKey() const {return _isPrimaryKey;}
        bool isUnsigned() const {return _isUnsigned;}
        
        bool typeIsSupported()const;

    private:
        
        bool _canBeNull;
        bool _isAutoIncremented;
        bool _isPrimaryKey;
        bool _isUnsigned;
        int _maxLength;
        int _precision; 
        std::string _columnName;
        std::string _variableName;
        WSqlDatum _default;
        WSqlDataType::Type _type;
};

}//namespace WSql
#endif // WSQLCOLUMN_H
