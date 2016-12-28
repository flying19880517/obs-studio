# Once done these will be defined:
#
#  AGORARTC_FOUND
#  AGORARTC_INCLUDE_DIRS
#  AGORARTC_LIBRARIES
#
# For use in OBS:
#
#  AGORARTC_INCLUDE_DIR

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(_AGORARTC QUIET agorartc AgoraRTCEngine)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_path(AGORARTC_INCLUDE_DIR
	NAMES IAgoraRtcEngine.h IAgoraMediaEngine.h
	HINTS
		ENV agorartcPath${_lib_suffix}
		ENV agorartcPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${agorartcPath${_lib_suffix}}
		${agorartcPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_AGORARTC_INCLUDE_DIRS}
	PATHS
		/usr/include /usr/local/include /opt/local/include /sw/include
	PATH_SUFFIXES
		include)

find_library(AGORARTC_LIB
	NAMES ${_AGORARTC_LIBRARIES} agorartc AgoraRTCEngine
	HINTS
		ENV agorartcPath${_lib_suffix}
		ENV agorartcPath
		ENV DepsPath${_lib_suffix}
		ENV DepsPath
		${agorartcPath${_lib_suffix}}
		${agorartcPath}
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_AGORARTC_LIBRARY_DIRS}
	PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin
		"build/Win${_lib_suffix}/VC12/DLL Release - DLL Windows SSPI"
		"../build/Win${_lib_suffix}/VC12/DLL Release - DLL Windows SSPI")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AgoraRTC DEFAULT_MSG AGORARTC_LIB AGORARTC_INCLUDE_DIR)
mark_as_advanced(AGORARTC_INCLUDE_DIR AGORARTC_LIB)

if(AGORARTC_FOUND)
	set(AGORARTC_INCLUDE_DIRS ${AGORARTC_INCLUDE_DIR})
	set(AGORARTC_LIBRARIES ${AGORARTC_LIB})
endif()
