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

//class_definition.tpl is currently the shortest ..
#define MIN_TPL_FILENAME_SIZE 20

#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <stdio.h>
#include <ctemplate/template.h>

#include "wormclassgenerator.h"

using namespace ctemplate;

namespace WSql
{
/** @{ */
/**
 * These are hashed string constants for refering to the marker tags used in the templates
 * Note that you may also simply use const char strings - this just helps avoid typos ..
 */
static const ::ctemplate::StaticTemplateString kcd_BELONGS_TO = STS_INIT_WITH_HASH(kcd_BELONGS_TO, "BELONGS_TO", 15166195752158467517LLU);
static const ::ctemplate::StaticTemplateString kcd_CLASS_NAME = STS_INIT_WITH_HASH(kcd_CLASS_NAME, "CLASS_NAME", 13981977283673860485LLU);
static const ::ctemplate::StaticTemplateString kcd_COLUMN_COUNT = STS_INIT_WITH_HASH(kcd_COLUMN_COUNT, "COLUMN_COUNT", 16485208626900918235LLU);
static const ::ctemplate::StaticTemplateString kcd_COLUMN_NAME = STS_INIT_WITH_HASH(kcd_COLUMN_NAME, "COLUMN_NAME", 16524890828269290931LLU);
static const ::ctemplate::StaticTemplateString kcd_COLUMNS_separator = STS_INIT_WITH_HASH(kcd_COLUMNS_separator, "COLUMNS_separator", 3248360594376546073LLU);
static const ::ctemplate::StaticTemplateString kcd_COLUMNS = STS_INIT_WITH_HASH(kcd_COLUMNS, "COLUMNS", 15302874640052016969LLU);
static const ::ctemplate::StaticTemplateString kcd_DATATYPE = STS_INIT_WITH_HASH(kcd_DATATYPE, "DATATYPE", 6518949878326190781LLU);
static const ::ctemplate::StaticTemplateString kcd_FOREIGNKEY_CLASSNAME = STS_INIT_WITH_HASH(kcd_FOREIGNKEY_CLASSNAME, "FOREIGNKEY_CLASSNAME", 14113744978891695861LLU);
static const ::ctemplate::StaticTemplateString kcd_FOREIGNKEY_CLASS_PLURAL = STS_INIT_WITH_HASH(kcd_FOREIGNKEY_CLASS_PLURAL, "FOREIGNKEY_CLASS_PLURAL", 13464148753922874173LLU);
static const ::ctemplate::StaticTemplateString kcd_FORWARD_DECLARATIONS = STS_INIT_WITH_HASH(kcd_FORWARD_DECLARATIONS, "FORWARD_DECLARATIONS", 15273852411010322531LLU);
static const ::ctemplate::StaticTemplateString kcd_HAS_MANY = STS_INIT_WITH_HASH(kcd_HAS_MANY, "HAS_MANY", 12417993436827992317LLU);
static const ::ctemplate::StaticTemplateString kcd_INCLUDES = STS_INIT_WITH_HASH(kcd_INCLUDES, "INCLUDES", 7699670683738647257LLU);
static const ::ctemplate::StaticTemplateString kcd_INCLUDE = STS_INIT_WITH_HASH(kcd_INCLUDE, "INCLUDE", 434435386609578605LLU);
static const ::ctemplate::StaticTemplateString kcd_PK_SECTION = STS_INIT_WITH_HASH(kcd_PK_SECTION, "PK_SECTION", 2633593037312264637LLU);
static const ::ctemplate::StaticTemplateString kcd_PRIMARY_KEY = STS_INIT_WITH_HASH(kcd_PRIMARY_KEY, "PRIMARY_KEY", 4310751395207134081LLU);
static const ::ctemplate::StaticTemplateString kcd_REFERENCED_CLASSNAME = STS_INIT_WITH_HASH(kcd_REFERENCED_CLASSNAME, "REFERENCED_CLASSNAME", 5376938313052943395LLU);
static const ::ctemplate::StaticTemplateString kcd_REFERENCED_TABLENAME = STS_INIT_WITH_HASH(kcd_REFERENCED_TABLENAME, "REFERENCED_TABLENAME", 14486319327059551333LLU);
static const ::ctemplate::StaticTemplateString kcd_TABLE_NAME = STS_INIT_WITH_HASH(kcd_TABLE_NAME, "TABLE_NAME", 3760310134096538793LLU);
static const ::ctemplate::StaticTemplateString kcd_UNSIGNED = STS_INIT_WITH_HASH(kcd_UNSIGNED, "UNSIGNED", 10867561526856517727LLU);
static const ::ctemplate::StaticTemplateString kcd_UNSUPPORTED = STS_INIT_WITH_HASH(kcd_UNSUPPORTED, "UNSUPPORTED", 8112833089436120679LLU);
static const ::ctemplate::StaticTemplateString kcd_VARIABLE_GETTOR = STS_INIT_WITH_HASH(kcd_VARIABLE_GETTOR, "VARIABLE_GETTOR", 4376112485907229951LLU);
static const ::ctemplate::StaticTemplateString kcd_VARIABLE_NAME = STS_INIT_WITH_HASH(kcd_VARIABLE_NAME, "VARIABLE_NAME", 5051229879184672055LLU);
static const ::ctemplate::StaticTemplateString kcd_VARIABLE_SETTOR = STS_INIT_WITH_HASH(kcd_VARIABLE_SETTOR, "VARIABLE_SETTOR", 18309610407346123363LLU);
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
 *    \ingroup WSql
 */

/*! \brief Constructs a generator with the database \a db
 */
WormClassGenerator::WormClassGenerator ( WSqlDatabase &db ) : _db ( db )
{
	usesBaseClass = false;
}
/*! \brief Initialize templates and metadata
 *
 * This method initializes the database metadata and loads the available
 * templates. It must be called (and optionally checked for success) \em before
 * run().
 * Note also that the templates are expected to end in ".tpl", worm will attempt to
 * identify the type of template according to the filename.
 *
 * On failure this sets a message in the database object and returns false.
 *
 * \retval bool - true if templates and metadata successfully initialized.
 */
bool WormClassGenerator::init()
{
	//load available templates
	DIR *dir;
	struct dirent *ent;

	dir = opendir ( _templateDirectory.c_str() );

	if ( dir != NULL )
	{
		WormCodeTemplate tpl;

		while ( ( ent = readdir ( dir ) ) != NULL )
		{
			if ( ent->d_type != DT_REG )
				continue;

			std::string entry = ent->d_name;
			size_t sz = entry.size();

			if ( sz < MIN_TPL_FILENAME_SIZE )
				continue;

			//only accept .tpl files
			std::string ext = entry.substr ( sz - 4 );

			if ( ext.compare ( ".tpl" ) )
				continue;

			tpl.setUri ( entry );
			_templates.push_back ( tpl );
			if( WormCodeTemplate::ClassDeclarationBase == tpl.type() )
				usesBaseClass = true;

			/* maybe use, pass a string to ctemplate:            std::string tmp;
			fs.open(entry.c_str());
			while(fs)
			    std::getline(fs,tmp);
			tpl.setContent(tmp);
			*/
		}

		closedir ( dir );
	}
	else
	{
		std::string msg = "Cannot open directory: " + _templateDirectory;

		if ( errno == EACCES )
			msg.append ( " - Access denied." );

		_db.addError ( WSqlError ( msg, errno, WSql::WSqlError::SYSTEM, WSql::WSqlError::FATAL ) );
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
	if ( _tablesToGenerate.empty() )
	{
		std::cerr << "No tables specified - assuming all tables .." << std::endl;
		_tablesToGenerate = _db.tableNames();
	}

	std::string outbuffer;
	std::string outfilename;
	std::vector<WormCodeTemplate>::iterator tpl_it;
	std::vector<std::string>::const_iterator tbl_it = _tablesToGenerate.begin();

	//each table translates to a class object ..
	for ( ; tbl_it != _tablesToGenerate.end(); ++tbl_it )
	{
		WSqlTable table = _db.tableMetaData ( *tbl_it );

		//foreach template, expand for this table
		for ( tpl_it = _templates.begin(); tpl_it != _templates.end(); ++ tpl_it )
		{
			const std::string tplfilename = _templateDirectory + tpl_it->uri();
			outbuffer = expand ( tplfilename, table, tpl_it->type() );
			outfilename = createOutFileName ( tpl_it->type(), table );

			if ( !writeFile ( outbuffer, outfilename ) )
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
 * \todo Break this up into smaller functions, its getting long ..
 * 
 * \param std::string filename - the file containing the template
 * \param WSqlTable table - the table being generated
 * \retval std::string an expanded template
 */
std::string WormClassGenerator::expand ( const std::string &filename, const WSqlTable &table, WormCodeTemplate::Type template_type )
{
	std::string strToReturn;
	bool has_string = false;
	bool has_vector = false;
	std::string type_declaration;
	std::string variable_name;
	std::string variable_settor;
	std::string variable_gettor;
	std::vector<std::string> forward_declarations;

	TemplateDictionary *topdict = new TemplateDictionary ( filename );
	TemplateDictionary *forwarddecls_dict = 0;
	TemplateDictionary *belongsto_dict = 0;
	TemplateDictionary *hasmany_dict = 0;
	TemplateDictionary *includes_dict = 0;
	TemplateDictionary *columns_dict = 0;
	topdict->SetValue ( kcd_CLASS_NAME, table.className() );
	topdict->SetValue ( kcd_TABLE_NAME, table.name() );
	std::string includes_string;
	
	//TODO: move this stuff to a method ..
	if( WormCodeTemplate::ClassDefinition == template_type )
	{
		includes_string = "#include \"" + table.className() + ".h\"";
		WSqlDataType::toLower(includes_string);
		includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
		includes_dict->SetValue(kcd_INCLUDE, includes_string);		
	}
	if( WormCodeTemplate::ClassDefinitionBase == template_type )
	{
		includes_string = "#include \"" + table.className() + "base.h\"";
		WSqlDataType::toLower(includes_string);
		includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
		includes_dict->SetValue(kcd_INCLUDE, includes_string);		
	}
	if( usesBaseClass  && WormCodeTemplate::ClassDeclaration == template_type )
	{
		includes_string = "#include \"" + table.className() + "base.h\"";
		WSqlDataType::toLower(includes_string);
		includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
		includes_dict->SetValue(kcd_INCLUDE, includes_string);		
	}
	
	const std::vector<WSqlColumn>& columns = table.columns();
	std::vector<WSqlColumn>::const_iterator col_it = columns.begin();

	if ( table.hasForeignKeys() )
	{
		std::vector< WSqlForeignKey >fks = table.foreignKeys();
		std::vector< WSqlForeignKey >::const_iterator fks_it = fks.begin();

		for ( ; fks_it != fks.end(); ++fks_it )
		{
			//fks_it->dump();
			std::vector<std::string>::const_iterator it = std::find ( forward_declarations.begin(), forward_declarations.end(), fks_it->referencedClassName() );

			if ( it == forward_declarations.end() ) //avoid repetition ..
			{
				forwarddecls_dict = topdict->AddSectionDictionary ( kcd_FORWARD_DECLARATIONS );
				forwarddecls_dict->SetValue ( kcd_REFERENCED_CLASSNAME, fks_it->referencedClassName() );
				forward_declarations.push_back ( fks_it->referencedClassName() );
				if ( ( ! usesBaseClass && WormCodeTemplate::ClassDefinition == template_type)
					||  WormCodeTemplate::ClassDefinitionBase == template_type )
				{
					includes_string = "#include \"" + fks_it->referencedClassName() + ".h\"";
					WSqlDataType::toLower(includes_string);
					includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
					includes_dict->SetValue(kcd_INCLUDE, includes_string);
				}
			}
			belongsto_dict = topdict->AddSectionDictionary ( kcd_BELONGS_TO );
			belongsto_dict->SetValue ( kcd_REFERENCED_CLASSNAME, fks_it->referencedClassName() );
			belongsto_dict->SetValue ( kcd_REFERENCED_TABLENAME, fks_it->referencedTableName() );
		}
	}

	if ( table.hasReferencedKeys() )
	{
		has_vector = true;
		std::vector< WSqlReferencedKey >rks = table.referencedKeys();
		std::vector< WSqlReferencedKey >::const_iterator rks_it = rks.begin();

		for ( ; rks_it != rks.end(); ++rks_it )
		{
			//rks_it->dump();
			std::vector<std::string>::const_iterator it = std::find ( forward_declarations.begin(), forward_declarations.end(), rks_it->referingClassName() );

			if ( it == forward_declarations.end() )
			{
				forwarddecls_dict = topdict->AddSectionDictionary ( kcd_FORWARD_DECLARATIONS );
				forwarddecls_dict->SetValue ( kcd_REFERENCED_CLASSNAME, rks_it->referingClassName() );
				forward_declarations.push_back ( rks_it->referingClassName() );
				if ( ( ! usesBaseClass && WormCodeTemplate::ClassDefinition == template_type)
					||  WormCodeTemplate::ClassDefinitionBase == template_type )
				{
					includes_string = "#include \"" + rks_it->referingClassName() + ".h\"";
					WSqlDataType::toLower(includes_string);
					includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
					includes_dict->SetValue(kcd_INCLUDE, includes_string);
				}
			}
			hasmany_dict = topdict->AddSectionDictionary ( kcd_HAS_MANY );
			hasmany_dict->SetValue ( kcd_FOREIGNKEY_CLASSNAME, rks_it->referingClassName() );
			hasmany_dict->SetValue ( kcd_FOREIGNKEY_CLASS_PLURAL, rks_it->referingClassNamePlural() );
		}
	}

	int i;
	char buff[8];
	
	for (i=0; col_it != columns.end(); ++col_it, i++ )
	{
		columns_dict = topdict->AddSectionDictionary ( kcd_COLUMNS );
		type_declaration = col_it->typeDeclaration();
		variable_name = col_it->variableName();
		std::string tmp = variable_name;
		tmp[0] = toupper ( tmp[0] );
		variable_settor = "set" + tmp;
		variable_gettor = "get" + tmp;

		if ( type_declaration.compare ( "std::string" ) == 0 )
			has_string = true;

		if ( ! col_it->typeIsSupported() )
			columns_dict->ShowSection ( kcd_UNSUPPORTED );
		
		if( col_it->isPrimaryKey() )
			topdict->SetValueAndShowSection("PRIMARY_KEY", variable_name, "PK_SECTION"  );
		
		columns_dict->SetValue ( kcd_COLUMN_NAME, col_it->columnName() );
		columns_dict->SetValue ( kcd_DATATYPE, type_declaration );
		columns_dict->SetValue ( kcd_VARIABLE_NAME, variable_name );
		columns_dict->SetValue ( kcd_VARIABLE_SETTOR, variable_settor );
		columns_dict->SetValue ( kcd_VARIABLE_GETTOR, variable_gettor );
		snprintf(buff, 8, "%d", i+1);
		columns_dict->SetValue("COLUMN_COUNT", buff);

		//FIXME always unsigned .. deep strangeness ..
	/*    if(col_it->isUnsigned())
			{
				coldict->ShowSection(kcd_UNSIGNED);
				std::cerr << "in table " << tbl.className() << " " << col_it->columnName() << "is unsigned .." << std::endl
				<< "        type: " << WSqlDataType::toString(col_it->type()) << std::endl;
			}    
	*/
	}
	//used for a final "QString.arg()" in cases of WHERE queries ..
	snprintf(buff, 8, "%d", i+1);
	topdict->SetValue("COLUMN_COUNT", buff);

	if(has_string)
	{
		if ( ( ! usesBaseClass && WormCodeTemplate::ClassDeclaration == template_type)
			||  WormCodeTemplate::ClassDeclarationBase == template_type )
		{
			includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
			includes_dict->SetValue(kcd_INCLUDE,  "#include <string>");				
		}
	}
	if(has_vector)
	{
		if ( ( ! usesBaseClass && WormCodeTemplate::ClassDeclaration == template_type)
			||  WormCodeTemplate::ClassDeclarationBase == template_type )
		{
			includes_dict = topdict->AddSectionDictionary(kcd_INCLUDES);
			includes_dict->SetValue(kcd_INCLUDE,  "#include <vector>");				
		}
	}

	ExpandTemplate ( filename, DO_NOT_STRIP, topdict, &strToReturn );
	delete topdict;
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
std::string WormClassGenerator::createOutFileName ( const WormCodeTemplate::Type type, const WSqlTable &table )
{
	std::string strToReturn;

	switch ( type )
	{
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
	}

	//TODO: make this optional
	WSqlDataType::toLower(strToReturn);
	return strToReturn;
}

/*! \brief Convenience function to create and write a file
 *
 * \retval bool true if successful
 */
bool WormClassGenerator::writeFile ( const std::string content, const std::string filename )
{
	std::ofstream fs;
	std::string target = _outputDirectory + "/" + filename; //!\todo; not portable FIXME
	fs.open ( target.c_str() );

	if ( !fs ) //!\todo capture error ..
		return false;

	fs << content;
	fs.close();
	return true;
}

/*!
 * Sets the output directory to \a dir - also appends directory separator if necessary.
 * \todo make portable.
 * \param std::string dir - write generated files to this directory
 */
void WormClassGenerator::setOutputDirectory ( const std::string dir )
{
	_outputDirectory = dir;

	if ( '/' != _outputDirectory[ _outputDirectory.size() - 1] )
		_outputDirectory.append ( "/" );
}
/*!
 * Sets the templates directory to \a dir - also appends directory separator if necessary.
 * \todo make portable.
 * \param std::string dir - look for templates in this directory
 */
void WormClassGenerator::setTemplateDirectory ( const std::string dir )
{
	_templateDirectory = dir;

	if ( '/' != _templateDirectory[ _templateDirectory.size() - 1] )
		_templateDirectory.append ( "/" );
}


} //namespace WSql
