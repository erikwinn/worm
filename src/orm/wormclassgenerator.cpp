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

static const StaticTemplateString kcd_INCLUDES = STS_INIT_WITH_HASH(kcd_INCLUDES, "INCLUDES", 7699670683738647257LLU);
static const StaticTemplateString kcd_CLASS_NAME = STS_INIT_WITH_HASH(kcd_CLASS_NAME, "CLASS_NAME", 13981977283673860485LLU);
static const StaticTemplateString kcd_COLUMNS = STS_INIT_WITH_HASH(kcd_COLUMNS, "COLUMNS", 15302874640052016969LLU);
static const StaticTemplateString kcd_UNSUPPORTED = STS_INIT_WITH_HASH(kcd_UNSUPPORTED, "UNSUPPORTED", 8112833089436120679LLU);
static const StaticTemplateString kcd_UNSIGNED = STS_INIT_WITH_HASH(kcd_UNSIGNED, "UNSIGNED", 10867561526856517727LLU);
static const StaticTemplateString kcd_DATATYPE = STS_INIT_WITH_HASH(kcd_DATATYPE, "DATATYPE", 6518949878326190781LLU);
static const StaticTemplateString kcd_VARIABLE_NAME = STS_INIT_WITH_HASH(kcd_VARIABLE_NAME, "VARIABLE_NAME", 5051229879184672055LLU);
static const StaticTemplateString kcd_COLUMN_NAME = STS_INIT_WITH_HASH(kcd_COLUMN_NAME, "COLUMN_NAME", 16524890828269290931LLU);

namespace WSql {

    
WormClassGenerator::WormClassGenerator(WSqlDatabase& db):_db(db)
{
}
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
            if(entry.compare(".") == 0 ||entry.compare(".") == 0)
                continue;
            tpl.setUri(entry);
/* maybe use, pass a string to ctemplate:            std::string tmp;
            fs.open(entry.c_str());
            while(fs)
                std::getline(fs,tmp);
            tpl.setContent(tmp);
            _templates.push_back(tpl);*/
        }
        closedir (dir);
    } else {
        //TODO handle error ..
        return false;
    }
    _db.initMetaData();
    return true;
}
void WormClassGenerator::run()
{
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
            const std::string tplfilename = _templateDirectory + "/" + tpl_it->uri();
            outbuffer = expand(tplfilename, table);
            outfilename = createOutFileName(tpl_it->type(), table);
            if(!writeFile(outbuffer, outfilename))
                std::cerr << "Warning: failed to write File " << outfilename << std::endl;
        }
    }
}

std::string WormClassGenerator::expand( const std::string& filename, const WSql::WSqlTable& tbl
 )
{    
    std::string strToReturn;
    bool has_string=false;
    
    TemplateDictionary topdict(filename);
    topdict.SetValue(kcd_CLASS_NAME, tbl.className());
    const std::vector<WSqlColumn>& columns = tbl.columns();
    std::vector<WSqlColumn>::const_iterator col_it = columns.begin();
    for(;col_it != columns.end();++col_it)
    {
        TemplateDictionary *coldict = topdict.AddSectionDictionary(kcd_COLUMNS);
        const std::string type_declaration = col_it->typeDeclaration();
        if(type_declaration.compare("std::string"))
            has_string=true;        
        if(! col_it->typeIsSupported())
            coldict->ShowSection(kcd_UNSUPPORTED);
        coldict->SetValue(kcd_COLUMN_NAME, col_it->columnName());
        coldict->SetValue(kcd_DATATYPE, col_it->typeDeclaration());
        coldict->SetValue(kcd_VARIABLE_NAME, col_it->variableName());
    }
    if(has_string)
        topdict.SetValue(kcd_INCLUDES,"#include <string>");
    ExpandTemplate(filename,DO_NOT_STRIP,&topdict,&strToReturn);
    return strToReturn;
}

/*! \brief Return a filename for this template given \a 
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


} //namespace WSql
