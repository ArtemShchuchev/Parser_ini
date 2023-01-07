cmake_minimum_required(VERSION 3.5)
project(Parser_ini)
add_executable(parser main.cpp SecondaryFunction.h SecondaryFunction.cpp Parser.h Parser.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT parser)

# if (Visual Studio) then set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else(MSVC)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif(MSVC)




### add_subdirectory(../Catch2 Catch2-build)
#find_package(Catch2 3 REQUIRED)
## 		These tests need their own main
#add_executable(test test.cpp Table.hpp)
#target_link_libraries(test PRIVATE Catch2::Catch2)
