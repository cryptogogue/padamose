// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef PADAMOSE_ABSTRACTVERSIONEDSTORETAG_H
#define PADAMOSE_ABSTRACTVERSIONEDSTORETAG_H

#include <padamose/padamose-common.h>
#include <padamose/AbstractVersionedBranch.h>
#include <padamose/VersionedStoreInspector.h>

namespace Padamose {

class VersionedStoreTag;

//================================================================//
// AbstractAbstractVersionedStoreTag
//================================================================//
// TODO: doxygen
class AbstractVersionedStoreTag :
    public virtual AbstractVersionedStoreInspector {
protected:

    friend class VersionedStoreRef;

    //----------------------------------------------------------------//
    void                    affirmBranch                        ();
    void                    prepareForSetValue                  ();
    
    //----------------------------------------------------------------//
    virtual VersionedStoreTag&          AbstractVersionedStoreTag_getTag            () = 0;
    
public:

    //----------------------------------------------------------------//
                            AbstractVersionedStoreTag           ();
    virtual                 ~AbstractVersionedStoreTag          ();
    void                    clearVersion                        ();
    VersionedStoreTag&      getTag                              ();
    void                    popVersion                          ();
    void                    pushVersion                         ();
    void                    revert                              ( size_t version );
    void                    revertAndClear                      ( size_t version );
    void                    setValueVariant                     ( string key, const Variant& value );
    void                    takeSnapshot                        ( const AbstractVersionedStoreTag& other );

    //----------------------------------------------------------------//
    /** \brief  Sets or overwrites the value for the corresponding key at the
                current version.
     
        \param  key         The key.
        \param  value       The value.
    */
    template < typename TYPE >
    void setValue ( string key, const TYPE& value ) {
        this->setValueVariant ( key, Variant ( value ));
    }
};

} // namespace Padamose
#endif
