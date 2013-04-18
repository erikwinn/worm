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

#ifndef WSQLDATUM_H
#define WSQLDATUM_H

#include <string>
#include <sstream>

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
			_converter << t;
			_data = _converter.str();
		};

		template <typename T> T data(){
			T result;
			_converter << _data;
			_converter >> result;
			return result;
		};

		short toShort() {return data<short>(); }
		int toInt() {return data<int>(); }
		long toLong() {return data<long>(); }
		float toFloat() {return data<float>(); }
		double toDouble() {return data<double>(); }

		unsigned short toUShort() {return data<unsigned short>(); }
		unsigned int toUInt() {return data<unsigned int>(); }
		unsigned long toULong() {return data<unsigned long>(); }

		std::string toString() {return data<std::string>(); }

		void clear();
	private:
		std::string _data;
		std::stringstream _converter;

};

}//namespace WSql
#endif // WSQLDATUM_H


