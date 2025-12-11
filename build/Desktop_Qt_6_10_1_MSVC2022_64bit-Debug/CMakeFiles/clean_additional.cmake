# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\OpenEnded_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\OpenEnded_autogen.dir\\ParseCache.txt"
  "OpenEnded_autogen"
  )
endif()
