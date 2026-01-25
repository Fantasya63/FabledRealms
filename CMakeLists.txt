cmake_minimum_required(VERSION 3.31)
project(FabledRealms)

add_executable(FabledRealms)


set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory("FabledRealms/vendor/GLFW")
add_subdirectory("FabledRealms/vendor/GLad")

target_include_directories(FabledRealms
   PRIVATE
    "${GLFW_SOURCE_DIR}/include"
    "${glad_SOURCE_DIR}/include"
)

target_sources(FabledRealms
  PRIVATE
)
