# Locate HPDF library
# This module defines
# HPDF_LIBRARY, the name of the library to link against
# HPDF_FOUND, if false, do not try to link to HPDF
# HPDF_INCLUDE_DIR, where to find hpdf.h
#

message(STATUS "looking for libwkhtmltox")

set( WKHTMLTOX_FOUND OFF )

find_path( WKHTMLTOX_INCLUDE_DIR wkhtmltox/pdf.h
  HINTS
  $ENV{WKHTMLTOXDIR}
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

# message( "WKHTMLTOX_INCLUDE_DIR is ${WKHTMLTOX_INCLUDE_DIR}" )

find_library( WKHTMLTOX_LIBRARY
  NAMES wkhtmltox
  HINTS
  $ENV{WKHTMLTOXDIR}
  PATH_SUFFIXES lib64 lib bin
  PATHS
  /usr/local/lib
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

# message( "WKHTMLTOX_LIBRARY is ${WKHTMLTOX_LIBRARY}" )

if(WKHTMLTOX_INCLUDE_DIR)
    if(WKHTMLTOX_LIBRARY)
	set( WKHTMLTOX_FOUND "YES" )
    endif()
endif()


if(WKHTMLTOX_FOUND)
    message(STATUS "looking for libwkhtmltox - done")
else()
    message(FATAL_ERROR "ERROR: libwkhtmltox not installed")
endif()
