/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Erik Winn <email>

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

#include "wormclassgenerator.h"
#include <ctemplate/template.h>
#include <dirent.h>
#include <fstream>

using namespace ctemplate;

namespace WSql {
    /** @name ctemplate strings */
    /** @{ */
    /**
     * These are hashed string constants for refering to the marker tags used in the templates
     */
    static const ::ctemplate::StaticTemplateString kcd_INCLUDES = STS_INIT_WITH_HASH(kcd_INCLUDES, "INCLUDES", 7699670683738647257LLU);
    static const ::ctemplate::StaticTemplateString kcd_CLASS_NAME = STS_INIT_WITH_HASH(kcd_CLASS_NAME, "CLASS_NAME", 13981977283673860485LLU);
    static const ::ctemplate::StaticTemplateString kcd_COLUMNS = STS_INIT_WITH_HASH(kcd_COLUMNS, "COLUMNS", 15302874640052016969LLU);
    static const ::ctemplate::StaticTemplateString kcd_UNSUPPORTED = STS_INIT_WITH_HASH(kcd_UNSUPPORTED, "UNSUPPORTED", 8112833089436120679LLU);
    static const ::ctemplate::StaticTemplateString kcd_UNSIGNED = STS_INIT_WITH_HASH(kcd_UNSIGNED, "UNSIGNED", 10867561526856517727LLU);
    static const ::ctemplate::StaticTemplateString kcd_DATATYPE = STS_INIT_WITH_HASH(kcd_DATATYPE, "DATATYPE", 6518949878326190781LLU);
    static const ::ctemplate::StaticTemplateString kcd_VARIABLE_NAME = STS_INIT_WITH_HASH(kcd_VARIABLE_NAME, "VARIABLE_NAME", 5051229879184672055LLU);
    static const ::ctemplate::StaticTemplateString kcd_COLUMN_NAME = STS_INIT_WITH_HASH(kcd_COLUMN_NAME, "COLUMN_NAME", 16524890828269290931LLU);
    /** @} */
    
/*! \class WormClassGenerator  An ORM generator class
 * \brief The main class in the ORM generator
 * 
 * This class is the central class in the ORM generator. It requires a valid WSqlDatabase object
 * and is used in this manner:
 * \code 
 *
 *    WSql::WSqlDatabase db( drivertype );
 *    db.setDatabaseName( dbname );
 *    db.setUserName( username );
 *    db.setHostName( hostname );
 *    db.setPassword( password );
 * 
 *    if ( !db.open() ) {
 *      some_error_func();
 *    }
 * 
 *     WSql::WormClassGenerator gen(db);
 *     gen.setTemplateDirectory(templatesdir);
 *     gen.setOutputDirectory(outputdir);
 *     if(!gen.init())
 *          some_error_func();
 *     else
 *          gen.run();
 * 
 * \endcode
 *  
 */

/*! \brief Constructs a generator with the database \a db
 */
WormClassGenerator::WormClassGenerator(WSqlDatabase& db):_db(db)
{
}
/*! \brief Initialize templates and metadata
 * 
 * This method initializes the database metadata and loads the available
 * templates. It must be called (and optionally checked for success) \em before
 * run().
 * 
 * \retval bool - true if templates and metadata successfully initialized.
 */
bool WormClassGenerator::init()
{
    //load available templates
    DIR *dir;
    struct dirent *ent;
        
    dir = opendir (_templateDirectory.c_str());
    if (dir != NULL) 
    {
        WormCodeTemplate tpl;
        //std::ifstream fs;
        while ((ent = readdir (dir)) != NULL) 
        {
            std::string entry = ent->d_name;
            if('~' == entry[entry.size()-1] || entry.compare("..") == 0 ||entry.compare(".") == 0)
                continue;
            tpl.setUri(entry);
            _templates.push_back(tpl);
            /* maybe use, pass a string to ctemplate:            std::string tmp;
            fs.open(entry.c_str());
            while(fs)
                std::getline(fs,tmp);
            tpl.setContent(tmp);
            */
        }
        closedir (dir);
    } else {
        //TODO handle error ..
        return false;
    }
    _db.initMetaData();
    return true;
}
/*! \brief Run the generator
 * 
 * This method iterates through the available templates for each configured
 * table, calls expand() and writes the results to a class file. It is the principle
 * control function for WormClassGenerator.
 * 
 * If no tables have been added it assumes that all tables in the database are
 * to be generated.
 * 
 * \sa addTable() setTemplateDirectory() setOutputDirectory() 
 */
void WormClassGenerator::run()
{
    if(_tablesToGenerate.empty())
    {
        std::cerr << "No tables specified - assuming all tables .." << std::endl;
        _tablesToGenerate = _db.tableNames();
    }
    
    std::string outbuffer;
    std::string outfilename;
    std::vector<WormCodeTemplate>::iterator tpl_it;
    std::vector<std::string>::const_iterator tbl_it = _tablesToGenerate.begin();
    //each table translates to a class object ..
    for(; tbl_it != _tablesToGenerate.end();++tbl_it)
    {
        WSqlTable table = _db.tableMetaData(*tbl_it);
        //foreach template, expand for this table
        for(tpl_it = _templates.begin(); tpl_it != _templates.end();++ tpl_it)
        {
            const std::string tplfilename = _templateDirectory + tpl_it->uri();
            outbuffer = expand(tplfilename, table);
            outfilename = createOutFileName(tpl_it->type(), table);
            if(!writeFile(outbuffer, outfilename))
                std::cerr << "Warning: failed to write File " << outfilename << std::endl;
        }
    }
}

/*! \brief Expands the template in \a filename for \a table
 * 
 * This initializes a dictionary with the values for each column in table mapped to the 
 * marker tags used in the template \a filename (Note: see top of file for supported tags)
 * It then expands the template replacing the tag markers and returns a string suitable for
 * writing to a file.
 * 
 * \param std::string filename - the file containing the template
 * \param WSqlTable table - the table being generated
 * \retval std::string an expanded template
 */
std::string WormClassGenerator::expand( const std::string& filename, const WSqlTable& table)
{    
    std::string strToReturn;
    bool has_string=false;
    
    TemplateDictionary topdict(filename);
    topdict.SetValue(kcd_CLASS_NAME, table.className());
    const std::vector<WSqlColumn>& columns = table.columns();
    std::vector<WSqlColumn>::const_iterator col_it = columns.begin();
    for(;col_it != columns.end();++col_it)
    {
        TemplateDictionary *coldict = topdict.AddSectionDictionary(kcd_COLUMNS);
        const std::string type_declaration = col_it->typeDeclaration();
        if(type_declaration.compare("std::string") == 0)
            has_string=true;
        if(! col_it->typeIsSupported())
            coldict->ShowSection(kcd_UNSUPPORTED);
//FIXME always unsigned .. deep strangeness ..        
/*      if(col_it->isUnsigned())
        {
            coldict->ShowSection(kcd_UNSIGNED);
            std::cerr << "in table " << tbl.className() << " " << col_it->columnName() << "is unsigned .." << std::endl
            << "        type: " << WSqlDataType::toString(col_it->type()) << std::endl; 
        }    */
        coldict->SetValue(kcd_COLUMN_NAME, col_it->columnName());
        coldict->SetValue(kcd_DATATYPE, col_it->typeDeclaration());
        coldict->SetValue(kcd_VARIABLE_NAME, col_it->variableName());
    }
    
    //TODO: stub - add a addInclude or such .. currently we only support string
    if(has_string)
        topdict.SetValueAndShowSection("INCLUDE","#include <string>", kcd_INCLUDES);
    
    ExpandTemplate(filename, DO_NOT_STRIP, &topdict, &strToReturn);
    return strToReturn;
}

/*! \brief Return a filename for the template type \a type for table \a table
 * 
 * This creates an output file name for a table by template type. For example, if the template
 * file was class_declaration.tpl, this will be interpreted as a template of type ClassDeclaration
 * and the file to write will be named using the class for the table: MyClass.h
 * 
 * \param WormCodeTemplate::Type  type - the type of template
 * \param WSqlTable table - the table for which the class is generated.
 * \retval std::string - a suitable file name
 */
std::string WormClassGenerator::createOutFileName(const WormCodeTemplate::Type type, const WSqlTable& table)
{
    std::string strToReturn;
    switch(type){
        case WormCodeTemplate::ClassDeclarationBase: 
            strToReturn = table.className() +  "Base.h";
            break;
        case WormCodeTemplate::ClassDefinitionBase:
            strToReturn = table.className() +  "Base.cpp";
            break;
        case WormCodeTemplate::ClassDeclaration: 
            strToReturn = table.className() +  ".h";
            break;
        case WormCodeTemplate::ClassDefinition: 
            strToReturn = table.className() +  ".cpp";
            break;
        default:
            std::cerr << "WormClassGenerator: WARNING: template type unsupported!";
            ;
    }
    return strToReturn;
}

/*! \brief Convenience function to create and write a file
 * 
 * \retval bool true if successful
 */
bool WormClassGenerator::writeFile( const std::string content, const std::string filename )
{
    std::ofstream fs;
    std::string target = _outputDirectory + "/" + filename; //TODO; not portable FIXME
    fs.open(target.c_str());
    if(!fs)//TODO capture error ..
        return false;
    fs << content;
    fs.close();
    return true;
}

/*! 
 * Sets the output directory to \a dir - also appends directory separator if necessary.
 * TODO: make portable.
 * \param std::string dir - write generated files to this directory
 */
void WormClassGenerator::setOutputDirectory( const std::string dir )
{
    _outputDirectory = dir;
    if('/' != _outputDirectory[ _outputDirectory.size()-1])
        _outputDirectory.append("/");
}
/*! 
 * Sets the templates directory to \a dir - also appends directory separator if necessary.
 * TODO: make portable.
 * \param std::string dir - look for templates in this directory
 */
void WormClassGenerator::setTemplateDirectory( const std::string dir )
{
    _templateDirectory = dir;
    if('/' != _templateDirectory[ _templateDirectory.size()-1])
        _templateDirectory.append("/");
}


} //namespace WSql
