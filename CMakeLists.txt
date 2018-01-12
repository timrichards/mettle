project(Mettle)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

find_file(CONAN_BUILDINFO NAMES conanbuildinfo.cmake PATHS
    ${CMAKE_BINARY_DIR} # Development builds
    ${CMAKE_BINARY_DIR}/..  # Conan create
    ${CMAKE_SOURCE_DIR}/conan  # Used by Bamboo continuous integration
    )
include(${CONAN_BUILDINFO})
conan_basic_setup(TARGETS)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/bencode.hpp/include)

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
add_library(mettle SHARED ${LIBRARY_SRC})
target_link_libraries(mettle CONAN_PKG::Boost Threads::Threads)

file(GLOB EXECUTABLE_SRC "src/mettle/*.cpp" "src/mettle/posix/*.cpp")
add_executable(mettle_binary ${EXECUTABLE_SRC})
target_link_libraries(mettle_binary mettle CONAN_PKG::Boost Threads::Threads)
install(TARGETS mettle mettle_binary
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

set_target_properties(mettle_binary PROPERTIES OUTPUT_NAME "mettle")

