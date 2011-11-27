SET(LIBSRC sql/wsqldatabase.cpp
sql/wsqldriver.cpp
sql/wsqlerror.cpp
sql/wsqlfield.cpp
sql/wsqldatum.cpp
sql/wsqlrecord.cpp
sql/wsqlresult.cpp
sql/wsqlcolumn.cpp
sql/wsqltable.cpp
sql/wsqlforeignkey.cpp
sql/wsqlreferencedkey.cpp
#sql/wsqldabatasemanager.cpp
#sql/wsqldriverfactory.cpp
sql/drivers/wsqlitedriver.cpp
sql/drivers/wsqliteresult.cpp
sql/drivers/wmysqldriver.cpp
sql/drivers/wmysqlresult.cpp
orm/wormcodetemplate.cpp
orm/wormclassgenerator.cpp
)

#switch these next two lines to build a shared library instead ..
#add_library(worm SHARED ${LIBSRC})
add_library(worm ${LIBSRC})

target_link_libraries(worm ${CTEMPLATE_LIBRARIES} ${SQLITE3_LIBRARIES} ${MYSQL_LIBRARIES} )

add_executable(wormgen wormgen.cpp)
target_link_libraries(wormgen worm)

install(TARGETS wormgen worm 
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

