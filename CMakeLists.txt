cmake_minimum_required(VERSION 3.20)
project(TSP_Optimization VERSION 1.0)

# # find_package(PackageName REQUIRED)

include_directories(/opt/ibm/ILOG/CPLEX_Studio2211/cplex/include)
# # find_package(Boost COMPONENTS filesystem REQUIRED)
# # include_directories( ${PackageName_INCLUDE_DIRS} )
# # include_directories(${Boost_INCLUDE_DIRS})


# Add the include directory containing vrp.h
include_directories(include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

set(CPLEXDIR /Applications/CPLEX_Studio2211/cplex)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I. -I${CPLEXDIR}/include/ilcplex")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${CPLEXDIR}/lib/arm64_osx/static_pic -L. -lcplex -lpthread -ldl")

# Add source files
add_executable(TSP_Optimization
    src/main.c
    src/algoSelector.c
    src/Algorithm/NN.c
    src/parser.c
    src/tsp.c
    src/Algorithm/optimizations.c
    src/menu.c
)

# target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(TSP_Optimization PRIVATE ${CPLEX_LIBRARIES} dl m)
include(CPack)