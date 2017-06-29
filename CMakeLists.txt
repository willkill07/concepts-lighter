cmake_minimum_required(VERSION 3.3)
project(ConceptsLighter LANGUAGES CXX)

# Option(s)
option(ENABLE_EXAMPLES "Build simple examples" On)

# Header-only library
add_library(concepts-lighter INTERFACE)
target_include_directories(concepts-lighter INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/>
)

# Installation
install(TARGETS concepts-lighter EXPORT ConceptsLighterConfig INCLUDES DESTINATION include)

install(EXPORT ConceptsLighterConfig DESTINATION lib/cmake/concepts-lighter)

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN *.hpp)

# Example
if(ENABLE_EXAMPLES)
  add_executable(example example.cpp)
  set_property(TARGET example PROPERTY CXX_STANDARD 11)
  target_link_libraries(example concepts-lighter)

  enable_testing()
  add_test(NAME test_example COMMAND example)
endif()
