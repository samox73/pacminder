cmake_minimum_required(VERSION 3.16)

# ========= GLOBAL CONFIGURATIONS =========
# -----------------------------------------

# set the project name
project(PacMinder VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Wshadow -Woverloaded-virtual")
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Boost COMPONENTS filesystem REQUIRED)

include_directories(
  ${Boost_INCLUDE_DIRS}
)

set(INCL_LIBS
  ${Boost_LIBRARIES}
)

add_executable(pacminder pacminder.cpp)

target_link_libraries(pacminder LINK_PUBLIC ${Boost_LIBRARIES}
  config++)

install(TARGETS pacminder
  DESTINATION /usr/bin
  )