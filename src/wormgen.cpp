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

/*! \mainpage WORM - ORM framework
 * 
 * \section intro_sec Introduction
 *
 * WORM is a Data Abstraction Layer for access to database servers and an
 * Object Relational Mapping system.  It consists of libraries providing a
 * generic API for database access and database metadata mapping of schema
 * structure. On top of this there is a tool to generate C++ class files for a
 * given arbitrary database schema called "wormgen".  wormgen accepts a few
 * arguments setting the database to use and optionally username, password,
 * output directory, etc. - "wormgen -h" shows usage. See below for more information.
 * 
 * At this writing the framework only supports SQlite3 and MySQL databases.
 * and the default output supports Wt's (Web Toolkit http://webtoolkit.eu) Dbo
 * library.  It generates header files that (with a little adjustment sometimes)
 * will work with Wt::Dbo for existing database schemata.
 * 
 * The output is configurable using the WORM template system which uses
 * google's ctemplate for output templating.
 * 
 * Additionally the libraries provide a general API for basic database access
 * including query execution with structured result sets and runtime metadata 
 * including column information such as name, data type, defaults, extras, etc.
 * 
 * NOTE: This API is not fully stable! I expect there to be some changes and
 * do not yet recommend building off the API for anything serious.  wormgen 
 * is functional (if simple) and changes there should not cause problems but 
 * the libraries are under heavy development.  There may be modifications 
 * and these should be expected until this message is removed.
 * You have been warned :).
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Requirements
 * NOTE: THIS IS UNTESTED ON WINDOWS or MAC AND NOT LIKELY TO WORK CURRENTLY.. 
 * linux or Unices should be ok.
 * 
 * \li cmake, make, gcc, libstdc++, stl
 * \li Boost - basic, header only (algorithm/string, lexical_cast)
 * \li SQlite3 libraries - you may comment out the sqlite driver in src/CMakeLists.txt
 * if you don't want SQlite3 support
 * \li libmysqlclient - same applies, you may comment this out
 * \li ctemplate 1.0; libctemplate (currently the no threads version - this can be changed in the cmake files)
 * 
 * \subsection step2 Step 2: Obtaining the sources
 *  Using git(1) clone the repository thus:
 *\code
 * git clone https://github.com/erikwinn/worm.git worm
 * \endcode
 * This will create a sources directory called "worm" - alternately you can
 * download a tarball and unpack it.
 * 
 * \subsection step3 Step 3: Building
 * \code
 * cd worm/build
 * cmake ../
 * make
 * \endcode
 * This will build the libraries and wormgen in ./build/src. By default the static
 * library is built - uncomment the shared library in src/CMakeLists.txt and
 * comment out the static library to change this (todo: add a configure script ..)
 * 
 * libraries: libworm.so libworm.a
 * binary: wormgen
 * 
 * Installation(optional):
 * \code
 * make install
 * ldconfig
 * \endcode
 * 
 * This will install the libraries to /usr/local/lib/libworm.* and the 
 * binary to /usr/local/bin/wormgen.  Code generation templates will
 * be installed to /usr/local/share/worm/
 * 
 * Uninstall:
 * \code
 * rm /usr/local/bin/wormgen
 * rm /usr/local/lib/libworm*
 * rm -rf /usr/local/share/worm
 * \endcode
 *  Alternately you can do:
 * \code
 * cat install_manifest.txt | xargs rm
 * \endcode
 * 
 * \note You can also use wormgen \em without installing by doing something like this if
 * it was built with the shared libraries:
 * (use a full path to work outside this directory):
 * \code
 * export LD_LIBRARY_PATH=./build/src
 * ./build/src/wormgen
 * \endcode
 * 
 * \section usage Using WORM
 * \subsection  codegen Generating code:
 * wormgen Usage:
 * \code
 * wormgen -h
 * \endcode
 * gives a description of the available options.
 * 
 * The database (-d) argument is the only required argument - you must have 
 * a database already set up and access to it.  By default it assumes the username
 * is root with no password - for an SQlite3 database pass the filename as the
 * argument for -d database; for example, from within the source directory you can
 * generate files from the sample Sqlite3 database thus:
 * 
 * \code
 * wormgen -D sqlite -d ./examples/blog.db
 * \endcode
 * 
 * This will generate files in the current directory - to specify a different output
 * directory use -o /path/to/output, for example:
 * 
 * \code
 * wormgen -D sqlite -d ./examples/blog.db -o /tmp
 * \endcode
 *
 * will place the generated files under /tmp
 *
 * Other commandline arguments include:
 * -D driver ([default]mysql or sqlite)
 * -t template directory (default /usr/local/share/worm/ or ./src/orm/templates)
 * -u username
 * -p password
 * -h help - use this to see all the arguments ..
 * 
 * \subsection customizing Customizing the output
 * The output can be configured by editing or creating new template files
 * in the template directory - any files in this directory are assumed to be 
 * templates to use.
 * 
 * The default template directory is /usr/local/share/worm - the option [-t DIRECTORY]
 * will tell wormgen to look in DIRECTORY for templates, eg:
 * \code
 * wormgen -D sqlite -d ./examples/blog.db -o /tmp -t $HOME/my_worm_templates/
 * \endcode
 * This means that you can copy the default template to my_worm_templates/ and
 * experiment with it or create other templates.  Each template must have a filename
 * like the one of the ones listed below - the filename will tell wormgen what kind of
 * template it is and how to name the output files for that template.
 * 
 * These are the currently supported template types:
 * 
 * \li base class header - generates FooBase.h:<br>
 * ClassDeclarationBase,               //class_declaration_base.tpl
 * \li base class source - generates FooBase.cpp:<br>
 * ClassDefinitionBase,                  //class_definition_base.tpl
 * \li class header - generates Foo.h:<br>
 * ClassDeclaration,                       //class_declaration.tpl
 * \li class source - generates Foo.cpp:<br>
 * ClassDefinition,                          //class_definition.tpl
 * 
 * Database and class naming conventions:
 * A table in the database should be named in lowercase with underscores
 * for best results (Note: this is optional but will avoid naming conflicts - wormgen
 * will still generate files for camel case naming schema). Ex: some_persons will
 * be transformed to a class SomePerson.  Columns like first_name will transform
 * to firstName.  Underscores for private variables, accessors, etc can be established
 * in the templates. Table names should be plural and will be transformed to singular.
 * 
 * Templates:
 * The templates use ctemplate and currently support only the tags in the default
 * template (which generates Wt::Dbo object headers). A decent introduction to
 * ctemplate syntax is available at the ctemplate website. The supported tags are
 * quite simple and self-explanatory - see the default template for more.
 * 
 * \section library Using the library
 * 
 * Note that the library API is still unstable and may change.
 * 
 * wormgen.cpp itself is quite simple and provides the best example of using the 
 * library - additionally, WormClassGenerator shows usage of the metadata 
 * aspects of the library. libworm can be used for basic database access, queries
 * and generic result sets - but it is currently intended primarily for the ORM
 * generator. The library does not support transactions, and all results are cached
 * locally in one call.
 * 
 * There are some small example programs under examples/ - I recommend
 * starting with these.  Also, the library code is heavily commented - if you have
 * doxygen installed the Doxyfile will generate HTML documentation
 * under doc/.  Until the API is stablelized and "real" documentation written
 * the best course is experimentation and "read the source, Luke!"
 * 
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
        std::cerr << PACKAGE << ": Error - no database specified! \n" << std::endl;
        print_help(3);
    }

//OK, lets go!
    WSql::WSqlDatabase db( drivertype );
    db.setDatabaseName( dbname );
    db.setUserName( username );
    db.setHostName( hostname );
    db.setPassword( password );

    if ( !db.open() ) {
        std::cerr << "Failed to open: " << db.error().text() << std::endl;
        return 4;
    }

    WSql::WormClassGenerator gen(db);
    gen.setTemplateDirectory(templatesdir);
    gen.setOutputDirectory(outputdir);
    gen.init();
    gen.run();
    
    return 0;
}
