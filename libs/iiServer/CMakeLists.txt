cmake_minimum_required(VERSION 3.21)
project(iiServer LANGUAGES CXX VERSION 0.0.9 DESCRIPTION "siisty one thread iiServer implementation")

#####################
#  finding sources  #
#####################

set(SRCS "")

# Sources
file(GLOB_RECURSE SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.hpp")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

#########################
#  building executable  #
#########################

qt6_add_resources(SRCS ${GLOBAL_RESOURCES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${PROJECT_NAME}
        SHARED
        ${SRCS}
    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME}
            INTERFACE
            ${SRCS}
        )
    else()
        add_library(${PROJECT_NAME}
            MODULE
            ${SRCS}
        )
    endif()
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Database
        Logger
        Network
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_target(${PROJECT_NAME})
endif()

################################
#  setting project properties  #
################################

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME    "${PROJECT_NAME}"
    SUFFIX         ""
    PREFIX         ""
    VERSION        "${PROJECT_VERSION}"
    PUBLIC_HEADER  "${HDRS_G}"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# install(TARGETS ${PROJECT_NAME}
    # LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}
    # PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/lib/include
    # PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/lib/include
# )
