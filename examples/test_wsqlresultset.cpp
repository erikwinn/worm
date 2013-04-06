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
    int row = 0;
    while (it != tables.end()) 
    {
        std::string sql = "select * from " + *it + " ;";
        if(!db.doQuery(sql))
        {
            std::cerr << "Query Failed: " << db.error().text() << std::endl;
            it++;
            continue;
        }else{
            std::vector<std::string> column_names;
            
            stbl = db.tableMetaData(*it);
            numflds = stbl.count();
            std::cout << "   ======== Table name: " << stbl.name() 
            << " =========" << std::endl;
            for (int i=0; i < numflds; ++i) 
            {
                WSql::WSqlColumn clm = stbl.column(i);
                column_names.push_back(clm.columnName());
                
                std::cout << "Column " << i << " = " << clm.columnName() << std::endl;
                std::cout << "  * Data type: " << WSql::WSqlDataType::toString(clm.dataType()) << std::endl;
                std::cout << "  * Can be null:  " << (clm.canBeNull() ? "true" : "false") << std::endl;
                std::cout << "  * Primary key:  " << (clm.isPrimaryKey() ? "true" : "false") << std::endl;
                std::cout << "  * Autoincrement:  "  << (clm.isAutoIncremented()?"true" : "false") << std::endl;
                std::cout << "  * default value:  "  << clm.defaultValue<std::string>() << std::endl;
            }
            WSql::WSqlResult *result = db.getResult();
            WSql::WSqlRecord record = result->fetchFirst();
            while(!record.empty())
            {
                std::cout << "Record Size: " << record.size() << std::endl;
                WSql::WSqlField fld;
                int pt = 0;
                for(;pt < column_names.size();pt++)
                {
                    std::cout << "data for " << column_names[pt] << ": " << record[column_names[pt]] << std::endl;
                    
                    //no danger - if there is nothing there we still get an empty field object 
                    fld = record.field(pt);
                    std::cout << "Field " << fld.name()<< ", Column "<< fld.columnName() << ": " 
                        << fld.data<std::string>() << std::endl;
                }
                record = result->fetchNext();
            }
        }
        it++;
    }
}
