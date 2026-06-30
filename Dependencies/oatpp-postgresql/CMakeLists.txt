cmake_minimum_required(VERSION 3.20)

###################################################################################################
## These variables are passed to oatpp-module-install.cmake script
## use these variables to configure module installation

set(OATPP_THIS_MODULE_NAME oatpp-postgresql) ## name of the module (also name of folders in installation dirs)
set(OATPP_THIS_MODULE_VERSION "1.4.0") ## version of the module (also sufix of folders in installation dirs)
set(OATPP_THIS_MODULE_LIBRARIES oatpp-postgresql) ## list of libraries to find when find_package is called
set(OATPP_THIS_MODULE_TARGETS oatpp-postgresql) ## list of targets to install
set(OATPP_THIS_MODULE_DIRECTORIES oatpp-postgresql) ## list of directories to install

###################################################################################################

project(${OATPP_THIS_MODULE_NAME}
        VERSION ${OATPP_THIS_MODULE_VERSION}
        LANGUAGES CXX
        ## HOMEPAGE_URL "https://github.com/oatpp/oatpp-postgresql"
        ## DESCRIPTION "Something about postgresql"
)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(OATPP_DIR_SRC "Path to oatpp module directory (sources)")
option(OATPP_DIR_LIB "Path to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib)")
option(OATPP_BUILD_TESTS "Build tests for this module" ON)
option(OATPP_INSTALL "Install module binaries" ON)

set(OATPP_MODULES_LOCATION "INSTALLED" CACHE STRING "Location where to find oatpp modules. can be [INSTALLED|EXTERNAL|CUSTOM]")

###################################################################################################
## get oatpp main module in specified location

set(OATPP_MODULES_LOCATION_INSTALLED INSTALLED)
set(OATPP_MODULES_LOCATION_EXTERNAL EXTERNAL)
set(OATPP_MODULES_LOCATION_CUSTOM CUSTOM)

# Force oatpp-postgresql to use your local vendored copy of oatpp
message("Using local vendored Oat++ paths directly")
set(OATPP_INCLUDE "${CMAKE_SOURCE_DIR}/Dependencies/oatpp/src")

set(OATPP_TEST_INCLUDE "${CMAKE_SOURCE_DIR}/Dependencies/oatpp/src")
if(OATPP_DIR_LIB)
    link_directories(${OATPP_DIR_LIB})
endif()

###################################################################################################
## get dependencies

message("\n############################################################################")
message("## ${OATPP_THIS_MODULE_NAME} module. Resolving dependencies...\n")

set(PostgreSQL_ADDITIONAL_VERSIONS "17")

find_package(PostgreSQL REQUIRED)

message("PostgreSQL_INCLUDE_DIRS=${PostgreSQL_INCLUDE_DIRS}")
message("PostgreSQL_LIBRARIES=${PostgreSQL_LIBRARIES}")
message("PostgreSQL_TYPE_INCLUDE_DIR=${PostgreSQL_TYPE_INCLUDE_DIR}")
message("PostgreSQL_VERSION_STRING=${PostgreSQL_VERSION_STRING}")

message("\n############################################################################\n")

###################################################################################################
## define targets

include(cmake/module-utils.cmake)

add_subdirectory("src")

if(OATPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()
