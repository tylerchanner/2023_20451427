cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

if (WIN32)
  set(CPACK_GENERATOR "NSIS")
else()
  set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)

add_library(maths SHARED adder.cpp)
add_executable(calc calc.cpp)
target_link_libraries(calc maths)

# Specify the install rules
install(TARGETS calc maths
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
