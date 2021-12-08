cmake_minimum_required ( VERSION 3.10.2 )
project ( padamose )

set ( CMAKE_CXX_STANDARD 17 )

add_subdirectory ( 3rd-party/googletest EXCLUDE_FROM_ALL )
add_subdirectory ( 3rd-party/hiredis EXCLUDE_FROM_ALL )
add_subdirectory ( 3rd-party/lognosis EXCLUDE_FROM_ALL )
add_subdirectory ( 3rd-party/sqlite-3.34.0 EXCLUDE_FROM_ALL )

add_library ( padamose "" )

add_dependencies ( padamose
    hiredis
    lognosis
)

target_include_directories ( padamose
    PRIVATE 
        src/
        3rd-party/hiredis/
        3rd-party/lognosis/src/
        3rd-party/lognosis/3rd-party/
        3rd-party/sqlite-3.34.0/

        # neither of these *should* be necessary; not going to troubleshoot it now
        /usr/local/include/
        /usr/include/
)

target_sources ( padamose
    PRIVATE
        src/padamose/AbstractHasVersionedBranch.cpp
        src/padamose/AbstractPersistenceProvider.cpp
        src/padamose/AbstractPersistentVersionedBranch.cpp
        src/padamose/AbstractStringStore.cpp
        src/padamose/AbstractVersionedBranch.cpp
        src/padamose/AbstractVersionedBranchClient.cpp
        src/padamose/AbstractVersionedCollection.cpp
        src/padamose/AbstractVersionedStoreTag.cpp
        src/padamose/BaseVersionedCollectionIterator.cpp
        src/padamose/BranchInspector.cpp
        src/padamose/DebugStringStore.cpp
        src/padamose/EphemeralValueStack.cpp
        src/padamose/EphemeralVersionedBranch.cpp
        src/padamose/MutableVersionedCollection.cpp
        src/padamose/RedisStringStore.cpp
        src/padamose/RocksDbStringStore.cpp
        src/padamose/SQLite.cpp
        src/padamose/SQLitePersistenceProvider.cpp
        src/padamose/SQLiteStringStore.cpp
        src/padamose/SQLiteVersionedBranch.cpp
        src/padamose/StringStoreVersionedBranch.cpp
        src/padamose/Variant.cpp
        src/padamose/VersionedCollectionSnapshot.cpp
        src/padamose/VersionedList.cpp
        src/padamose/VersionedMap.cpp
        src/padamose/VersionedStoreIterator.cpp
        src/padamose/VersionedStoreLock.cpp
)
