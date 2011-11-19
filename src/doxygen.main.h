//This is documentation, not code - the main page in doxygen output ..

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
\note You can also use wormgen without installing by doing something like this if
it was built with the shared libraries:
(use a full path to work outside this directory):
 * \code
    export LD_LIBRARY_PATH=./build/src
    ./build/src/wormgen -h
 * \endcode
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
 * in the template directory - any files in this directory named with the extension
 * ".tpl" are assumed to be templates to use. Note that there is a naming convention
 * for each type of template - see the list of supported template types below.
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
 * wormgen.cpp itself is quite simple and provides a quick example of using the
 * library - additionally, WormClassGenerator shows usage of the metadata
 * aspects of the library. libworm can also be used for basic database access, queries
 * and generic result sets - but it is currently intended primarily for the ORM
 * generator. The library does not support transactions, and all results are cached
 * locally in one call.
 *
 * There are also some small example programs under examples/ - I recommend
 * starting with these.  Also, the library code is heavily commented -and there is
 * HTML documentation under doc/html.  Until the API is stablized and "real"
 * documentation written the best course is experimentation and "reading the
 * source, Luke!" - the DAL functions available are documented in comments but
 * not yet stable so your mileage may vary.
 *
 */
