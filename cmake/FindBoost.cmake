FIND_PATH(Boost_ROOT_DIR boost/utility.hpp 
		    $ENV{Boost_ROOT_DIR} 
		    /usr/include
		    /usr/local/include
		    /opt
		    /opt/include
		    ${PROJECT_SOURCE_DIR}/../boost
		    DOC "Path to Boost directory"
	)

SET(Boost_VER 1_31)
SET(Boost_INCLUDE_DIR ${Boost_ROOT_DIR})
	    
SET(Boost_LIB_SUFFIX "gcc")

FIND_LIBRARY(Boost_Regex_LIBRARY 
  NAMES boost_regex-${Boost_LIB_SUFFIX}-${Boost_VER} boost_regex 
  PATHS ${Boost_ROOT_DIR}/../lib ${Boost_ROOT_DIR}/stage/lib
  DOC "Boost Regular expressions")

FIND_LIBRARY(Boost_Filesystem_LIBRARY
  NAMES boost_filesystem-${Boost_LIB_SUFFIX}-${Boost_VER} boost_filesystem 
  PATHS ${Boost_ROOT_DIR}/../lib ${Boost_ROOT_DIR}/stage/lib
  DOC "Boost Filesystem")
