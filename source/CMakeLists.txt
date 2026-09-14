add_executable(main main.cpp)
add_library(placeHolderLibrary 
    placeHolder/placeHolder.cpp placeHolder/placeHolder.hpp
)

target_include_directories(placeHolderLibrary PUBLIC placeHolder)

target_link_libraries(main PRIVATE placeHolderLibrary)