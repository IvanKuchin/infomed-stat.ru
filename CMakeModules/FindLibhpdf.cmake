# Locate HPDF library
# This module defines
# HPDF_LIBRARY, the name of the library to link against
# HPDF_FOUND, if false, do not try to link to HPDF
# HPDF_INCLUDE_DIR, where to find hpdf.h
#

message(STATUS "looking for libhpdf-dev")

set( HPDF_FOUND OFF )

find_path( HPDF_INCLUDE_DIR hpdf.h
  HINTS
  $ENV{HPDFDIR}
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

#message( "HPDF_INCLUDE_DIR is ${HPDF_INCLUDE_DIR}" )

find_library( HPDF_LIBRARY
  NAMES hpdf
  HINTS
  $ENV{HPDFDIR}
  PATH_SUFFIXES lib64 lib bin
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

#message( "HPDF_LIBRARY is ${HPDF_LIBRARY}" )

if(HPDF_INCLUDE_DIR)
    if(HPDF_LIBRARY)
	set( HPDF_FOUND "YES" )
    endif()
endif()


if(HPDF_FOUND)
    message(STATUS "looking for libhpdf-dev - done")
else()
    message(FATAL_ERROR "ERROR: libhpdf-dev not installed")
endif()
