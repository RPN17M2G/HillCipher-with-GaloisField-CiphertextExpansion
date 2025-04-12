# CMakeList.txt : CMake project for GaloisFieldHillCipher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

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

add_library(sodium INTERFACE thirdparty/sodium/libsodium.lib)
target_link_libraries(GaloisFieldHillCipher INTERFACE sodium)

target_include_directories(GaloisFieldHillCipher PRIVATE 
  include
  thirdparty/sodium/include
)