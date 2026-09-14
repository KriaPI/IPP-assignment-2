include(GoogleTest)

# Define executable targets
add_executable(place test.cpp)

# Link targets
target_link_libraries(place PRIVATE
    GTest::gtest GTest::gtest_main
    placeHolderLibrary
)

# Define tests
gtest_discover_tests(place)