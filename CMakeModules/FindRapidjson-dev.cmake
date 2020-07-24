# Locate HPDF library
# This module defines
# HPDF_LIBRARY, the name of the library to link against
# HPDF_FOUND, if false, do not try to link to HPDF
# HPDF_INCLUDE_DIR, where to find hpdf.h
#

message(STATUS "looking for rapidjson-dev")

set( ARCHIVE_FOUND OFF )

find_path( ARCHIVE_INCLUDE_DIR rapidjson/rapidjson.h
  HINTS
  $ENV{ARCHIVEDIR}
  PATH_SUFFIXES include 
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include 
  /opt/include
  /mingw/include
)

find_path( ARCHIVE_INCLUDE_DIR rapidjson/document.h
  HINTS
  $ENV{ARCHIVEDIR}
  PATH_SUFFIXES include 
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include 
  /opt/include
  /mingw/include
)

find_path( ARCHIVE_INCLUDE_DIR rapidjson/prettywriter.h
  HINTS
  $ENV{ARCHIVEDIR}
  PATH_SUFFIXES include 
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include 
  /opt/include
  /mingw/include
)

#message( "ARCHIVE_INCLUDE_DIR is ${ARCHIVE_INCLUDE_DIR}" )

if(ARCHIVE_INCLUDE_DIR)
	set( ARCHIVE_FOUND "YES" )
endif()


if(ARCHIVE_FOUND)
    message(STATUS "looking for rapidjson-dev - done")
else()
    message(FATAL_ERROR "ERROR: rapidjson-dev not installed")
endif()
