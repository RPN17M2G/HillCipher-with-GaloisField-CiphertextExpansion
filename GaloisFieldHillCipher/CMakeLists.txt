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
  "src/EncryptionMatrixGeneration.cpp"
  "include/EncryptionMatrixGeneration.h"
  "include/ExtendedHillCipher.h"
  "include/StatusCodes.h"
  "include/MathUtils.h"
  "include/CipherUtils.h"
 "src/CipherUtils.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GaloisFieldHillCipher PROPERTY CXX_STANDARD 20)
endif()

# Specify the path to the sodium library
#set(SODIUM_INCLUDE_DIR "path/to/sodium/include")
#set(SODIUM_LIBRARY_DIR "path/to/sodium/lib")

# Include the sodium headers
#include_directories(${SODIUM_INCLUDE_DIR})

# Link the sodium library
#link_directories(${SODIUM_LIBRARY_DIR})
#target_link_libraries(GaloisFieldHillCipher sodium)

# TODO: Add tests and install targets if needed.

