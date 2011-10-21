/*
 *    WORM - a DAL/ORM code generation framework
 *    Copyright (C) 2011  Erik Winn <erikwinnmail@yahoo.com>
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
#include "sql/wsqldatabase.h"
#include "sql/wsqldatatype.h"
#include <iostream>
#include <fstream>
#include <getopt.h>

#define PACKAGE    "wormgen"
#define VERSION    "0.0.1"

void print_help( int exval )
{
    std::cout << PACKAGE << " " << VERSION <<  "\n Usage:\n" << std::endl;
    std::cout << PACKAGE << " [-hVv] [-o outputdir] [-u username] [-p password] [-s hostname] [-D driver] <-d database>\n" << std::endl;
    std::cout << "  -h              print this help and exit" << std::endl;
    std::cout << "  -V              print version and exit" << std::endl;
    std::cout << "  -v              set verbose flag" << std::endl;
    std::cout << "  -d database            set datbase name" << std::endl;
    std::cout << "  -u username            set username" << std::endl;
    std::cout << "  -p password            set password" << std::endl;
    std::cout << "  -s server              set server or hostname" << std::endl;
    std::cout << "  -o DIRECTORY           set output directory" << std::endl;
    std::cout << "  -D driver              set database driver (default MYSQL)" << std::endl
    << "Note: driver may be one of: 'mysql' or 'sqlite'. " << std::endl;
    
    exit( exval );
}

WSql::DriverType getDriver(const std::string drivername)
{
    if(drivername.compare("mysql")==0)
        return WSql::WMYSQL;
    if(drivername.compare("sqlite")==0)
        return WSql::WSQLITE;
    std::cout << PACKAGE << ": Error - driver not supported:" << drivername << " \n" << std::endl;
    print_help( 1 );    
}

int main( int argc, char ** argv )
{
    if ( argc == 1 )
        print_help( 1 );
    int opt;
    bool verbose = false;
    std::string hostname = "localhost";
    std::string dbname;
    std::string username = "root";
    std::string password = "";
    std::string outputdir = ".";
    WSql::DriverType drivertype = WSql::WMYSQL;

    while (( opt = getopt( argc, argv, "hVvf:o:u:d:D:p:s:" ) ) != -1 ) {
        switch ( opt ) {
            case 'h':
                print_help( 0 );
                break;
            case 'V':
                std::cout <<  PACKAGE << " " << VERSION << std::endl;
                exit( 0 );
                break;
            case 'v':
                verbose = true;
                break;
            case 'u':
                username = optarg;
                break;
            case 'd':
                dbname = optarg;
                break;
            case 'D':
                drivertype = getDriver(std::string(optarg));
                break;
            case 'p':
                password = optarg;
                break;
            case 's':
                hostname = optarg;
                break;
            case 'o':
                outputdir = optarg;
                break;
            case ':':
                std::cout << PACKAGE << ": Error - Option " << optopt << " requires a value\n" << std::endl;
                print_help( 1 );
                break;
            default:
                std::cout << PACKAGE << ": Error - No such option:" << optopt << " \n" << std::endl;
                print_help( 1 );
        }
    }

    if ( dbname.empty() ) {
        std::cerr << PACKAGE << ": Error - no database specified! \n" << std::endl;
        print_help( 1 );
    }

    WSql::WSqlDatabase db( drivertype );
    db.setDatabaseName( dbname );
    db.setUserName( username );
    db.setHostName( hostname );
    db.setPassword( password );

    if ( !db.open() ) {
        std::cerr << "Failed to open: " << db.error().text() << std::endl;
        return 1;
    }
    //initialize all the metadata in advance to avoid query result invalidations in loops
    db.initMetaData(); 
    std::vector<std::string>tables = db.tableNames();
    if ( db.hasError() )
        std::cout << "error: " << db.error().text() << std::endl;

    std::vector<std::string>::iterator it = tables.begin();
    WSql::WSqlTable metatable;
    int numflds = 0;
    while ( it != tables.end() ) 
    {
        metatable = db.tableMetaData( *it );
        std::string code = "#include <Wt/Dbo/Dbo>\n#include <string>\n\nnamespace dbo = Wt::Dbo;\n\nclass ";
        code.append( metatable.name() + " {\n\n    public:\n" );
        numflds = metatable.count();
        for ( int i = 0; i < numflds; ++i ) 
        {
            WSql::WSqlColumn clm = metatable.column( i );
            std::string variable =  clm.columnName();
            switch ( clm.dataType() ) 
            {
                case WSql::WSqlDataType::NCHAR:
                case WSql::WSqlDataType::CHAR:
                    code.append( "\n    char " + variable + ";" );
                    break;
                case WSql::WSqlDataType::TEXT:
                case WSql::WSqlDataType::TINYTEXT:
                case WSql::WSqlDataType::LONGTEXT:
                case WSql::WSqlDataType::MEDIUMTEXT:
                case WSql::WSqlDataType::VARCHAR:
                case WSql::WSqlDataType::NVARCHAR:
                case WSql::WSqlDataType::DATE:
                case WSql::WSqlDataType::DATETIME:
                case WSql::WSqlDataType::YEAR:
                case WSql::WSqlDataType::TIME:
                case WSql::WSqlDataType::TIMESTAMP:
                case WSql::WSqlDataType::TIMESTAMPTZ:
                    code.append( "\n    std::string " + variable + ";" );
                    break;
                case WSql::WSqlDataType::TINYINT:
                    if ( clm.isUnsigned() )
                        code.append( "\n    unsigned short " + variable + ";" );
                    else
                        code.append( "\n    short " + variable + ";" );
                    break;
                case WSql::WSqlDataType::SMALLINT:
                case WSql::WSqlDataType::MEDIUMINT:
                case WSql::WSqlDataType::INT:
                    if ( clm.isUnsigned() )
                        code.append( "\n    unsigned int " + variable + ";" );
                    else
                        code.append( "\n    int " + variable + ";" );
                    break;
                case WSql::WSqlDataType::BIGINT:
                    if ( clm.isUnsigned() )
                        code.append( "\n    unsigned long " + variable + ";" );
                    else
                        code.append( "\n    long " + variable + ";" );
                    break;
                case WSql::WSqlDataType::FLOAT:
                    code.append( "\n    float " + variable + ";" );
                    break;
                case WSql::WSqlDataType::DECIMAL:
                case WSql::WSqlDataType::DOUBLE:
                    code.append( "\n    double " + variable + ";" );
                    break;
                default:
                    code.append( "\n    // unsupported " + WSql::WSqlDataType::toString( clm.dataType() )
                                 + " " + variable + ";" );
                    //TODO: handle enum and blobs
            }
        }
        code.append( "\n\n    template<class Action> void persist(Action& a)\n    {" );
        for ( int i = 0; i < numflds; ++i ) 
        {
            std::string variable =  metatable.column( i ).columnName();
            code.append( "\n        dbo::field(a, " + variable + ", \"" + variable + "\");" );
        }
        code.append( "\n    }\n};\n" );
        std::ofstream f;
        std::string fname = outputdir + "/" + metatable.name();
        fname.append( ".h" );
        f.open( fname.c_str() );
        f << code;
        f.close();
        if ( verbose )
        {
            std::cout << "Wrote : " << metatable.name() << " to " << fname << std::endl;
            std::vector<WSql::WSqlForeignKey>fkeys = metatable.foreignKeys();
            std::vector<WSql::WSqlForeignKey>::const_iterator fkit = fkeys.begin();
            for(;fkit != fkeys.end();++fkit)
            {
                std::cout << " foreign key: " << fkit->columnName() 
                    << ", keyname: " << fkit->keyName() <<  std::endl
                    << "   *  references: " << fkit->referencedColumnName() 
                    << " in " << fkit->referencedTableName() <<std::endl;
            }
        }
        it++;
    }
    return 0;
}
