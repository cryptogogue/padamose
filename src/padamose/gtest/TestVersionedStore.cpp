// Copyright (c) 2017-2018, Cryptogogue Inc. All Rights Reserved.
// http://cryptogogue.com

#include <padamose/gtest/gtest-helpers.h>
#include <padamose/padamose.h>

namespace Padamose {
namespace Test {

static const string KEY     = "test";
static const string KEY2    = "test2";
static const string STR0    = "abc";
static const string STR1    = "def";
static const string STR2    = "ghi";
static const string STR3    = "jkl";
static const string STR4    = "mno";

//----- Let's start with some simple tests -----/
//----- Set/Get Values -----/

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_hasValue_returns_false_if_value_was_not_set ) {
    VersionedStoreTag store;

    ASSERT_FALSE ( store.hasValueWithType < string >( KEY ) );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_hasValue_returns_true_if_value_was_set ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );

    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ) );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_hasValue_if_value_was_set_with_different_type_fails ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );

    ASSERT_FALSE ( store.hasValueWithType < s64 >( KEY ));

}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_setValue_for_string_type ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_setValue_for_int_type ) {
    const s64 value = 1;
    VersionedStoreTag store;
    store.setValue < s64 >( KEY, value );
    
    ASSERT_EQ ( store.getValue < s64 >( KEY ), value );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_setValue_for_float_type ){
    const double value = 1.f;
    VersionedStoreTag store;
    store.setValue < float >( KEY, value );
    
    ASSERT_EQ ( store.getValue < double >( KEY ), value );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_setValue_for_bool_type ){
    const bool value = false;
    VersionedStoreTag store;
    store.setValue < bool >( KEY, value );

    ASSERT_EQ ( store.getValue < bool >( KEY ), value );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_getValue_key_does_not_exists_exception_fails ){
    VersionedStoreTag store;
    
    ASSERT_EXCEPTION_CAUGHT ( store.getValue < string >( KEY ), KeyNotFoundException );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_getValue_mismatch_type_exception_fails ){
    const string value = "sample string";
    VersionedStoreTag store;
    store.setValue < string >( KEY, value );
    
    ASSERT_EXCEPTION_CAUGHT ( store.getValue < s64 >( KEY ), TypeMismatchOnGetException );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_setValue_different_type_for_the_same_key_exception_fails ){
    const string str_value = "sample string";
    const int int_value = 1;
    VersionedStoreTag store;
    store.setValue < string >( KEY, str_value );
    
    ASSERT_EXCEPTION_CAUGHT ( store.setValue < s64 >( KEY, int_value ), TypeMismatchOnAssignException );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_setValue_different_type_with_different_keys ){
    const int int_value = 1;
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    store.setValue < s64 >( KEY2, int_value );

    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
    ASSERT_EQ ( store.getValue < s64 >( KEY2 ), int_value );
}

//-------------- Let's make some snapshots -----------------------//

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_assign_operator_copy_store_state ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0);
    VersionedStoreTag store1 = store;
    
    ASSERT_EQ ( store1.getValue < string >( KEY ), STR0);
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_getSnapshot_copy_store_state ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    VersionedStoreTag store1;
    store1.takeSnapshot( store );
    
    ASSERT_EQ ( store1.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_getSnapshot_not_preserve_current_store_state ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    VersionedStoreTag store1;
    store1.setValue < s64 >( KEY, 1 );
    store1.setValue < string >( KEY2, STR1 );
    store1.takeSnapshot ( store );
    
    ASSERT_EQ ( store1.getValue < string >( KEY ), STR0);
    ASSERT_FALSE ( store1.hasValueWithType < string >( KEY2 ));
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_adding_value_to_snapshoot_does_not_affect_original ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    VersionedStoreTag store1;
    store1.takeSnapshot ( store );
    store1.setValue < string >( KEY2, STR1 );
    
    ASSERT_TRUE ( store1.hasValueWithType < string >( KEY2 ));
    ASSERT_FALSE ( store.hasValueWithType < string >( KEY2 ));
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_changing_value_to_snapshoot_does_not_affect_original ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    VersionedStoreTag store1;
    store1.takeSnapshot ( store );
    store1.setValue < string >( KEY, STR1 );
    
    ASSERT_EQ ( store1.getValue < string >( KEY ), STR1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_snapshot_will_have_same_version ) {
    VersionedStoreTag store;
    store.pushVersion ();
    ASSERT_EQ ( store.getVersion (), 1 );
    
    VersionedStoreTag store1;
    store1.takeSnapshot ( store );
    
    ASSERT_EQ ( store.getVersion (), store1.getVersion () );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_store_snapshot_clone_values_for_every_store_version ) {
    //Create store and set values for version 0 and version 1
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );
   
    // Do snapshot and popVersion
    VersionedStoreTag store1;
    store1.takeSnapshot ( store );
    store1.popVersion ();

    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
    ASSERT_EQ ( store1.getValue < string >( KEY ), STR0 );
    ASSERT_EQ ( store1.getVersion (), 0 );
}

//-------------- Let's try some versioning -----------------------//

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_newly_created_store_has_version_0 ) {
    VersionedStoreTag store;

    ASSERT_EQ ( store.getVersion (), 0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_pushVersion_without_value_will_increase_version ) {
    VersionedStoreTag store;
    store.pushVersion();
 
    ASSERT_EQ ( store.getVersion (), 1 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_pushVersion_will_not_change_values_set ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();

    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ) );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_popVersion_when_version_0_does_not_decrease_version ) {
    VersionedStoreTag store;
    store.popVersion ();

    ASSERT_EQ ( store.getVersion (), 0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_popVersion_when_version_0_erases_values_set ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    
    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ) );
    store.popVersion();

    ASSERT_FALSE ( store.hasValueWithType < string >( KEY ) );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_popVersion_reverts_previous_version_value ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
    
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );
    
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );

    store.popVersion ();

    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ) );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_popVersion_permamently_removes_value_even_if_pushValue_again_without_changes ) {
    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );
    
    store.popVersion ();
    store.pushVersion ();
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_multiple_setValue_without_pushVersion_will_override_current_value ) {
    VersionedStoreTag store;

    // setValue for version 0
    store.setValue < string >( KEY, STR0 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
    
    store.pushVersion ();
   
    // Update value three times on the same version 1
    store.setValue < string >( KEY, STR1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
    
    store.setValue < string >( KEY, STR2 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR2 );
    
    store.setValue < string >( KEY, STR3 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR3 );

    // popVersion reverts to value set on version 0
    store.popVersion ();

    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ) );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_clear_will_remove_all_changes_made_for_that_version_and_set_version_0 ){
    VersionedStoreTag store;
    
    // setValue for version 0
    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );

    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ) );
    
    store.clear ();
    ASSERT_EQ ( store.getVersion (), 0 );
    ASSERT_FALSE ( store.hasValueWithType < string >( KEY ) );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_clearVerion_will_remove_all_changes_made_for_that_version_and_version_is_0 ){
    VersionedStoreTag store;

    // setValue for version 0
    store.setValue < string >( KEY, STR0 );
    ASSERT_TRUE ( store.hasValueWithType < string >( KEY ));
    
    store.clearVersion ();
    
    ASSERT_EQ ( store.getVersion (), 0 );
    ASSERT_FALSE ( store.hasValueWithType < string >( KEY ));
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_clearVerion_will_remove_all_changes_made_for_that_version_but_vesion_number_will_not_be_altered ){
    VersionedStoreTag store;

    // setValue for version 0
    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );

    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
    
    store.clearVersion ();
    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_revert_to_current_version_doesnt_make_any_changes ){
    VersionedStoreTag store;

    // setValue for version 0
    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );

    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
    
    store.revert ( store.getVersion ());
    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_revert_to_non_existing_future_version_exception_fails ){
    VersionedStoreTag store;

    // setValue for version 0
    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );

    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
    
    ASSERT_EXCEPTION_CAUGHT ( store.revert ( store.getVersion () + 10 ), VersionOutOfBoundsException );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_revert_to_negative_version_exception_fails ){
    VersionedStoreTag store;

    store.setValue < string >( KEY, STR0 );
    store.pushVersion ();
    store.setValue < string >( KEY, STR1 );

    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
   
    // size_t is unsigned so in fact this will end in large possitive number
    ASSERT_EXCEPTION_CAUGHT ( store.revert ( -1 ), VersionOutOfBoundsException );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_revert_to_specific_version_will_restore_value_set_in_that_version ){
    VersionedStoreTag store;

    store.setValue < string >( KEY, STR0 );
    store.pushVersion (); // version 1 
    store.setValue < string >( KEY, STR1 );
    store.pushVersion (); // version 2
    store.setValue < string >( KEY, STR2 );
    store.pushVersion (); // version 3
    store.setValue < string >( KEY, STR3 );

    ASSERT_EQ ( store.getVersion (), 3 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR3 );
   
    store.revert ( 1 );
    ASSERT_EQ ( store.getVersion (), 1 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_revert_to_version_will_restore_value_set_in_that_version ){
    VersionedStoreTag store;

    store.setValue < string >( KEY, STR0 );
    store.pushVersion (); // version 1
    store.setValue < string >( KEY, STR1 );
    store.pushVersion (); // version 2
    store.setValue < string >( KEY, STR2 );

    ASSERT_EQ ( store.getVersion (), 2 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR2 );
   
    store.revert ( 0 );
    ASSERT_EQ ( store.getVersion (), 0 );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test_fork_after_empty_versions ){

    VersionedStoreTag store;
    store.setValue < string >( KEY, STR0 );
    store.pushVersion (); // version 1
    store.setValue < string >( KEY, STR1 );
    store.pushVersion (); // version 2
    store.pushVersion (); // version 3

    VersionedStoreTag fork ( store );
    ASSERT_EQ ( store.getValue < string >( KEY ), STR1 );
    ASSERT_EQ ( fork.getValue < string >( KEY ), STR1 );
   
    fork.setValue < string >( KEY, STR2 );
    ASSERT_EQ ( fork.getValue < string >( KEY ), STR2 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test0 ) {

    VersionedStoreTag store0;
    VersionedStoreTag store1;
    VersionedStoreTag store2;
 
    store0.setDebugName ( "store0" );
    store1.setDebugName ( "store1" );
    store2.setDebugName ( "store2" );
 
    store0.setValue < string >( KEY, STR0 );
    store0.pushVersion ();
    store0.setValue < string >( KEY, STR1 );
    
    store1.takeSnapshot ( store0 );
    store0.setValue < string >( KEY, STR2 );
    
    store2.takeSnapshot ( store0 );
    store0.setValue < string >( KEY, STR3 );
    
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR3 );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR1 );
    ASSERT_TRUE ( store2.getValue < string >( KEY ) == STR2 );
    
    ASSERT_TRUE ( store0.getVersion () == 1 );
    ASSERT_TRUE ( store1.getVersion () == 1 );
    ASSERT_TRUE ( store2.getVersion () == 1 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test1 ) {

    VersionedStoreTag store0;
    VersionedStoreTag store1;
 
    store0.setValue < string >( KEY, STR0 );
    store1.takeSnapshot ( store0 );
    store0.setValue < string >( KEY, STR1 );
    
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR1 );
    ASSERT_TRUE ( store0.getVersion () == 0 );
    
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store1.getVersion () == 0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test2 ) {

    VersionedStoreTag store0;
    
    store0.popVersion ();
    
    ASSERT_TRUE ( store0.hasValueWithType < string >( KEY ) == false );
    ASSERT_TRUE ( store0.getVersion () == 0 );
    
    store0.setValue < string >( KEY, STR0 );
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store0.getVersion () == 0 );
    
    store0.pushVersion ();
    store0.setValue < string >( KEY, STR1 );
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR1 );
    ASSERT_TRUE ( store0.getVersion () == 1 );
    
    store0.popVersion ();
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store0.getVersion () == 0 );
    
    store0.popVersion ();
    ASSERT_TRUE ( store0.hasValueWithType < string >( KEY ) == false );
    ASSERT_TRUE ( store0.getVersion () == 0 );
    
    store0.setValue < string >( KEY, STR0 );
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store0.getVersion () == 0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test3 ) {

    VersionedStoreTag store0;
    VersionedStoreTag store1;
    VersionedStoreTag store2;
    
    // set a v0 value
    store0.setValue < string >( KEY, STR0 );
    
    // sanity test - get back the value we set
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    
    // new version
    store0.pushVersion ();
    
    // should not change value
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    
    // set a v1 value
    store0.setValue < string >( KEY, STR1 );
    
    // make sure we get our new value
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR1 );
    
    // back to v0
    store0.popVersion ();
    
    // should now get the original v0 value
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    
    // make store1 a snapshot of store0
    store1 = store0;
    
    // make sure we can get our v0 from the store1
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR0 );
    
    // now push a new version on the stores
    store0.pushVersion ();
    store1.pushVersion ();
    
    // values in either version should still be v0
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR0 );
    
    // aset a new value into store0
    store0.setValue < string >( KEY, STR2 );
    
    // the value in store0 should change, but not in store1
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR2 );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR0 );
    
    // set a new value into store1
    store1.setValue < string >( KEY, STR3 );
    
    // the value in store0 should change, but not in store1
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR2 );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR3 );
    
    // now pop store0...
    store0.popVersion ();
    
    // store0 should be back to the original value
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR3 );
    
    // and pop store1...
    store1.popVersion ();
    
    // WHAT STRANGE ENCHANTMENT BE THIS?!
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == STR0 );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == STR0 );
}

//----------------------------------------------------------------//
TEST ( VersionedStoreTag, test4 ) {

    VersionedStoreTag store0;
    VersionedStoreTag store1;

    store0.setValue < string >( KEY, "a" );
    store0.pushVersion ();
    
    store0.setValue < string >( KEY, "b" );
    store0.pushVersion ();
    
    store0.setValue < string >( KEY, "c" );
    store0.pushVersion ();

    store1 = store0;
    
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == "c" );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == "c" );
    
    store0.setValue < string >( KEY, "aa" );
    store1.setValue < string >( KEY, "bb" );
    
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == "aa" );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == "bb" );
    
    store0.popVersion ();
    
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == "c" );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == "bb" );
    
    store1.popVersion ();
    ASSERT_TRUE ( store0.getValue < string >( KEY ) == "c" );
    ASSERT_TRUE ( store1.getValue < string >( KEY ) == "c" );
}

} // namespace Test
} // namespace Padamose
