# List of test functions
#add_custom_test(shared_dummy)
#add_custom_test(Board)
#add_custom_test(Game)
#add_custom_test(Pieces)
#add_custom_test(Player)
add_custom_test(Engine)
add_custom_test(RandomAI)

# vim: set sw=2 sts=2 et:


##List of test functions
## Find all test_*.cpp files in the current directory
#file(GLOB TEST_SOURCES "test_*.cpp")
#
##Add a custom test for each found source file
#foreach(TEST_SOURCE ${TEST_SOURCES})
#    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
#    string(REPLACE "test_" "" TEST_NAME ${TEST_NAME})
#    add_custom_test(${TEST_NAME})
#endforeach()
#
## vim: set sw=2 sts=2 et:


