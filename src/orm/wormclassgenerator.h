/*
    WORM - a DAL/ORM code generation framework
    Copyright (C) 2011  Erik Winn <sidewalksoftware@gmail.com>

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


#ifndef WORMCLASSGENERATOR_H
#define WORMCLASSGENERATOR_H

#include <string>
#include <vector>
#include "wsqldatabase.h"
#include "wormcodetemplate.h"

namespace WSql {

class WormClassGenerator
{

    public:
        
        WormClassGenerator(WSqlDatabase& db);
        void run();
        bool init();
        
        void addTable(const std::string tablename){_tablesToGenerate.push_back(tablename);}
        void setTemplateDirectory(const std::string dir);
        void setOutputDirectory(const std::string dir);
    private:
    
        bool writeFile(const std::string content, const std::string filename);
        std::string createOutFileName(const WormCodeTemplate::Type type, const WSqlTable& table);    
        std::string expand( const std::string &filename, const WSql::WSqlTable &table, WSql::WormCodeTemplate::Type template_type );
        
        WSqlDatabase& _db;
        std::string _templateDirectory;
        std::string _outputDirectory;
        std::vector<std::string> _tablesToGenerate;
        std::vector<WormCodeTemplate> _templates;
};

} //namespace WSql

#endif // WORMCLASSGENERATOR_H
