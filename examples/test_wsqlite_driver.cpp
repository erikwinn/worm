#include "../src/sql/wsqldatabase.h"
#include "../src/sql/drivers/wsqlitedriver.h"

#include <iostream>

int main()
{
    WSql::WSqlDatabase db;
    db.setDatabaseName(std::string("blog.db"));
    WSql::WSqliteDriver driver(&db);
    if (!driver.open()) {
        //      std::cerr << "nope .." << std::endl;
        //ret = driver.getError().getText();
        return 1;
    }
    std::vector<std::string>tables = driver.tableNames();
    std::vector<std::string>::iterator it = tables.begin();
    WSql::WSqlTable stbl;
    int numflds = 0;
    int row = 0;
    while (it != tables.end()) 
    {
        stbl = driver.tableMetaData(*it++);
        numflds = stbl.count();
        std::cout << "   ======== Table name: " << stbl.name() 
        << "=========" << std::endl;
        for (int i=0; i < numflds; ++i) 
        {
            WSql::WSqlColumn clm = stbl.column(i);
            std::cout << "Column " << i << " = " << clm.columnName() << std::endl;
            std::cout << "  * type: " << WSql::WSqlDataType::toString(clm.dataType()) << std::endl;
            std::cout << "  * can be null:  " << (clm.canBeNull() ? "true" : "false") << std::endl;
            std::cout << "  * default:  "  << clm.defaultValue<std::string>() << std::endl;
        }
    }
}
