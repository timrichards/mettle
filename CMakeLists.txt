cmake_minimum_required(VERSION 3.2)
project(Mettle LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS program_options iostreams)

file(GLOB LIBRARY_SRC 
    "src/libmettle/*.cpp" 
    "src/libmettle/log/*.cpp" 
    )

if (UNIX)
    file(GLOB PLATFORM_SRC
    	"src/libmettle/posix/*.cpp" 
    	"src/posix/*.cpp" 
    	"src/libmettle/log/posix/*.cpp"
	)
elseif (WIN32)
    message(FATAL_ERROR "Mettle CMakeLists.txt not tested on Windows (although Mettle itself is")
    file(GLOB PLATFORM_SRC
    	"src/libmettle/windows/*.cpp" 
    	"src/windows/*.cpp" 
    	"src/libmettle/log/windows/*.cpp"
	)
else (UNIX)
    message(FATAL_ERROR "Mettle CMakeLists.txt supported only on Posix for now")
endif (UNIX)

add_library(mettle SHARED ${LIBRARY_SRC} ${PLATFORM_SRC})
target_include_directories(mettle
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mettle
    PUBLIC
        Boost::program_options Boost::iostreams Threads::Threads
)

file(GLOB EXECUTABLE_SRC 
	"src/mettle/*.cpp" 
	"src/mettle/posix/*.cpp"
	)

add_executable(mettle_runner ${EXECUTABLE_SRC})
target_link_libraries(mettle_runner
	mettle Boost::program_options Boost::iostreams Threads::Threads)

set_target_properties(mettle_runner PROPERTIES OUTPUT_NAME "mettle")

