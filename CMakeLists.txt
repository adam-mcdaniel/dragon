cmake_minimum_required(VERSION 3.10)
project(dragon VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")
# set(CMAKE_INSTALL_PREFIX "../install")

add_library(dragon STATIC
    src/function.cpp
    src/option.cpp
    src/machine.cpp
)

target_compile_features(dragon
    PUBLIC
      cxx_attributes
      cxx_defaulted_functions
      cxx_deleted_functions
      cxx_final
)

target_include_directories(dragon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(dragon PUBLIC __STDC_LIMIT_MACROS __STDC_FORMAT_MACROS)



install(TARGETS dragon DESTINATION lib)
install(FILES src/dragon.hpp src/function.hpp src/machine.hpp src/option.hpp DESTINATION include/dragon)


export(PACKAGE dragon)