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


#include "wormcodetemplate.h"

namespace WSql
{

WormCodeTemplate::WormCodeTemplate()
{
	_type = Unknown;
}

WormCodeTemplate::WormCodeTemplate ( const WormCodeTemplate &other )
{
	_type = other._type;
	_content = other._content;
	_uri = other._uri;
}

WormCodeTemplate::~WormCodeTemplate()
{

}

WormCodeTemplate &WormCodeTemplate::operator= ( const WormCodeTemplate &other )
{
	_type = other._type;
	_content = other._content;
	_uri = other._uri;
	return *this;
}

bool WormCodeTemplate::operator== ( const WormCodeTemplate &other ) const
{
	return ( _type == other._type
			 && _content.compare ( other._content ) == 0
			 && _uri.compare ( other._uri ) == 0
		   );
}
/*! \brief Set the URI or filename containing the template
 *
 * This sets the URI for the template - it also sets the Type of this template
 * from the filename.
 *
 * \param std::string uri - the location of the template file
 */
void WormCodeTemplate::setUri ( const std::string &uri )
{
	if ( uri.empty() )
		return;

	_uri = uri;
	std::string tmp;
	size_t pos = uri.rfind ( '/' ); //!\todo NOT PORTABLE, FIXME

	if ( std::string::npos == pos )
	{
		pos = uri.rfind ( '\\' );

		if ( std::string::npos == pos )
			tmp = uri;
	}
	else
		tmp = uri.substr ( pos + 1 );

	if ( tmp.compare ( "class_declaration.tpl" ) == 0 )
		_type = ClassDeclaration;
	else
		if ( tmp.compare ( "class_declaration_base.tpl" ) == 0 )
			_type = ClassDeclarationBase;
		else
			if ( tmp.compare ( "class_definition.tpl" ) == 0 )
				_type = ClassDefinition;
			else
				if ( tmp.compare ( "class_definition_base.tpl" ) == 0 )
					_type = ClassDefinitionBase;
				else
					if ( tmp.compare ( "edit_view_declaration.tpl" ) == 0 )
						_type = EditViewDeclaration;
					else
						if ( tmp.compare ( "edit_view_declaration_base.tpl" ) == 0 )
							_type = EditViewDeclarationBase;
						else
							if ( tmp.compare ( "edit_view_definition.tpl" ) == 0 )
								_type = EditViewDefinition;
							else
								if ( tmp.compare ( "edit_view_definition_base.tpl" ) == 0 )
									_type = EditViewDefinitionBase;
								else
									if ( tmp.compare ( "list_view_declaration.tpl" ) == 0 )
										_type = ListViewDeclaration;
									else
										if ( tmp.compare ( "list_view_declaration_base.tpl" ) == 0 )
											_type = ListViewDeclarationBase;
										else
											if ( tmp.compare ( "list_view_definition.tpl" ) == 0 )
												_type = ListViewDefinition;
											else
												if ( tmp.compare ( "list_view_definition_base.tpl" ) == 0 )
													_type = ListViewDefinitionBase;
												else
													_type = Unknown;
}

} //namespace WSql
