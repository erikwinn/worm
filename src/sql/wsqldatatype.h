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

    /*! \namespace WSql::WSqlDataType 
     * \brief WSqlDataType namespace - Utilities and definitions of supported data types
     * 
     * This is a container for type flags and convenience functions for the supported
     * SQL datatypes. In this namespace are definitions for datatypes, facilities for 
     * translating these to and from other naming conventions as well as facilities for
     * generating transformations of datatypes into various strings used in the ORM
     * generating classes.
     * 
     * This provides a central utility namespace for transformations and definitions
     * related to data types as defined in DBMSs and C++, including ORM mapping
     * of table names to class names, column names and types to variable names and
     * types, etc.
     * 
     * Below is a list the ANSI SQl standard types supported by WSQL - these will be 
     * mapped to native C++ data types in ORM class generation.  For example  a 
     * TINYINT column will declared as a member of type "short", a VARCHAR or 
     * TEXT as type std::string, a DECIMAL to a double, etc. Implementers of drivers 
     * can use this as a guide for translating types for a particular DBMS.
     *
     * Writers of drivers must translate any proprietary or other data types specific to the
     * DBMS of the driver to these types. Most DBMS metadata is returned in a string identifier
     * of one of these types and can usually be mapped conveniently by using the functions 
     * toString(type) or toType(string).
     *  
     * See the following for more information on the specific data types:
     * \include datatypes.txt
     */
    
    namespace WSqlDataType {
    
    
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
    
    //! Attempt to return a singularized form of \a name
    static std::string toSingular(const std::string& name)
    {
        std::string strToReturn = name;
        size_t size = strToReturn.size();
        if(!size)
            return strToReturn;
        if('s' == strToReturn[size-1] && 's' != strToReturn[size-2])//dont fix dress, address ..
        {
            strToReturn.erase(size-1);
            if('e' == strToReturn[size-2])
            {//eg. Cities to City ..
                if( 'i' == strToReturn[size-3])
                {
                    strToReturn.erase(size-3);
                    strToReturn.append("y");
                }else if( 'h' == strToReturn[size-3])//eg. bushes .. might need fixing ..
                    strToReturn.erase(size-2);                
            }
            return strToReturn;
        }
        /*        std::string cmp = boost::to_lower_copy(strToReturn);
         *TODO        if(cmp.compare("people") .. or some such ..*/
        return strToReturn;
    }
    
    //! Attempt to return a pluralized form of \a name
    static std::string toPlural(const std::string& name)
    {
        std::string strToReturn = name;
        size_t sz = name.size();
        if(sz && 's' == strToReturn[sz - 1])
            strToReturn.append("es");
        else if(sz > 2 && 'y' == strToReturn[sz - 1]
            && 'o' != strToReturn[sz - 2]
            && 'a' != strToReturn[sz - 2])
        {
                strToReturn.erase(sz - 1);
                strToReturn.append("ies");
        }
        else
            strToReturn.append("s");
        //TODO make me a little smarter .. people, fish, sheep etc.
        return strToReturn;
    }
    
    /*! \brief Returns a suitable variable name transformed from \a columnname
     *     
     *     This translates a column name as defined in a database to a variable name.
     *     A column name should have the format "name" or "some_name", eg. "user" or 
     *     "order_id" - these will be rendered as "user" and "orderId". Note that in contrast 
     *     to tableNameToClass() the first letter is not capitalized and plural are left plural.
     *     
     *     \param std::string - columnname - the name to transform
     *     \retval std::string - a string suitable for a variable name
     */
    static std::string columnNameToVariable(const std::string& columnname)
    {
        std::string strToReturn = columnname;
        size_t pos = 0;
        pos = strToReturn.find('_');
        while(pos != std::string::npos)
        {
            strToReturn.erase(pos,1);
            if((pos + 1) < strToReturn.size())
                strToReturn[pos]= toupper(strToReturn[pos]);
            pos = strToReturn.find('_');
        }
        return strToReturn;
    }
    
    /*! \brief Returns a transformed table name as a class name
     *     
     *     This translates a table name as defined in a database to a class name.
     *     A table name should have the format "names" or "some_names", eg. "users" or 
     *     "phone_numbers" - these will be rendered as "User" and "PhoneNumber".
     *     Note that in keeping with accepted convention the table names are plural;
     *     these will also be singularized; this results in the table "orders" being 
     *     rendered as "Order" and "order_items" as "OrderItem"
     * 
     *     \note This assumes that tables are named according to convention as
     *      found also in Rails - tablenames without this convention are left as
     *      is - they will be rendered  but may cause problems with class instance 
     *     declarations in generated output.    
     * 
     *     \param std::string tablename - the name to transform
     *     \retval std::string - a string suitable for a class name
     */
    static std::string tableNameToClass(const std::string& tablename)
    {
        std::string strToReturn = toSingular(tablename);
        strToReturn[0] = toupper(strToReturn[0]);
        return columnNameToVariable(strToReturn);
    }

} //namespace WSqlDataType
    
}// namespace WSql

#endif // WSQLDATATYPE_H
