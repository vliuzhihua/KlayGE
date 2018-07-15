ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)

SET(BOOST_ROOT "${KLAYGE_ROOT_DIR}/External/boost")
SET(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib/${KLAYGE_PLATFORM_NAME}" "${KLAYGE_ROOT_DIR}/KlayGE/bin/${KLAYGE_PLATFORM_NAME}")
SET(Boost_ADDITIONAL_VERSIONS "1.66.0")
IF(KLAYGE_IS_DEV_PLATFORM)
	SET(BOOST_COMPONENTS program_options)
ELSE()
	SET(BOOST_COMPONENTS "")
ENDIF()
SET(BOOST_NO_SYSTEM_PATHS ON)
SET(Boost_COMPILER "-${KLAYGE_COMPILER_NAME}${KLAYGE_COMPILER_VERSION}")
IF(NOT MSVC)
	IF(ANDROID OR IOS)
		SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} filesystem system)
		SET(Boost_USE_STATIC_LIBS ON)
		SET(Boost_USE_STATIC_RUNTIME ON)
		SET(Boost_THREADAPI "pthread")
	ELSE()
		IF(NOT KLAYGE_COMPILER_GCC)
			SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} filesystem system)
		ENDIF()
	ENDIF()
ENDIF()
FIND_PACKAGE(Boost COMPONENTS ${BOOST_COMPONENTS})

IF(NOT Boost_LIBRARY_DIR)
	SET(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIRS})
ENDIF()

IF(KLAYGE_COMPILER_MSVC)
	IF(KLAYGE_PLATFORM_WINDOWS_STORE)
		SET(KLAYGE_FILESYSTEM_LIBRARY "kernel32")
	ELSE()
		SET(KLAYGE_FILESYSTEM_LIBRARY "")
	ENDIF()
ELSE()
	IF(KLAYGE_COMPILER_GCC)
		SET(KLAYGE_FILESYSTEM_LIBRARY "stdc++fs")
	ELSE()
		SET(KLAYGE_FILESYSTEM_LIBRARY ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
	ENDIF()
ENDIF()
