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


#ifndef WSQLDATATYPE_H
#define WSQLDATATYPE_H

#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace WSql {

namespace WSqlDataType {
    /*! \namespace WSqlDataType 
     * \brief WSqlDataType namespace - flags for supported SQL data types
     * 
     * This is a container for type flags and convenience functions for the supported
     * SQL datatypes.
     * 
     * These are the ANSI SQl standard types supported by WSQL - these will be mapped to
     * native C++ data types in ORM class generation.  For example  a TINYINT column
     * will declared as a member of type "short", a VARCHAR or TEXT as type std::string,
     * a DECIMAL to a double, etc.
     * .
     *  The following is a list of the types with specifications - Implementers of drivers should
     * use this as a guide for translating types for a particular DBMS.
     *
     * Writers of drivers should translate any proprietary or other data types specific to the
     * DBMS of the driver to these types. Most DBMS metadata is returned in a string identifier
     * of one of these types and can usually be mapped conveniently by using the functions 
     * toString(type) or toType(string).
     *  
     * See the following for more information on the specific data types:
     * \include datatypes.txt
     */
    

    /*!\enum Type - flags representing data types
     * The types currently supported - adjust this if/when new strings
     * are added.
     */
    enum Type {
        NOTYPE = 0,
        TINYINT,
        SMALLINT,
        MEDIUMINT,
        INT,
        BIGINT,
        FLOAT,
        DOUBLE,
        DECIMAL,
        DATE,
        DATETIME,
        YEAR,
        TIME,
        TIMESTAMP,
        TIMESTAMPTZ,
        CHAR,
        VARCHAR,
        NCHAR,
        NVARCHAR,
        TEXT,
        TINYTEXT,
        MEDIUMTEXT,
        LONGTEXT,
        ENUM,
        SET,
        BLOB
    };
    /*! \a TypeNames - array of strings representing data types
     * The types currently supported in handy string format - adjust this if/when new strings
     * are added.
     */
    static const char * const TypeNames[] ={
        "NOTYPE",
        "TINYINT",
        "SMALLINT",
        "MEDIUMINT",
        "INT",
        "BIGINT",
        "FLOAT",
        "DOUBLE",
        "DECIMAL",
        "DATE",
        "DATETIME",
        "TIME",
        "YEAR",
        "TIMESTAMP",
        "TIMESTAMPTZ",
        "CHAR",
        "VARCHAR",
        "NCHAR",
        "NVARCHAR",
        "TEXT",
        "TINYTEXT",
        "MEDIUMTEXT",
        "LONGTEXT",
        "ENUM",
        "SET",
        "BLOB"
    };
    
    //! Number of types supported. \note Change this if you add types!!
    static const unsigned short number_of_datatypes = 26;
    
    //! Covenience function - returns a string for the type
    static std::string toString(Type type)
    {
        std::string strToReturn;
        if (type < 0 || type > (number_of_datatypes - 1))
            strToReturn = "INVALID";
        else
            strToReturn = TypeNames[type]; //careful .. dont mess this up, add types and name in order.
        return strToReturn;
    }
    
    //! Convenience function - translates a string to a type flag
    static Type toType(std::string name)
    {
        boost::to_upper(name);
        boost::trim(name);
        //TODO: intelligence - support more type names, translate to ours ..
        int i = 0;
        for ( ; i < number_of_datatypes; ++i)
            if ( name.compare(TypeNames[i]) == 0 )
                return static_cast<Type>(i);
        return static_cast<Type>(0);//NOTYPE
    }    
} //namespace WSqlDataType
    
}// namespace WSql

#endif // WSQLDATATYPE_H
