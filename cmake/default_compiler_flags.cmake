if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4 /w14640 /permissive-
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(
        -Wall -Wextra -Wpedantic
    )
endif()
