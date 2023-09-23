cmake_minimum_required(VERSION 3.14)

project(FMHD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)

file(GLOB_RECURSE src CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)

add_compile_options(-O2 -fopenmp -msse4.2 -mavx)
add_library(minhash_lib ${src})
target_include_directories(minhash_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
set_target_properties(minhash_lib PROPERTIES OUTPUT_NAME "minhash")

add_executable(minhash_executable
  src/main.cc
)

target_link_libraries(minhash_executable
  PUBLIC minhash_lib
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(minhash_executable 
      PUBLIC OpenMP::OpenMP_CXX
    )
endif()