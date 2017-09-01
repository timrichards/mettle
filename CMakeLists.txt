project(Mettle)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

include(../conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/bencode.hpp/include)

find_package(Threads)

find_package(Boost REQUIRED COMPONENTS program_options iostreams)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

# This is a hack for now.  For one thing, I need to get Windows supported
file(GLOB LIBRARY_SRC 
    "src/libmettle/*.cpp" 
    "src/libmettle/log/*.cpp" 
    "src/libmettle/posix/*.cpp" 
    "src/posix/*.cpp" 
    "src/libmettle/log/posix/*.cpp"
    )
add_library(mettle SHARED ${LIBRARY_SRC})
target_link_libraries(mettle ${CONAN_LIBS} 
    Boost::program_options Boost::filesystem Boost::regex 
    Threads::Threads)

file(GLOB EXECUTABLE_SRC "src/mettle/*.cpp" "src/mettle/posix/*.cpp")
add_executable(mettle_binary ${EXECUTABLE_SRC})
target_link_libraries(mettle_binary mettle ${CONAN_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(mettle_binary PROPERTIES OUTPUT_NAME "mettle")

