# Once done these will be defined:
#
#  XMLY_FOUND
#  XMLY_INCLUDE_DIRS
#  XMLY_LIBRARIES
#
# For use in OBS:
#
#  XMLY_INCLUDE_DIR

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(_XMLY QUIET XmlySignalingClient)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_path(XMLY_INCLUDE_DIR
	NAMES XMSignalingClientApi.h XMSignalingClientType.h XMSignalingError.h XMSignalingMsgTypes.h
	HINTS
		ENV xmlyPath${_lib_suffix}
		ENV xmlyPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${xmlyPath${_lib_suffix}}
		${xmlyPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_XMLY_INCLUDE_DIRS}
	PATHS
		/usr/include /usr/local/include /opt/local/include /sw/include
	PATH_SUFFIXES
		include)

find_library(XMLY_LIB
	NAMES ${_XMLY_LIBRARIES} XmlySignalingClient
	HINTS
		ENV xmlyPath${_lib_suffix}
		ENV xmlyPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${xmlyPath${_lib_suffix}}
		${xmlyPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_XMLY_LIBRARY_DIRS}
	PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XMLY DEFAULT_MSG XMLY_LIB XMLY_INCLUDE_DIR)
mark_as_advanced(XMLY_INCLUDE_DIR XMLY_LIB)

if(XMLY_FOUND)
	set(XMLY_INCLUDE_DIRS ${XMLY_INCLUDE_DIR})
	set(XMLY_LIBRARIES ${XMLY_LIB})
endif()
