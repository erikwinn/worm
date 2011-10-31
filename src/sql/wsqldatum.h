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

#ifndef WSQLDATUM_H
#define WSQLDATUM_H

#include <string>
#include <boost/lexical_cast.hpp>

namespace WSql
{

class WSqlDatum
{

    public:
        WSqlDatum();
        WSqlDatum( const WSqlDatum& other );
        virtual ~WSqlDatum();
        virtual WSqlDatum& operator=( const WSqlDatum& other );
        virtual bool operator==( const WSqlDatum& other ) const;
        inline bool operator!=( const WSqlDatum &other ) const {
            return !operator==( other );
        }
        template <typename T> void setData( const T t ) {
            try {
                _data = boost::lexical_cast<std::string>( t );
            }
            catch ( boost::bad_lexical_cast &e ) {
                //!\todo handle exception
            }
        };

        template <typename T> T data()const {
            try {
                return boost::lexical_cast<T>( _data );
            }
            catch ( boost::bad_lexical_cast &e ) {
                //!\todo handle exception
                //well, gotta do _something ..
                return T();
            }
        };

        short toShort()const {return data<short>(); }
        int toInt()const {return data<int>(); }
        long toLong()const {return data<long>(); }
        float toFloat()const {return data<float>(); }
        double toDouble()const {return data<double>(); }

        unsigned short toUShort()const {return data<unsigned short>(); }
        unsigned int toUInt()const {return data<unsigned int>(); }
        unsigned long toULong()const {return data<unsigned long>(); }

        std::string toString()const {return data<std::string>(); }

        void clear();
    private:
        std::string _data;

};

}//namespace WSql
#endif // WSQLDATUM_H

