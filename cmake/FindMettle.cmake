find_library(METTLE_LIB NAMES libmettle.so)
if(NOT METTLE_LIB)
  message(FATAL_ERROR "mettle library not found")
endif()

find_path(METTLE_INCLUDE_DIR NAMES mettle.hpp)
if(NOT METTLE_INCLUDE_DIR)
  message(FATAL_ERROR "mettle headers not found")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Mettle
	REQUIRED_VARS METTLE_LIB METTLE_INCLUDE_DIR
)

if(METTLE_FOUND AND NOT TARGET Mettle::Mettle)
	add_library(Mettle::Mettle INTERFACE IMPORTED)
	set_target_properties(Mettle::Mettle PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${METTLE_INCLUDE_DIR}"
		INTERFACE_LINK_LIBRARIES "${METTLE_LIB}"
		)
endif()
