cmake_minimum_required(VERSION 4.3.3)
set(CMAKE_C_COMPILER "cl.exe" CACHE FILEPATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "cl.exe" CACHE FILEPATH "C++ compiler" FORCE)

set(OATPP_BUILD_TESTS OFF CACHE BOOL "Disable oatpp tests" FORCE)
set(OATPP_BUILD_EXAMPLES OFF CACHE BOOL "Disable oatpp examples" FORCE)

project(StudentManagement LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OATPP_MSVC_LINK_STATIC ON CACHE BOOL "" FORCE)
set(OATPP_MSVC_LINK_STATIC_RUNTIME ON CACHE BOOL "" FORCE) # <-- The real hidden variable

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(MSVC)
    # /MT is the static runtime for Release
    # /MTd is the static runtime for Debug
    add_compile_options(
        $<$<CONFIG:Release>:/MT>
        $<$<CONFIG:Debug>:/MTd>
    )
endif()

set(PostgreSQL_ROOT "D:/Program Files/PostgreSQL/18")
set(PostgreSQL_INCLUDE_DIR "D:/Program Files/PostgreSQL/18/include")
set(PostgreSQL_LIBRARY "D:/Program Files/PostgreSQL/18/lib/libpq.lib")

add_subdirectory(Dependencies/oatpp)
# IMPORTANT: Create the aliases that swagger is searching for
if(NOT TARGET oatpp::oatpp)
    add_library(oatpp::oatpp ALIAS oatpp)
endif()

if(NOT TARGET oatpp::oatpp-test)
    add_library(oatpp::oatpp-test ALIAS oatpp-test)
endif()
add_subdirectory(Dependencies/oatpp-swagger)
add_subdirectory(Dependencies/oatpp-postgresql)
add_subdirectory(Dependencies/bcrypt)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(oatpp PRIVATE 
        "-Wno-undefined-reinterpret-cast" 
        "-Wno-shadow-header" 
        "-Wno-deprecated-declarations"
    )

    get_target_property(OATPP_INTERFACE_LIBS oatpp INTERFACE_LINK_LIBRARIES)
    if(OATPP_INTERFACE_LIBS)
        list(REMOVE_ITEM OATPP_INTERFACE_LIBS "atomic")
        set_target_properties(oatpp PROPERTIES INTERFACE_LINK_LIBRARIES "${OATPP_INTERFACE_LIBS}")
    endif()

    get_target_property(OATPP_PRIVATE_LIBS oatpp LINK_LIBRARIES)
    if(OATPP_PRIVATE_LIBS)
        list(REMOVE_ITEM OATPP_PRIVATE_LIBS "atomic")
        set_target_properties(oatpp PROPERTIES LINK_LIBRARIES "${OATPP_PRIVATE_LIBS}")
    endif()
endif()

set(MY_CUSTOM_BIN_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/$<CONFIG>-$<PLATFORM_ID>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_CUSTOM_BIN_DIR}")

# Force all executable targets to compile into this specific directory
add_subdirectory(src)

# disable for now
# add_subdirectory(test)



