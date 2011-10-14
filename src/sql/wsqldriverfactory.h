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


#ifndef WSQLDRIVERFACTORY_H
#define WSQLDRIVERFACTORY_H
namespace WSql{

class WSqlDriverFactory
{

public:
    WSqlDriverFactory();
    WSqlDriverFactory(const WSqlDriverFactory& other);
    virtual ~WSqlDriverFactory();
    virtual WSqlDriverFactory& operator=(const WSqlDriverFactory& other);
    virtual bool operator==(const WSqlDriverFactory& other) const;
	
};

}//namespace WSql

#endif // WSQLDRIVERFACTORY_H
