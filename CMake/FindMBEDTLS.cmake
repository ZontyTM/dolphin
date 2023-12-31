find_path(MBEDTLS_INCLUDE_DIR mbedtls/ssl.h PATH_SUFFIXES mbedtls2)

find_library(MBEDTLS_LIBRARY mbedtls PATH_SUFFIXES mbedtls2)
find_library(MBEDX509_LIBRARY mbedx509 PATH_SUFFIXES mbedtls2)
find_library(MBEDCRYPTO_LIBRARY mbedcrypto PATH_SUFFIXES mbedtls2)

set(MBEDTLS_INCLUDE_DIRS ${MBEDTLS_INCLUDE_DIR})
set(MBEDTLS_LIBRARIES ${MBEDTLS_LIBRARY} ${MBEDX509_LIBRARY} ${MBEDCRYPTO_LIBRARY})

if(NOT MBEDTLS_INCLUDE_DIR STREQUAL "MBEDTLS_INCLUDE_DIR-NOTFOUND")
	if(EXISTS ${MBEDTLS_INCLUDE_DIR}/mbedtls/build_info.h)
	    file(STRINGS ${MBEDTLS_INCLUDE_DIR}/mbedtls/build_info.h MBEDTLS_VERSION_STR REGEX "^#define[ \t]+MBEDTLS_VERSION_STRING[\t ].*")
	else()
	    file(STRINGS ${MBEDTLS_INCLUDE_DIR}/mbedtls/version.h MBEDTLS_VERSION_STR REGEX "^#define[ \t]+MBEDTLS_VERSION_STRING[\t ].*")
	endif()
	string(REGEX REPLACE "^#define[\t ]+MBEDTLS_VERSION_STRING[\t ]+\"([.0-9]+)\".*" "\\1" MBEDTLS_VERSION ${MBEDTLS_VERSION_STR})
endif()

if(NOT MBEDTLS_INCLUDE_DIR STREQUAL "MBEDTLS_INCLUDE_DIR-NOTFOUND" AND MBEDTLS_VERSION VERSION_GREATER_EQUAL 3)
	# Once CMake 3.19 is required, we can enable HANDLE_VERSION_RANGE and use that
	if(MBEDTLS_FIND_REQUIRED)
		set(type FATAL_ERROR)
	else()
		set(type STATUS)
	endif()
	if(MBEDTLS_FIND_REQUIRED OR NOT MBEDTLS_FIND_QUIETLY)
		message(${type} "Could NOT find MBEDTLS: Found unsuitable version \"${MBEDTLS_VERSION}\", but a 2.x version is required (found ${MBEDTLS_INCLUDE_DIR})")
	endif()
	set(MBEDTLS_FOUND FALSE)
else()
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(MBEDTLS
		REQUIRED_VARS MBEDTLS_INCLUDE_DIR MBEDTLS_LIBRARY MBEDX509_LIBRARY MBEDCRYPTO_LIBRARY
		VERSION_VAR MBEDTLS_VERSION)
endif()

mark_as_advanced(MBEDTLS_INCLUDE_DIR MBEDTLS_LIBRARY MBEDX509_LIBRARY MBEDCRYPTO_LIBRARY)

if(MBEDTLS_FOUND)
	add_library(MbedTLS::mbedcrypto UNKNOWN IMPORTED)
	set_target_properties(MbedTLS::mbedcrypto PROPERTIES
	    IMPORTED_LOCATION "${MBEDCRYPTO_LIBRARY}"
	    INTERFACE_INCLUDE_DIRECTORIES "${MBEDTLS_INCLUDE_DIR}"
	)

	add_library(MbedTLS::mbedx509 UNKNOWN IMPORTED)
	set_target_properties(MbedTLS::mbedx509 PROPERTIES
	    IMPORTED_LOCATION "${MBEDX509_LIBRARY}"
	    INTERFACE_INCLUDE_DIRECTORIES "${MBEDTLS_INCLUDE_DIR}"
	    INTERFACE_LINK_LIBRARIES MbedTLS::mbedcrypto
	)

	add_library(MbedTLS::mbedtls UNKNOWN IMPORTED)
	set_target_properties(MbedTLS::mbedtls PROPERTIES
	    IMPORTED_LOCATION "${MBEDTLS_LIBRARY}"
	    INTERFACE_INCLUDE_DIRECTORIES "${MBEDTLS_INCLUDE_DIR}"
	    INTERFACE_LINK_LIBRARIES MbedTLS::mbedx509
	)
endif()
