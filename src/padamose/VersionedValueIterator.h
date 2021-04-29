// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_VERSIONEDVALUEITERATOR_H
#define PADAMOSE_VERSIONEDVALUEITERATOR_H

#include <padamose/padamose-common.h>
#include <padamose/VersionedStoreLock.h>

namespace Padamose {

//================================================================//
// VersionedValueIterator
//================================================================//
/** \brief Implements a sparse version iterator of a specific value
    in a store.

    The iterator can traverse from the current version of the value
    stack used to initialize it back to version zero. Iteration works
    in both directions, though iterating forward may incur additional
    overhead in cases where multiple branches exist.
 
    When the iterator is initialized, an "anchor" snapshot is created.
    This ensures that the values being iterated will not change, even
    if the initializing snapshot is rewound and modified. In other words,
    the iterator is itself a snapshot of the entire version history.
 
    If the iterator is iterated past the beginning or end of its
    range, it will become invalid. To restore it, step the iterator
    in the opposite direction.
 
    Iteration is performed in a sparse manner: only the versions of
    the store where the value was modified will be visited.
*/
template < typename TYPE >
class VersionedValueIterator :
    public virtual HasVersionedBranch {
protected:

    friend class VersionedStoreTag;

    enum {
        VALID,
        
        // these are only set *after* a call to prev() or next().
        // they are not meant to be exposed or for general use.
        NO_PREV,
        NO_NEXT,
    };

    /// The anchor snapshot.
    shared_ptr < AbstractVersionedBranch >  mAnchorBranch;
    size_t                                  mAnchorVersion;
    
    /// Key of the value being iterated.
    string                      mKey;
    
    /// Internal state of the iterator.
    int                         mState;
    
    /// Lower bound of the stack being iterated.
    size_t                      mFirstVersion;
    
    /// Upper bound of the stack being iterated.
    size_t                      mLastVersion;

    //----------------------------------------------------------------//
    /** \brief Starts at the anchor's branch and seeks backward to find the
        "next" value. This is called when the upper bound of a branch is reached.
     
        \param     prevBranch   Lower bound branch; the seek will stop at (and exclude) this branch.
    */
    void seekNext ( shared_ptr < AbstractVersionedBranch > prevBranch ) {
        
        shared_ptr < AbstractVersionedBranch > branch = this->mAnchorBranch;
        size_t top = this->mAnchorVersion + 1;

        shared_ptr < AbstractVersionedBranch > bestBranch;

        size_t first = 0;
        size_t last = 0;

        // walk back to prevBranch (from the top), searing for the earliest branch
        // that contains values.
        for ( ; branch != prevBranch; branch = branch->mSourceBranch ) {
            if ( branch->getValueVersionExtents ( this->mKey, top - 1, first, last )) {
                bestBranch = branch;
            }
            top = branch->mVersion;
        }

        if ( bestBranch ) {

            this->mFirstVersion = first;
            this->mLastVersion = last;

            this->setBranch ( bestBranch, this->mFirstVersion );
            this->mState = VALID;
        }
        else {
            this->mState = NO_NEXT;
        }
    }

    //----------------------------------------------------------------//
    /** \brief Starts at the current branch and seeks backward to find the
        "prev" value. This is called when the lower bound of a branch is reached.
     
        An "upper bound" is given via the "top" param. This is necessary to correctly
        set the branch bounds.
     
        \param      branch      Starting branch for the search.
        \param      top         Upper bound for the search.
    */
    void seekPrev ( shared_ptr < AbstractVersionedBranch > branch, size_t top ) {
        
        for ( ; branch; branch = branch->mSourceBranch ) {

            size_t first;
            size_t last;

            if ( branch->getValueVersionExtents ( this->mKey, top - 1, first, last )) {

                this->mFirstVersion = first;
                this->mLastVersion = last;

                this->setBranch ( branch, this->mLastVersion );
                this->mState = VALID;
                return;
            }
            top = branch->mVersion;
        }
        this->mState = NO_PREV;
    }

public:

    //----------------------------------------------------------------//
    /** \brief Operator overload for bool. False if the iterator is empty
        or has been iterated past its bounds. Calls isValid() internally.
    */
    operator bool () const {
        return this->isValid ();
    }

    //----------------------------------------------------------------//
    /** \brief Prefix increment operator. Calls next() internally.
    */
    VersionedStoreIterator& operator++ () {
        this->next ();
        return *this;
    }
    
    //----------------------------------------------------------------//
    /** \brief Prefix decrement operator. Calls prev() internally.
    */
    VersionedStoreIterator& operator-- () {
        this->prev ();
        return *this;
    }

    //----------------------------------------------------------------//
    /** \brief Dereference operator. Calls value() internally.
    */
    const TYPE operator * () const {
        return this->value ();
    }

    //----------------------------------------------------------------//
    /** \brief Returns true if iterator is in the valid range for iteration.

        \return     True if iteration is in the valid range.
    */
    bool isValid () const {
        return ( this->mState == VALID );
    }
    
    //----------------------------------------------------------------//
    /** \brief Iterates forward by one version.

        \return     True if the step was successful.
    */
    bool next () {
        
        if ( !this->mSourceBranch ) return false;
        
        if ( this->mState == NO_PREV ) {
            this->mState = VALID;
        }
        else if ( this->mState != NO_NEXT ) {
            
             if ( this->mVersion < this->mLastVersion ) {
                this->mVersion = this->mSourceBranch->getValueNextVersion ( this->mKey, this->mVersion );
            }
            else {
                assert ( this->mVersion == this->mLastVersion );
                this->seekNext ( this->mSourceBranch );
            }
        }
        return ( this->mState != NO_NEXT );
    }
    
    //----------------------------------------------------------------//
    /** \brief Iterates backward by one version.

        \return     True if the step was successful.
    */
    bool prev () {

        if ( !this->mSourceBranch ) return false;

        if ( this->mState == NO_NEXT ) {
            this->mState = VALID;
        }
        else if ( this->mState != NO_PREV ) {
                
            if ( this->mVersion > this->mFirstVersion ) {
                this->mVersion = this->mSourceBranch->getValuePrevVersion ( this->mKey, this->mVersion );
            }
            else {
                assert ( this->mVersion == this->mFirstVersion );
                this->seekPrev ( this->mSourceBranch->mSourceBranch, this->mVersion );
            }
        }
        return ( this->mState != NO_PREV );
    }

    //----------------------------------------------------------------//
    /** \brief Find the position where the value version is equal to
        or less than the given version.

        \todo This implementation is pretty inefficient in that every value is visited.
    */
    void seekLower ( size_t version ) {
    
        assert ( this->mAnchor.mSourceBranch );
    
        this->seekPrev ( this->mAnchor.mSourceBranch, this->mAnchor.mVersion + 1 );
        for ( ; this->isValid () && ( this->mVersion > version ); this->prev ());
        // iterator will now be <= version, or nil
    }
    
    //----------------------------------------------------------------//
    /** \brief Find the position where the value version is equal to
        or greater than the given version.

        \todo This implementation is pretty inefficient in that every value is visited.
    */
    void seekUpper ( size_t version ) {
    
        this->seekLower ();
        if ( this->isValid () && ( this->mVersion < version )) {
            this->next ();
        }
    }

    //----------------------------------------------------------------//
    /** \brief Returns a const reference to the current value being iterated.

        \return     The current value of the iterator.
    */
    const TYPE value () const {
        assert ( this->isValid ());
        return this->getValue < TYPE >( this->mKey );
    }
    
    //----------------------------------------------------------------//
    /** \brief Initialize the iterator with a given snapshot and set the
        key of the value to iterate. Internal type of the key must match the
        template type.

        \param  client      Snapshot to use as the upper bound for iteration.
        \param  key         Key of the value to be iterated.
    */
    VersionedValueIterator ( const AbstractHasVersionedBranch& other, string key ) :
        mAnchorBranch ( other.getSourceBranch ()),
        mAnchorVersion ( other.getVersion ()),
        mKey ( key ) {
        
        HasVersionedBranch::BranchPtr sourceBranch = this->mAnchorBranch;
        
        if ( sourceBranch ) {
            this->seekPrev ( sourceBranch, this->mAnchorVersion + 1 );
        }
    }
    
    //----------------------------------------------------------------//
    ~VersionedValueIterator () {
    }
};

} // namespace Padamose
#endif
