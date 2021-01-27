cmake_minimum_required ( VERSION 3.10.2 )
project ( sqlite )

set ( CMAKE_CXX_STANDARD 17 )

add_library ( sqlite "" )

add_definitions (
    "-DSQLITE_THREADSAFE=0"
    "-DSQLITE_DEFAULT_MEMSTATUS=0"
    "-DSQLITE_OMIT_DECLTYPE"
    "-DSQLITE_OMIT_PROGRESS_CALLBACK"
    "-DSQLITE_OMIT_SHARED_CACHE"
    "-DSQLITE_USE_ALLOCA"
)

target_sources ( sqlite
    PRIVATE
        sqlite3.c
)

