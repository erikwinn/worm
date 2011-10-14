#include "../src/sql/wsqldatum.h"
#include <iostream>

int main()
{
    WSql::WSqlDatum dtm;
    double testd = 1455.096789;
    int testi = 99;
    std::string tests = "this is a string";
    dtm.setPrecision(5);
    dtm.setData(testd);
    double d = dtm.data<double>();
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(10);  
    std::cout << "value: " << d << std::endl;
//    std::cout << "type: " << WSql::WSqlDataType::toString(dtm.dataType()) << std::endl;
    
    dtm.setData(testi);
//    dtm.setDataType(WSql::WSqlDataType::INT);
    int i = dtm.data<int>();
    std::cout << "value: " << i << std::endl;
//    std::cout << "type: " << WSql::WSqlDataType::toString(dtm.dataType()) << std::endl;
    
    dtm.setData<std::string>(tests);
//    dtm.setDataType(WSql::WSqlDataType::TEXT);
    std::string s = dtm.data<std::string>();
    std::cout << "value: " << s.c_str() << std::endl;
//    std::cout << "type: " << WSql::WSqlDataType::toString(dtm.dataType()) << std::endl;
    
    return 0;
}