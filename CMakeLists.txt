project(Mettle)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
# include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
# include_directories(${CMAKE_SOURCE_DIR}/bencode.hpp/include)

find_package(Threads)

find_package(Boost REQUIRED COMPONENTS program_options filesystem iostreams)

# This is a hack for now.  For one thing, I need to get Windows supported
file(GLOB LIBRARY_SRC 
    "src/libmettle/*.cpp" 
    "src/libmettle/log/*.cpp" 
    "src/libmettle/posix/*.cpp" 
    "src/posix/*.cpp" 
    "src/libmettle/log/posix/*.cpp"
    )
add_library(Mettle SHARED ${LIBRARY_SRC})
target_include_directories(Mettle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Mettle Boost::program_options Boost::iostreams Threads::Threads)

file(GLOB EXECUTABLE_SRC "src/mettle/*.cpp" "src/mettle/posix/*.cpp")
add_executable(mettle_binary ${EXECUTABLE_SRC})
target_link_libraries(mettle_binary Mettle Boost::program_options Boost::iostreams Threads::Threads)

set_target_properties(mettle_binary PROPERTIES OUTPUT_NAME "mettle")

