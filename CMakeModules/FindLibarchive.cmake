# Locate HPDF library
# This module defines
# HPDF_LIBRARY, the name of the library to link against
# HPDF_FOUND, if false, do not try to link to HPDF
# HPDF_INCLUDE_DIR, where to find hpdf.h
#

message(STATUS "looking for libarchive-dev")

set( ARCHIVE_FOUND OFF )

find_path( ARCHIVE_INCLUDE_DIR archive.h
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

find_path( ARCHIVE_INCLUDE_DIR archive_entry.h
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

find_library( ARCHIVE_LIBRARY
  NAMES archive
  HINTS
  $ENV{ARCHIVEDIR}
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

#message( "ARCHIVE_LIBRARY is ${ARCHIVE_LIBRARY}" )

if(ARCHIVE_INCLUDE_DIR)
    if(ARCHIVE_LIBRARY)
	set( ARCHIVE_FOUND "YES" )
    endif()
endif()


if(ARCHIVE_FOUND)
    message(STATUS "looking for libarchive-dev - done")
else()
    message(FATAL_ERROR "ERROR: libarchive-dev not installed")
endif()
