cmake_minimum_required(VERSION 3.11)

project("GaloisFieldHillCipher" LANGUAGES C)

# Platform-specific debug info for MSVC
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  if (MSVC)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")
  endif()
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.c GaloisFieldHillCipher.c thirdparty/log/src/*.c)
file(GLOB_RECURSE SOURCES_WITHOUT_MAIN src/*.c thirdparty/log/src/*.c)
file(GLOB_RECURSE INCLUDES include/*.h thirdparty/log/src/*.h)
file(GLOB_RECURSE TESTS_SOURCES tests/*.c)
file(GLOB_RECURSE TESTS_INCLUDES tests/*.h)

add_executable(GaloisFieldHillCipher
        ${SOURCES}
)

include(FetchContent)
FetchContent_Declare(Sodium
        GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
        GIT_TAG 99f14233eab1d4f7f49c2af4ec836f2e701c445e
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

target_link_libraries(GaloisFieldHillCipher PRIVATE sodium)

add_subdirectory(thirdparty/argparse)
target_link_libraries(GaloisFieldHillCipher PRIVATE argparse_static)

add_subdirectory(thirdparty/Unity)

target_include_directories(GaloisFieldHillCipher PRIVATE
        include
        thirdparty/argparse
        thirdparty/sodium/include
        thirdparty/log/src
)

add_executable(UnitTests
        ${TESTS_SOURCES}
        ${SOURCES_WITHOUT_MAIN}
)

target_include_directories(UnitTests PRIVATE
        include
        thirdparty/Unity/src
        thirdparty/argparse
        thirdparty/sodium/include
        thirdparty/log/src
)

target_link_libraries(UnitTests PRIVATE
        unity
        sodium
        argparse_static
)

# Link the math library 'm' on non-Windows platforms.
# MSVC on Windows includes this in its default C runtime.
if(NOT MSVC)
  target_link_libraries(UnitTests PRIVATE m)
  target_link_libraries(GaloisFieldHillCipher PRIVATE m)
endif()

enable_testing()
add_test(NAME AllUnitTests COMMAND UnitTests)
