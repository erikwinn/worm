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
#sqlite3 support (comment out if not needed):
sql/drivers/wsqlitedriver.cpp
sql/drivers/wsqliteresult.cpp
#mysql support (comment out if not needed):
sql/drivers/wmysqldriver.cpp
sql/drivers/wmysqlresult.cpp
orm/wormcodetemplate.cpp
orm/wormclassgenerator.cpp
)

# to build a shared (.so) library:
#add_library(worm SHARED ${LIBSRC})
# to build a static (.a) library:
add_library(worm ${LIBSRC})

# for sqlite3 only uncomment the following line:
#target_link_libraries(worm ${CTEMPLATE_LIBRARIES} ${SQLITE3_LIBRARIES} )
# for mysql only uncomment the following line:
#target_link_libraries(worm ${CTEMPLATE_LIBRARIES} ${MYSQL_LIBRARIES} )
# for both mysql and sqlite3 uncomment the following line:
target_link_libraries(worm ${CTEMPLATE_LIBRARIES} ${SQLITE3_LIBRARIES} ${MYSQL_LIBRARIES} )

add_executable(wormgen wormgen.cpp)
target_link_libraries(wormgen worm)

install(TARGETS wormgen worm 
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

