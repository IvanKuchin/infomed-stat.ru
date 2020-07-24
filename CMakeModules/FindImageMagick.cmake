# Locate IMAGEMAGICK library
# This module defines
# IMAGEMAGICK_LIBRARY, the name of the library to link against
# IMAGEMAGICK_FOUND, if false, do not try to link to AVCODEC
# IMAGEMAGICK_INCLUDE_DIR, where to find avcodec.h
#

message(STATUS "looking for ImageMagick++-dev")

set( IMAGEMAGICK_FOUND OFF )

find_path( IMAGEMAGICK_INCLUDE_DIR Magick++.h
  HINTS
  $ENV{IMAGEMAGICKDIR}
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

#message( "IMAGEMAGICK_INCLUDE_DIR is ${IMAGEMAGICK_INCLUDE_DIR}" )

find_library( IMAGEMAGICK_LIBRARY
  NAMES Magick++
  HINTS
  $ENV{IMAGEMAGICKDIR}
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

#message( "IMAGEMAGICK_LIBRARY is ${IMAGEMAGICK_LIBRARY}" )

if(IMAGEMAGICK_INCLUDE_DIR)
    if(IMAGEMAGICK_LIBRARY)
	set( IMAGEMAGICK_FOUND "YES" )
    endif()
endif()


if(IMAGEMAGICK_FOUND)
    message(STATUS "looking for ImageMagick++-dev - done")
else()
    message(FATAL_ERROR "ERROR: libgraphicsmagick++-dev not installed")
endif()
