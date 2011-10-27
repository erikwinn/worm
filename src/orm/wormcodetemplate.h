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


#ifndef WORMCODETEMPLATE_H
#define WORMCODETEMPLATE_H
#include <string>

namespace WSql {
    
class WormCodeTemplate
{
    
    public:
        enum Type{
            Unknown,
            ClassDeclarationBase,               //class_declaration_base.tpl
            ClassDefinitionBase,                  //class_definition_base.tpl
            ClassDeclaration,                       //class_declaration.tpl
            ClassDefinition,                          //class_definition.tpl
            EditViewDeclarationBase,       //edit_view_declaration_base.tpl
            EditViewDeclaration,                //edit_view_declaration.tpl
            EditViewDefinitionBase,          //edit_view_definition_base.tpl
            EditViewDefinition,                  //edit_view_definition.tpl
            ListViewDeclarationBase,       //list_view_declaration_base.tpl
            ListViewDeclaration,               //list_view_declaration.tpl
            ListViewDefinitionBase,          //list_view_definition_base.tpl
            ListViewDefinition                   //list_view_definition.tpl
        };
        
        WormCodeTemplate();
        WormCodeTemplate( const WormCodeTemplate& other );
        virtual ~WormCodeTemplate();
        virtual WormCodeTemplate& operator=( const WormCodeTemplate& other );
        virtual bool operator==( const WormCodeTemplate& other ) const;

        const Type& type()const{return _type;}
        const std::string& uri(){return _uri;};
        const std::string& content()const{return _content;}
        void setContent(const std::string content){_content=content;}
        void setType(Type t){_type=t;}
        void setUri(const std::string& uri);
        
    private:
        Type _type;
        std::string _uri; //AKA, filename for current purposes ..
        std::string _content;
};

} //namespace WSql

#endif // WORMCODETEMPLATE_H
