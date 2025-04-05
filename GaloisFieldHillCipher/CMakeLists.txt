# CMakeList.txt : CMake project for GaloisFieldHillCipher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GaloisFieldHillCipher")

include_directories(include)

# Add source to this project's executable.
add_executable (GaloisFieldHillCipher 
  "GaloisFieldHillCipher.cpp" 
  "include/ExtendedHillCipher.h"
  "include/StatusCodes.h"
  "include/MathUtils.h"
  "include/CipherUtils.h"
 "src/CipherUtils.cpp" 
 "src/ExtendedHillCipher.cpp"
"src/MathUtils.cpp")

