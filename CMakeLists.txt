cmake_minimum_required(VERSION 3.20)
project(TSP_Optimization VERSION 1.0)

# # find_package(PackageName REQUIRED)
# # find_package(Boost COMPONENTS filesystem REQUIRED)
# # include_directories( ${PackageName_INCLUDE_DIRS} )
# # include_directories(${Boost_INCLUDE_DIRS})

# Add the include directory containing vrp.h
include_directories(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Add source files
add_executable(TSP_Optimization
    src/main.c
    src/algoSelector.c
    src/Algorithm/NN.c
    src/parser.c
    src/tsp.c
    src/Algorithm/optimizations.c
)

# target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_FILESYSTEM_LIBRARY})

include(CPack)