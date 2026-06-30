set(TEST_NAME Tests)
# 1. Collect all .cpp files in this folder
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.c
)

# 2. Define the executable (using the project name from root)
add_executable(${TEST_NAME} ${SOURCES})
set_target_properties(${TEST_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${TEST_NAME}/$<CONFIG>-$<PLATFORM_ID>"
)
# 3. Include directories
# We use PARENT_SCOPE or absolute paths to find the Dependencies folder
target_include_directories(${TEST_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/Dependencies"
    "${CMAKE_SOURCE_DIR}/src"
)

# 4. Link everything
# The 'glfw' and 'Vulkan::Vulkan' targets are globally available 
# because they were defined in the root or via find_package.
target_link_libraries(${TEST_NAME} PRIVATE 
    oatpp
    oatpp-swagger
    bcrypt
)

if(WIN32)
    # Mandatorily link the Windows socket layer for network transactions
    target_link_libraries(${TEST_NAME} PRIVATE ws2_32 wsock32)

    # CRITICAL MIN GW FIX: Bake the GNU runtimes directly inside the binary.
    # Without this, launching the .exe outside your terminal will crash instantly
    # with a popup saying "libstdc++-6.dll missing".
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${TEST_NAME} PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
    endif()
endif()

