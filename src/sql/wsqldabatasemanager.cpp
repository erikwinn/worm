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


#include "wsqldabatasemanager.h"
namespace WSql{

WSqlDabataseManager::WSqlDabataseManager()
{

}

WSqlDabataseManager::WSqlDabataseManager(const WSqlDabataseManager& other)
{

}

WSqlDabataseManager::~WSqlDabataseManager()
{

}

WSqlDabataseManager& WSqlDabataseManager::operator=(const WSqlDabataseManager& other)
{
    return *this;
}

bool WSqlDabataseManager::operator==(const WSqlDabataseManager& other) const
{
///!\todo return ...;
}

}//namespace WSql
