project(Mettle LANGUAGES CXX)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/bencode.hpp/include)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost REQUIRED COMPONENTS program_options iostreams)
include_directories(${Boost_INCLUDE_DIRS})

# This is a hack for now.  For one thing, I need to get Windows supported
file(GLOB LIBRARY_SRC 
    "src/libmettle/*.cpp" 
    "src/libmettle/log/*.cpp" 
    "src/posix/*.cpp" 
    "src/libmettle/log/posix/*.cpp"
    )
add_library(mettle STATIC ${LIBRARY_SRC})
target_link_libraries(mettle ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT})

file(GLOB EXECUTABLE_SRC "src/mettle/*.cpp" "src/mettle/posix/*.cpp")
add_executable(mettle_binary ${CONAN_LIBS} ${EXECUTABLE_SRC})
target_link_libraries(mettle_binary mettle ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(mettle_binary PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME "mettle")

