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


#ifndef WSQLDABATASEMANAGER_H
#define WSQLDABATASEMANAGER_H

#include <string>
#include <vector>
#include "wsqldatabase.h"
namespace WSql
{

class WSqlDabataseManager
{

    public:
        WSqlDabataseManager();
        WSqlDabataseManager( const WSqlDabataseManager& other );
        virtual ~WSqlDabataseManager();
        virtual WSqlDabataseManager& operator=( const WSqlDabataseManager& other );
        virtual bool operator==( const WSqlDabataseManager& other ) const;

        void add( WSqlDabatase &db );
        void remove( WSqlDabatase &db );
        void remove( int index );
        void remove( std::string db_name );

        WSqlDabatase& get( int index );
        WSqlDabatase& get( std::string db_name );

        static WSqlDatabase addDatabase( WSqlDriver* driver,
                                         const std::string& connectionName = defaultConnection );
        static WSqlDatabase cloneDatabase( const WSqlDatabase &other, const std::string& connectionName );
        static WSqlDatabase database( const std::string& connectionName = defaultConnection,
                                      bool open = true );
        static void removeDatabase( const std::string& connectionName );
        static bool contains( const std::string& connectionName = defaultConnection );
        static std::vector<std::string> drivers();
        static std::vector <std::string> connectionNames();


    private:

};

}//namespace WSql

#endif // WSQLDABATASEMANAGER_H
