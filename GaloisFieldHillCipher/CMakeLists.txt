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
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GaloisFieldHillCipher PROPERTY CXX_STANDARD 20)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

# Include the sodium headers
include_directories(${SODIUM_INCLUDE_DIRS})

# Link the sodium library
target_link_libraries(your_target_name ${SODIUM_LIBRARIES})

# Find and link the sodium library
target_link_libraries(GaloisFieldHillCipher thirdparty/sodium)

# TODO: Add tests and install targets if needed.
