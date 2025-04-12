# CMakeList.txt : CMake project for GaloisFieldHillCipher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GaloisFieldHillCipher" LANGUAGES C)

FILE(GLOB_RECURSE SOURCES src/*.c GaloisFieldHillCipher.c)
FILE(GLOB_RECURSE INCLUDES include/*.h
	thirdparty/sodium/include/*.h)

add_executable (GaloisFieldHillCipher  
	${SOURCES}
	${INCLUDES})

# Sodium build
include(FetchContent)

# Update the commit to point to whatever libsodium-cmake-commit you want to target.
FetchContent_Declare(Sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
    GIT_TAG 99f14233eab1d4f7f49c2af4ec836f2e701c445e # HEAD as of 2022-05-28
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

target_link_libraries(GaloisFieldHillCipher PRIVATE sodium)

target_include_directories(GaloisFieldHillCipher PRIVATE 
  include
)