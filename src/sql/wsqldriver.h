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

#ifndef WSQLDRIVER_H
#define WSQLDRIVER_H

#include <string>
#include "wsqldatabase.h"
#include "wsqlerror.h"
#include "wsqlresult.h"

namespace WSql
{

class WSqlTable;
class WSqlDatabase;

class WSqlDriver
{

    public:
        WSqlDriver( WSqlDatabase *db );
        virtual ~WSqlDriver();

        virtual bool open() = 0;
        virtual void close() = 0;
        virtual bool doQuery(std::string sql) = 0;
        //virtual bool execute(std::string sql) = 0;
        // virtual WSqlResult exec(const WSqlQuery &queryObject) = 0;
        virtual std::vector<std::string> tableNames() = 0;
        virtual WSqlTable tableMetaData( const std::string &tableName ) = 0;
                
        virtual WSqlResult* getResult(bool iscached=true){return _result;}
        virtual WSqlError error() const { return _error;}
        virtual bool isValid()const { return _isValid;}
        virtual bool isOpen()const { return _isOpen;}
        
        WSqlTable findTable( std::string tablename )const;
        
        bool hasError()const { return _hasError;}

//note: maybe auto disconnect/reconnect here?  or, do we even need this?              
        void setDatabase( WSqlDatabase *dp ) { _database = dp;}

    protected:
        friend class WSqlDatabase;

        WSqlTable* getTable( const std::string& tablename );
        
        void setIsValid( bool b ) { _isValid = b;}
        void setIsOpen( bool o ) { _isOpen = o;}
        void setHasError( bool e ) { _hasError = e;}
        
        void setError(const WSqlError& e );
        void setError(const std::string& text,
                      WSqlError::ErrorType type,
                      WSqlError::ErrorSeverity severity,
                      bool isvalid = true );
        inline void setError(const std::string& text){
            setError(text,WSql::WSqlError::DRIVER, WSql::WSqlError::WARNING);
        }
        inline void setError(const char * text){setError(std::string(text));}
        
        WSqlDatabase *_database;
        WSqlResult * _result;
        std::vector<WSqlTable> _tables;
        
    private:
        bool _isValid;
        bool _isOpen;
        bool _hasError;
        std::string _databaseName;
        WSqlError _error;
        std::vector<WSqlError> _errors;
};

} // namespace WSql

#endif // WSQLDRIVER_H
