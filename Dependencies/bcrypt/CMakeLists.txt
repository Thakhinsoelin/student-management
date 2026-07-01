# Minimal, Windows-Friendly CMakeLists.txt for bcrypt
project(bcrypt C CXX)

set(SRCFILES
    src/bcrypt.c
    src/crypt_blowfish.c
    src/crypt_gensalt.c
    src/wrapper.c
)

# Only add the ASM file if we are on a platform that supports it
# MSVC handles assembly differently, so it's often safer to exclude x86.S 
# if you aren't sure, as the C code will still work.
if(NOT MSVC)
    list(APPEND SRCFILES src/x86.S)
    enable_language(ASM)
endif()

add_library(bcrypt STATIC ${SRCFILES})

# This makes sure the include folder is found by your main project
#target_include_directories(bcrypt PUBLIC 
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#)

target_include_directories(bcrypt PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Apply your preferred C/C++ settings here
if(MSVC)
    target_compile_definitions(bcrypt PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
