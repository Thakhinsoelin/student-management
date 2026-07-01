# 1. Collect all .cpp files in this folder
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.c
    *.h
    *.hpp
)

# 2. Define the executable (using the project name from root)
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/$<CONFIG>-$<PLATFORM_ID>"
)
# 3. Include directories
# We use PARENT_SCOPE or absolute paths to find the Dependencies folder
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/Dependencies"
    "${CMAKE_SOURCE_DIR}/src"
)

# Get the path to swagger resources from the library

# Add it as a definition so the C++ code can see it
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    OATPP_SWAGGER_RES_PATH="${CMAKE_SOURCE_DIR}/Dependencies/oatpp-swagger/res"
)
# 4. Link everything
# The 'glfw' and 'Vulkan::Vulkan' targets are globally available 
# because they were defined in the root or via find_package.
if(MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        oatpp
        oatpp-swagger
        oatpp-postgresql
        bcrypt
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        oatpp
        oatpp-swagger
        oatpp-postgresql
        PostgreSQL::PostgreSQL
        bcrypt
    )
endif()

if(WIN32)
    # Mandatorily link the Windows socket layer for network transactions
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 wsock32)

    # CRITICAL MIN GW FIX: Bake the GNU runtimes directly inside the binary.
    # Without this, launching the .exe outside your terminal will crash instantly
    # with a popup saying "libstdc++-6.dll missing".
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${PROJECT_NAME} PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
    endif()
endif()

