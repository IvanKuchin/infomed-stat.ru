set(EXECUTABLE_CGIs_with_INVOICE_PD_PI_LIBS
    )
set(EXECUTABLE_CGIs_with_PD_PI_LIBS
    index.cgi
    doctor.cgi
    ajax_anyrole_1.cgi
    system.cgi
    )
set(EXECUTABLE_CGIs_with_PI_LIBS
    cron_daily_pd
    )

# --- Libraries
# ---- list of independent libraries
set(INDEPENDENT_LIBRARIES_SRCs
    )

set(JOINT_LIBRARY_SRCs
    c_company.cpp
    utilities_infomed1.cpp
    utilities_infomed2.cpp
    )


# --- MACROSes start ---

macro(set_pd_lib_properties target_lib proj_src proj_inc_list proj_lib_list)
    add_library                 (${target_lib}      ${proj_src})
    target_include_directories  (${target_lib}      PUBLIC  ${proj_inc_list})
    target_link_libraries       (${target_lib}      ${proj_lib_list})

    set(PROJECT_PD_LIBS         ${PROJECT_PD_LIBS}  ${target_lib})
endmacro()

# --- MACROSes finish ---
GET_DIRECTORY_PROPERTY  (XL_LIBRARY_DIR DIRECTORY       ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION XL_LIBRARY_DIR)

GET_DIRECTORY_PROPERTY  (PROJECT_PI_INCLUDE_DIRECTORIES DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY  (PROJECT_PI_LIBS                DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_PI_LIBS)
GET_DIRECTORY_PROPERTY  (PROJECT_EXTERNAL_LIBS          DIRECTORY ${PROJECT_SOURCE_DIR}/src/pi/ DEFINITION PROJECT_EXTERNAL_LIBS)

set(PROJECT_PD_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/src/pd/include/
    ${PROJECT_SOURCE_DIR}/cyrillic-facets/      # --- mbstowcs 
    )

SET(PROJECT_ALL_INCLUDE_DIRECTORIES ${PROJECT_PI_INCLUDE_DIRECTORIES} ${PROJECT_PD_INCLUDE_DIRECTORIES})

# --- Build libraries
# ---- list of independent libraries
foreach(INDEPENDENT_LIBRARY_SRC ${INDEPENDENT_LIBRARIES_SRCs})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${INDEPENDENT_LIBRARY_SRC})

    set_pd_lib_properties(${file_without_ext} "${file_without_ext}.cpp" "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${project_link_libraries}")
endforeach()

# ---- joint lib
set_pd_lib_properties(__joint_pd "${JOINT_LIBRARY_SRCs}" "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${project_link_libraries}")


# --- MSVC grouping
source_group("Headers Private"  FILES ${HDR_PRIVATE})
source_group("Headers Public"   FILES ${HDR_PUBLIC})
source_group("Sources"          FILES ${SOURCES})
source_group("Resources"        FILES ${RESOURCES})

# --- Link part (2 parts: custom build, normal build)
# ---- 1) Custom build speeding up linking process by linking only required libraries (.o)

# ---- 2) Normal build link with _all_ libraries (.o), therefore change in any .src trigger re-link for all executables
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs_with_PI_LIBS})
    __add_executable_2libs(${EXECUTABLE_CGI} "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs_with_PD_PI_LIBS})
    __add_executable_3libs(${EXECUTABLE_CGI} "${PROJECT_ALL_INCLUDE_DIRECTORIES}" "${PROJECT_PD_LIBS}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()
foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs_with_INVOICE_PD_PI_LIBS})
    __add_executable_4libs(${EXECUTABLE_CGI} "${PROJECT_ALL_INCLUDE_DIRECTORIES}"  "${PROJECT_PD_LIBS}" "${PROJECT_PI_LIBS}" "${PROJECT_EXTERNAL_LIBS}")
endforeach()


# --- Install settings
# --- RPATH customization used only for libxl. LibXL didn't installed like any other OS-libraries
# --- it is downloaded from web-site and placed to src-tree. 
# --- During "cmake .." it is copied to /usr/local/share/... and then RPATH customized.
# ---"
# --- this setting allows to dynamic linker find .so library in custom location
# --- otherwise it have to be added to ld.so.conf or environment variable
# --- actual linker options are -Wl,-rpath,/path/to/dynamic/library
# --- https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/RPATH-handling
# set         (CMAKE_INSTALL_RPATH    ${XL_LIBRARY_DIR})
#foreach(EXECUTABLE_CGI ${EXECUTABLE_CGIs_with_INVOICE_PD_PI_LIBS})
#    set_target_properties(${EXECUTABLE_CGI}     PROPERTIES
#                                                BUILD_WITH_INSTALL_RPATH TRUE
#                                                INSTALL_RPATH_USE_LINK_PATH TRUE
#                                                INSTALL_RPATH "${XL_LIBRARY_DIR}"
#                                                )
#endforeach()
#message(STATUS  "configure libxl RPATH to ${XL_LIBRARY_DIR}}")

# --- Install part

install(TARGETS index.cgi			DESTINATION		${CGIDIR} COMPONENT app)
install(TARGETS ajax_anyrole_1.cgi	DESTINATION		${CGIDIR} COMPONENT app)
install(TARGETS system.cgi          DESTINATION     ${CGIDIR} COMPONENT app)
install(TARGETS doctor.cgi          DESTINATION     ${CGIDIR} COMPONENT app)

install(TARGETS cron_daily_pd		DESTINATION		${CRON_INSTALL_DIR} COMPONENT cron)

# print_all_variables()
