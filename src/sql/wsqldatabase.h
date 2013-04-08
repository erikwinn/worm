/*
 *    WORM - a DAL/ORM code generation framework
 *    Copyright (C) 2011  Erik Winn <sidewalksoftware@gmail.com>
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

#ifndef WSQLDATABASE_H
#define WSQLDATABASE_H

#include <string>
#include <vector>
#include "wsql.h"
#include "wsqlerror.h"
#include "wsqltable.h"
#include "wsqldriver.h"

namespace WSql {

class WSqlResult;
class WSqlDriver;

class WSqlDatabase
{

public:

    WSqlDatabase();
    WSqlDatabase(const WSql::DriverType &type);
    WSqlDatabase(const WSqlDatabase &other);

    ~WSqlDatabase();

    WSqlDatabase &operator=(const WSqlDatabase &other);

    // Connecting and errors ..
    bool open();
    bool open(const std::string& username, const std::string& password);
    void close();
    bool isOpen() const;
    bool hasError() const;
    WSqlError error();
    std::vector<std::string> errors();
    bool isValid() const;

    //General connection configurations ..
    std::string databaseName() const;
    std::string userName() const;
    std::string password() const;
    std::string hostName() const;
    int port() const {return _port;}
    std::string connectionOptions() const;
    WSql::DriverType driverType() {return _driverType;}

    void setDatabaseName(const std::string& name);
    void setUserName(const std::string& name);
    void setPassword(const std::string& password);
    void setHostName(const std::string& host);
    void setPort(int p);
    void setConnectOptions(const std::string& options = std::string());
    void setDriverType(WSql::DriverType t);

    void addError(const WSqlError& e) {_errorStack.push_back(e);}
    //REMOVE:?
    void setDriver(WSqlDriver *d) {_driver=d;}

    //Raw driver access ..
    WSqlDriver* driver() const;
    WSqlDriver* handle() const {return driver();}

    //Metadata
    const std::vector<std::string>& tableNames(WSql::TableType type = WSql::Tables);
    WSqlTable tableMetaData( const std::string &tableName ) const;
    void initMetaData();

    //Query interaction - wrapper around driver ..
    bool doQuery(const std::string& sql );
//    bool execute(const std::string& sql );
    WSqlResult* getResult(bool iscached=true);
    bool initDriver();

private:
    void init();

    bool _isValid;
    WSql::DriverType _driverType;
    WSqlDriver* _driver;
    std::string _databaseName;
    std::string _userName;
    std::string _password;
    std::string _hostname;
    std::string _connectionOptions;
    int _port;

    std::vector<std::string> _tableNames;
    std::vector<WSqlError> _errorStack;
};

} //namespace WSql

#endif // WSQLDATABASE_H
