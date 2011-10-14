#include "../src/sql/wsqlcolumn.h"
#include <iostream>

int main()
{
    WSql::WSqlColumn clm;
    double testd = 1455.09432;
    int testi = 99;
    std::string tests = "this is a string";
    clm.setPrecision(5);
    clm.setDefaultValue(testd);
    clm.setDataType(WSql::WSqlDataType::DOUBLE);
    double d = clm.defaultValue<double>();
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(10);  
    std::cout << "value: " << d << std::endl;
    std::cout << "type: " << WSql::WSqlDataType::toString(clm.dataType()) << std::endl;
    
    clm.setDefaultValue(testi);
    clm.setDataType(WSql::WSqlDataType::INT);
    int i = clm.defaultValue<int>();
    std::cout << "value: " << i << std::endl;
    std::cout << "type: " << WSql::WSqlDataType::toString(clm.dataType()) << std::endl;
    
    //clm.
    clm.setDefaultValue(tests);
    clm.setDataType(WSql::WSqlDataType::TEXT);
    std::string s = clm.defaultValue<std::string>();
    std::cout << "value: " << s.c_str() << std::endl;
    std::cout << "type: " << WSql::WSqlDataType::toString(clm.dataType()) << std::endl;
    
    return 0;
}