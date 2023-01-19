cmake_minimum_required(VERSION 3.5)
project(Parser_ini VERSION 1.0 LANGUAGES CXX)
add_executable(parser
	main.cpp
	SecondFunk/SecondaryFunction.h SecondFunk/SecondaryFunction.cpp
	Parse/Parser.h Parse/Parser.cpp)



# if (Visual Studio)
# настраиваю в качестве запускаемого проекта
# настройка на стандарт с++17
if(MSVC)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT parser)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()




### add_subdirectory(../Catch2 Catch2-build)
#find_package(Catch2 3 REQUIRED)
## 		These tests need their own main
#add_executable(test test.cpp Table.hpp)
#target_link_libraries(test PRIVATE Catch2::Catch2)
