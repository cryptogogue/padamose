cmake_minimum_required ( VERSION 3.10.2 )
project ( sqlite )

set ( CMAKE_CXX_STANDARD 17 )

add_library ( sqlite "" )

target_sources ( sqlite
    PRIVATE
        sqlite3.c
)

