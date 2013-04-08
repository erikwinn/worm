#include "../src/sql/wsqldatabase.h"
#include <iostream>

int main()
{
	WSql::WSqlDatabase db ( WSql::WMYSQL );
	db.setDatabaseName ( std::string ( "sakila" ) );
	db.setUserName ( "root" );

	if ( !db.open() )
	{
		std::cerr << "Failed to open: " << db.error().text() << std::endl;
		return 1;
	}

	std::vector<std::string>tables = db.tableNames();

	if ( db.hasError() )
		std::cout << "error: " << db.error().text() << std::endl;

	std::vector<std::string>::iterator it = tables.begin();
	WSql::WSqlTable stbl;
	int numflds = 0;
	int row = 0;

	while ( it != tables.end() )
	{
		std::vector<std::string> column_names;
		stbl = db.tableMetaData ( *it );
		numflds = stbl.count();
		std::cout << "   ======== Table name: " << stbl.name()
				  << " =========" << std::endl;

		for ( int i = 0; i < numflds; ++i )
		{
			WSql::WSqlColumn clm = stbl.column ( i );
			column_names.push_back ( clm.columnName() );

			std::cout << "Column " << i << " = " << clm.columnName() << std::endl;
			std::cout << "  * Data type: " << WSql::WSqlDataType::toString ( clm.dataType() ) << std::endl;
			std::cout << "  * Max Length:  " << clm.maxLength() << std::endl;
			std::cout << "  * Unsigned:  " << ( clm.isUnsigned() ? "true" : "false" ) << std::endl;
			std::cout << "  * Can be null:  " << ( clm.canBeNull() ? "true" : "false" ) << std::endl;
			std::cout << "  * Primary key:  " << ( clm.isPrimaryKey() ? "true" : "false" ) << std::endl;
			std::cout << "  * Autoincrement:  "  << ( clm.isAutoIncremented() ? "true" : "false" ) << std::endl;
			std::cout << "  * default value:  "  << clm.defaultValue<std::string>() << std::endl;
		}

		std::string sql = "select * from " + *it + " limit 2;";

		if ( !db.doQuery ( sql ) )
		{
			std::cout << "Query Failed: " << db.error().text() << std::endl;
			it++;
			continue;
		}
		else
		{

			WSql::WSqlResult *result = db.getResult();
			WSql::WSqlRecord record = result->fetchFirst();

			while ( !record.empty() )
			{
				std::cout << "Record Size: " << record.size() << std::endl;
				WSql::WSqlField fld;
				int pt = 0;

				for ( ; pt < record.size(); pt++ )
				{
//                    std::cout << "data for " << column_names[pt] << ": " << record[column_names[pt]] << std::endl;

					//no danger - if there is nothing there we still get an empty field object
					fld = record.field ( pt );
					std::cout << "Field " << fld.name() << ", Origin Column " << fld.columnName() << ": "
							  << fld.data<std::string>() << std::endl;
				}

				record = result->fetchNext();
			}
		}

		it++;
	}
}
