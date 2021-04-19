// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/AbstractStringStore.h>
#include <padamose/EphemeralVersionedBranch.h>
#include <padamose/SQLitePersistenceProvider.h>
#include <padamose/SQLiteVersionedBranch.h>
#include <padamose/VersionedStoreTag.h>

namespace Padamose {

//================================================================//
// SQLiteVersionedBranch
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::deleteBranch () {

    this->begin ();

    SQLite& db = this->getDB ();

    SQLiteResult result = db.exec (
        
        "DELETE FROM branches WHERE branchID IS ?1",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, this->mBranchID );
        }
    );
    result.reportWithAssert ();
    
    result = db.exec (
        
        "DELETE FROM tuples WHERE branchID IS ?1",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, this->mBranchID );
        }
    );
    result.reportWithAssert ();
    
    this->commit ();
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLite& SQLiteVersionedBranch::getDB () const {

    assert ( this->mProvider );
    assert ( this->mProvider->mDB );
    return this->mProvider->mDB;
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::loadFromStore () {

    SQLite& db = this->getDB ();

    u64 sourceBranchID = 0;

    SQLiteResult result = db.exec (
        
        "SELECT sourceBranchID, version, top FROM branches WHERE branchID IS ?1",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, this->mBranchID );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            sourceBranchID      = stmt.getValue < u64 >( 0 );
            this->mVersion      = stmt.getValue < u64 >( 1 );
            this->mTopVersion   = stmt.getValue < u64 >( 2 );
        }
    );
    result.reportWithAssert ();
    
    this->mSourceBranch = this->mProvider->affirmBranch ( sourceBranchID );
    
    if ( this->mSourceBranch ) {
        this->mSourceBranch->insertClient ( *this );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::setTopVersion ( u64 topVersion ) {

    if ( this->mTopVersion == topVersion ) return;
    
    SQLite& db = this->getDB ();
    
    SQLiteResult result = db.exec (
    
        "UPDATE branches SET top = ?1 WHERE branchID IS ?2",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, topVersion );
            stmt.bind ( 2, this->mBranchID );
        }
    );
    result.reportWithAssert ();
    
    this->mTopVersion = topVersion;
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLiteVersionedBranch::SQLiteVersionedBranch ( shared_ptr < SQLitePersistenceProvider > provider, u64 branchID ) :
    mBranchID ( branchID ),
    mTopVersion ( 0 ),
    mProvider ( provider ) {
    
    assert ( provider );
    
    this->loadFromStore ();
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLiteVersionedBranch::SQLiteVersionedBranch ( shared_ptr < SQLitePersistenceProvider > provider, AbstractVersionedBranch& from ) :
    mBranchID ( 0 ),
    mProvider ( provider ) {
    
    assert ( provider );
    
    SQLiteVersionedBranch* sourceAsSQL     = dynamic_cast < SQLiteVersionedBranch* >( from.getSourceBranch ().get ());
    u64 sourceBranchID                      = sourceAsSQL ? sourceAsSQL->mBranchID : 0;
    u64 version                             = from.getVersion ();
    
    SQLite& db = this->getDB ();
        
    SQLiteResult result = db.exec (
    
        "INSERT INTO branches ( sourceBranchID, version, top ) VALUES ( ?1, ?2, ?3 )",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, sourceBranchID );
            stmt.bind ( 2, version );
            stmt.bind ( 3, version );
        }
    );
    result.reportWithAssert ();
    
    this->mBranchID = sqlite3_last_insert_rowid ( db );
    
    // set the source branch and version manually
    this->mSourceBranch     = from.getSourceBranch ();
    this->mVersion          = version;
    this->mTopVersion       = version;
    
    if ( this->mSourceBranch ) {
        this->mSourceBranch->insertClient ( *this );
    }
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLiteVersionedBranch::~SQLiteVersionedBranch () {

    if ( !this->mProvider->isFrozen ()) {
        this->deleteBranch ();
    }

    this->mProvider->eraseBranch ( *this );
    this->setParent ( NULL, 0 );
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::AbstractVersionedBranch_begin () {

    assert ( this->mProvider );
    return this->mProvider->begin ();
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::AbstractVersionedBranch_commit () {

    assert ( this->mProvider );
    return this->mProvider->commit ();
}

//----------------------------------------------------------------//
// TODO: doxygen
SQLiteVersionedBranch::ConstProviderPtr SQLiteVersionedBranch::AbstractPersistentVersionedBranch_getProvider () const {
    return this->mProvider;
}

//----------------------------------------------------------------//
/** \brief Initializes a new branch using the given branch as a parent. If the base
    version is less than the top of the parent branch, then the corresponding layer is copied.
 
    A parent branch must be provided. If the base version given is below the parent's immutable
    top (i.e. other clients or children depend on later versions) then all values set in that
    version must be copied into the child branch.
 
    The given version must be greater than or equal to the base version of the parent branch. (We
    could iterate back through the list of branches if an earlier version is given, but for the
    use case of this constructor, that functionality isn't needed.)
 
    There is a special case when a child branch is created with a base version of zero. Since
    there are no earlier versions, the parent branch's version may still be copied from, but no
    parent is set in the child. The child becomes a new root and thus has no parent.
 
    \param      parent          Parent branch.
    \param      baseVersion     Base version of the child branch.
*/
shared_ptr < AbstractVersionedBranch > SQLiteVersionedBranch::AbstractVersionedBranch_fork ( size_t baseVersion ) {
    
    shared_ptr < EphemeralVersionedBranch > child = make_shared < EphemeralVersionedBranch >();

    assert ( this->mVersion <= baseVersion );

    child->setParent ( this->mVersion < baseVersion ? this->shared_from_this () : this->mSourceBranch, baseVersion );

    if ( baseVersion < this->mTopVersion ) {

        SQLite& db = this->getDB ();

        // TODO: this is lazy; requires two calls for every value in layer
        SQLiteResult result = db.exec (
            
            "SELECT key FROM tuples WHERE branchID IS ?1 AND version IS ?2",
            
            //--------------------------------//
            [ & ]( SQLiteStatement& stmt ) {
                stmt.bind ( 1, this->mBranchID );
                stmt.bind ( 2, ( u64 )baseVersion );
            },
            
            //--------------------------------//
            [ & ]( int, const SQLiteStatement& stmt ) {
                
                string key = stmt.getValue < string >( 0 );
                
                Variant variant = this->AbstractVersionedBranch_getValueVariant ( baseVersion, key );
                child->setValueVariant ( baseVersion, key, variant );
            }
        );
        result.reportWithAssert ();
    }
    
    return child;
}

//----------------------------------------------------------------//
/** \brief Returns the version above the top layer held in the branch.

    \return     The version above the top layer or branch's base version if the branch is empty.
*/
size_t SQLiteVersionedBranch::AbstractVersionedBranch_getTopVersion () const {

    return this->mTopVersion;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t SQLiteVersionedBranch::AbstractVersionedBranch_getValueNextVersion ( string key, size_t version ) const {

    SQLite& db = this->getDB ();

    SQLiteResult result = db.exec (
        
        "SELECT MIN ( version ) FROM tuples WHERE branchID IS ?1 AND version > ?2 AND key is ?3",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, ( int )this->mBranchID );
            stmt.bind ( 2, ( int )version );
            stmt.bind ( 3, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            assert ( !stmt.isNull ( 0 ));
            version = ( size_t )stmt.getValue < u64 >( 0 );
        }
    );
    result.reportWithAssert ();
    
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
size_t SQLiteVersionedBranch::AbstractVersionedBranch_getValuePrevVersion ( string key, size_t version ) const {

    SQLite& db = this->getDB ();

    SQLiteResult result = db.exec (
        
        "SELECT MAX ( version ) FROM tuples WHERE branchID IS ?1 AND version < ?2 AND key is ?3",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, ( int )this->mBranchID );
            stmt.bind ( 2, ( int )version );
            stmt.bind ( 3, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            assert ( !stmt.isNull ( 0 ));
            version = ( size_t )stmt.getValue < u64 >( 0 );
        }
    );
    result.reportWithAssert ();
    
    return version;
}

//----------------------------------------------------------------//
// TODO: doxygen
Variant SQLiteVersionedBranch::AbstractVersionedBranch_getValueVariant ( size_t version, string key ) const {

    LGN_LOG_SCOPE ( PDM_FILTER_SQLSTORE, INFO, __PRETTY_FUNCTION__ );

    Variant variant;
    if (( this->mTopVersion == this->mVersion ) || ( this->mVersion > version )) return variant;

    version = version < this->mTopVersion ? version : this->mTopVersion - 1;

    SQLite& db = this->getDB ();

    SQLiteResult result = db.exec (
        
        "SELECT type, stringVal, intVal, realVal, MAX ( version ) FROM tuples WHERE branchID IS ?1 AND version <= ?2 AND key is ?3",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, ( int )this->mBranchID );
            stmt.bind ( 2, ( int )version );
            stmt.bind ( 3, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            if ( stmt.isNull ( 4 )) return;
            
            int type = stmt.getValue < int >( 0 );
            
            switch ( type ) {
                case Variant::BOOL_VARIANT:
                    variant = stmt.getValue < bool >( 2 );
                    break;
                case Variant::DOUBLE_VARIANT:
                    variant = stmt.getValue < double >( 3 );
                    break;
                case Variant::INT64_VARIANT:
                    variant = stmt.getValue < s64 >( 2 );
                    break;
                case Variant::UINT64_VARIANT:
                    variant = stmt.getValue < u64 >( 2 );
                    break;
                case Variant::STRING_VARIANT:
                    variant = stmt.getValue < string >( 1 );
                    break;
            }
        }
    );
    result.reportWithAssert ();

    return variant;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool SQLiteVersionedBranch::AbstractVersionedBranch_getValueVersionExtents ( string key, size_t upperBound, size_t& first, size_t& last ) const {

    if ( upperBound < this->mVersion ) return false;

    SQLite& db = this->getDB ();
    bool found = false;

    SQLiteResult result = db.exec (
        
        "SELECT MIN ( version ) FROM tuples WHERE branchID IS ?1 AND version <= ?2 AND key is ?3",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, ( int )this->mBranchID );
            stmt.bind ( 2, ( int )upperBound );
            stmt.bind ( 3, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            if ( stmt.isNull ( 0 )) return;
            
            first = ( size_t )stmt.getValue < u64 >( 0 );
            found = true;
        }
    );
    result.reportWithAssert ();

    if ( !found ) return false;

    result = db.exec (
        
        "SELECT MAX ( version ) FROM tuples WHERE branchID IS ?1 AND version <= ?2 AND key is ?3",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, ( int )this->mBranchID );
            stmt.bind ( 2, ( int )upperBound );
            stmt.bind ( 3, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            
            assert ( !stmt.isNull ( 0 ));
            last = ( size_t )stmt.getValue < u64 >( 0 );
        }
    );
    result.reportWithAssert ();
    
    return found;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool SQLiteVersionedBranch::AbstractVersionedBranch_hasKey ( string key, size_t upperBound ) const {

    if (( this->mTopVersion == this->mVersion ) || ( this->mVersion > upperBound )) return false;

    SQLite& db = this->getDB ();

    bool exists = false;
    
    SQLiteResult result = db.exec (
    
        "SELECT 1 FROM tuples WHERE branchID = ?1 AND version <= ?2 AND key = ?3",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, this->mBranchID );
            stmt.bind ( 2, ( u64 )upperBound );
            stmt.bind ( 3, key );
        },
        
        //--------------------------------//
        [ & ]( int, const SQLiteStatement& stmt ) {
            exists = ( stmt.getValue < int >( 0 ) == 1 );
        }
    );
    result.reportWithAssert ();

    return exists;
}

//----------------------------------------------------------------//
// TODO: doxygen
bool SQLiteVersionedBranch::AbstractVersionedBranch_isPersistent () const {

    return true;
}

//----------------------------------------------------------------//
/** \brief Implementation of virtual method. Appends the contents of
    the branch onto the given branch and transfers all clients and
    children to the given branch.
 
    The branch is optimized before being appended. Optimization may
    recursively trigger additional joins.
 
    Neither branch is permitted to have direct references.
 
    \param      other       The branch to be appended to.
*/
void SQLiteVersionedBranch::AbstractVersionedBranch_joinBranch ( AbstractVersionedBranch& other ) {

    SQLiteVersionedBranch* otherSQL = dynamic_cast < SQLiteVersionedBranch* >( &other );
    assert ( otherSQL );
    
    SQLite& db = this->getDB ();
    
    assert ( otherSQL->mTopVersion <= this->mVersion );
    
    SQLiteResult result = db.exec (
    
        "UPDATE tuples SET branchID = ?1 WHERE branchID = ?2",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, otherSQL->mBranchID );
            stmt.bind ( 2, this->mBranchID );
        }
    );
    result.reportWithAssert ();
    
    otherSQL->setTopVersion ( this->mTopVersion );
    
    this->transferClients ( other );
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::AbstractVersionedBranch_persist ( shared_ptr < AbstractPersistentVersionedBranch > persist ) {
    UNUSED ( persist );
    // do nothing; already persisted
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::AbstractVersionedBranch_print ( string prefix ) const {

    LGN_LOG ( PDM_FILTER_TREE, INFO,
        "%s[%d-%d]: sqlite %p (refs: %d)",
        prefix.c_str (),
        ( int )this->mVersion,
        ( int )this->getTopVersion (),
        this,
        ( int )this->mLockCount
    );
}

//----------------------------------------------------------------//
/** \brief Sets a value at the given version. If the version doesn't exist,
    a new layer will be created. Also creates a value stack if none exists. Throws
    a TypeMismatchOnAssignException if a value of a different type has already been
    assigned to the key.

    \param      version     The version to set the value at. Must be equal to or greater than the branch's base version.
    \param      key         Key of the value to set.
    \param      value       Raw pointer to value to set.

    \throws     TypeMismatchOnAssignException
*/
void SQLiteVersionedBranch::AbstractVersionedBranch_setValueVariant ( size_t version, string key, const Variant& value ) {

    SQLite& db = this->getDB ();

    //"INSERT OR REPLACE INTO tuples ( branchID, version, key, type, stringVal, intVal, realVal ) VALUES ( ?1, ?2, ?3, ?4, ?5, ?6, ?7 ) ON CONFLICT DO NOTHING",

    SQLiteResult result = db.exec (
        
        SQL_STR (
            INSERT INTO tuples ( branchID, version, key, type, stringVal, intVal, realVal ) VALUES ( ?1, ?2, ?3, ?4, ?5, ?6, ?7 )
            ON CONFLICT ( branchID, version, key )
            DO UPDATE
            SET type = excluded.type, stringVal = excluded.stringVal, intVal = excluded.intVal, realVal = excluded.realVal
        ),
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, this->mBranchID );
            stmt.bind ( 2, ( u64 )version );
            stmt.bind ( 3, key );
            
            size_t type = value.getType ();
            stmt.bind ( 4, ( u64 )type );
            
            string stringVal;
            s64 intVal      = 0;
            double realVal  = 0;
            
            switch ( type ) {
                case Variant::BOOL_VARIANT:
                    intVal = value.get < bool >();
                    break;
                case Variant::DOUBLE_VARIANT:
                    realVal = value.get < double >();
                    break;
                case Variant::INT64_VARIANT:
                    intVal = value.get < s64 >();
                    break;
                case Variant::UINT64_VARIANT: {
                    u64 uintVal = value.get < u64 >();
                    intVal = ( s64 )uintVal;
                    assert ( intVal == uintVal );
                    break;
                }
                case Variant::STRING_VARIANT:
                    stringVal = value.get < string >();
                    break;
            }
            
            stmt.bind ( 5, stringVal );
            stmt.bind ( 6, intVal );
            stmt.bind ( 7, realVal );
        }
    );
    result.reportWithAssert ();
    
    size_t top = version + 1;
    if ( this->mTopVersion < top ) {
        this->setTopVersion ( top );
    }
}

//----------------------------------------------------------------//
/** \brief Discards all layers and values with versions greater than or equal to the
    given version.
 
    \param      topVersion  Version for new "top" of branch.
*/
void SQLiteVersionedBranch::AbstractVersionedBranch_truncate ( size_t topVersion ) {
    
    if ( this->mTopVersion <= topVersion ) return;
    topVersion = topVersion < this->mVersion ? this->mVersion : topVersion;
    
    SQLite& db = this->getDB ();
    
    // TODO: check receiving branch top < this->mVersion
    
    SQLiteResult result = db.exec (
    
        "DELETE FROM tuples WHERE branchID = ?1 AND version >= ?2",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, this->mBranchID );
            stmt.bind ( 2, ( u64 )topVersion );
        }
    );
    result.reportWithAssert ();
    
    this->setTopVersion ( topVersion );
}

//----------------------------------------------------------------//
// TODO: doxygen
void SQLiteVersionedBranch::AbstractVersionedBranchClient_sourceBranchDidChange () {

    SQLite& db = this->getDB ();
        
    SQLiteVersionedBranch* sourceBranch = dynamic_cast < SQLiteVersionedBranch* >( this->mSourceBranch.get ());
    u64 sourceBranchID = sourceBranch ? sourceBranch->mBranchID : 0;
    
    SQLiteResult result = db.exec (
    
        "UPDATE branches SET sourceBranchID = ?1 WHERE branchID IS ?2",
        
        //--------------------------------//
        [ & ]( SQLiteStatement& stmt ) {
            stmt.bind ( 1, sourceBranchID );
            stmt.bind ( 2, this->mBranchID );
        }
    );
    result.reportWithAssert ();
}

} // namespace Padamose
