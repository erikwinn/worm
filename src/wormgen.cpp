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


#include "sql/wsqldatabase.h"
#include "orm/wormclassgenerator.h"
#include <iostream>
#include <getopt.h>
#include <dirent.h>

#define PACKAGE    "wormgen"
#define VERSION    "0.2"

#define DEFAULT_TPL_DIR "/usr/local/share/worm"

void print_help( int exval )
{
    std::cout << PACKAGE << " " << VERSION <<  "\n Usage:\n" << std::endl;
    std::cout << PACKAGE << " [-hVv] <-d database> \n [-t templates_dir ] [-o outputdir] [-u username] [-p password] [-s hostname] [-D driver] \n" << std::endl;
    std::cout << "  -h              print this help and exit" << std::endl;
    std::cout << "  -V              print version and exit" << std::endl;
    std::cout << "  -v              set verbose flag" << std::endl;
    std::cout << "  -d database            set database schema to use" << std::endl;
    std::cout << "  -u username            set username" << std::endl;
    std::cout << "  -p password            set password" << std::endl;
    std::cout << "  -s server              set server or hostname" << std::endl;
    std::cout << "  -o DIRECTORY           set output directory" << std::endl;
    std::cout << "  -t DIRECTORY          set templates directory" << std::endl;
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
    std::string templatesdir = "./src/orm/templates";
    DIR *dir = opendir (DEFAULT_TPL_DIR);
    if (dir != NULL){
        templatesdir = DEFAULT_TPL_DIR;
        closedir(dir);
    }
    WSql::DriverType drivertype = WSql::WMYSQL;

    while (( opt = getopt( argc, argv, "hVvf:o:u:d:D:p:s:t:" ) ) != -1 ) {
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
            case 't':
                templatesdir = optarg;
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
    
    if ( (dir = opendir(templatesdir.c_str())) == NULL)  {
        std::cout << PACKAGE << " - FATAL: Cannot find template directory " << templatesdir << "!" << std::endl;
        print_help(2);
    }
    closedir(dir);
    if ( dbname.empty() ) {
        std::cerr << PACKAGE << "FATAL: Error - no database specified! \n" << std::endl;
        print_help(3);
    }

//OK, lets go!
    WSql::WSqlDatabase db( drivertype );
    db.setDatabaseName( dbname );
    db.setUserName( username );
    db.setHostName( hostname );
    db.setPassword( password );

    if ( !db.open() ) {
        std::cerr << "FATAL: Failed to open database - error: " << db.error().text() << std::endl;
        return 4;
    }

    WSql::WormClassGenerator gen(db);
    gen.setTemplateDirectory(templatesdir);
    gen.setOutputDirectory(outputdir);
    gen.init();
    gen.run();
    
    return 0;
}
