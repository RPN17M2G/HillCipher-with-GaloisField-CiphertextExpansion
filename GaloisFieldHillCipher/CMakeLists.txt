cmake_minimum_required(VERSION 3.11)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("GaloisFieldHillCipher" LANGUAGES C)

file(GLOB_RECURSE SOURCES src/*.c GaloisFieldHillCipher.c)
file(GLOB_RECURSE SOURCES_WITHOUT_MAIN src/*.c)
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE TESTS_SOURCES tests/*.c)
file(GLOB_RECURSE TESTS_INCLUDES tests/*.h)

add_executable(GaloisFieldHillCipher
        ${SOURCES}
        ${INCLUDES}
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
)

add_executable(UnitTests
        ${TESTS_SOURCES}
        ${TESTS_INCLUDES}
        ${SOURCES_WITHOUT_MAIN}
        ${INCLUDES}
)

target_include_directories(UnitTests PRIVATE
        include
        thirdparty/Unity/src
        thirdparty/argparse
        thirdparty/sodium/include
)
target_link_libraries(UnitTests PRIVATE unity)
target_link_libraries(UnitTests PRIVATE sodium)
target_link_libraries(UnitTests PRIVATE argparse_static)

enable_testing()
add_test(NAME AllUnitTests COMMAND UnitTests)
