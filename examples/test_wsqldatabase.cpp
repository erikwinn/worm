#include "../src/sql/wsqldatabase.h"
#include <iostream>

int main()
{
    WSql::WSqlDatabase db(WSql::WSQLITE);
    db.setDatabaseName(std::string("blog.db"));
    if (!db.open()) {
        std::cerr << "Failed to open: " << db.error().text() << std::endl;
        return 1;
    }
    std::vector<std::string>tables = db.tableNames();
    std::vector<std::string>::iterator it = tables.begin();
    WSql::WSqlTable stbl;
    int numflds = 0;
    int col = 0;
    while (it != tables.end())
    {
        stbl = db.tableMetaData(*it++);
        numflds = stbl.count();
        std::cout << "   ======== Table name: " << stbl.name()
                  << " =========" << std::endl;
        for (col=0; col < numflds; ++col)
        {
            WSql::WSqlColumn clm = stbl.column(col);
            std::cout << "Column " << col << " = " << clm.columnName() << std::endl;
            std::cout << "  * type: " << WSql::WSqlDataType::toString(clm.dataType()) << std::endl;
            std::cout << "  * can be null:  " << (clm.canBeNull() ? "true" : "false") << std::endl;
            std::cout << "  * primary key:  " << (clm.isPrimaryKey() ? "true" : "false") << std::endl;
            std::cout << "  * default:  "  << clm.defaultValue<std::string>() << std::endl;
            std::cout << "  * Autoincrement:  "  << (clm.isAutoIncremented()?"true" : "false") << std::endl;
        }
    }
}
