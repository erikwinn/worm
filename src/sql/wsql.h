
#ifndef WSQL_H
#define WSQL_H

/*!\namespace WSql WSql namespace contains both DAL and ORM classes as well as
 * type flags and convenience functions.
 */
namespace WSql
{
	enum TableType
	{
		Tables = 0x01,
		InformationSchema = 0x02,
		Views = 0x04,
		AllTables = 0xff
	};

	enum DriverType
	{
		NONE,
		WMYSQL,
		WSQLITE,
		WPSQL,
		WMONGO,
		WBERKLEYDB //hehe ..
	};
}
#endif // WSQL_H
