cmake_minimum_required(VERSION 3.18)
project(mst)

find_package(igraph REQUIRED)
find_package(MPI REQUIRED)

set(CMAKE_C_COMPILER mpicc)

file(GLOB SOURCES src/*.c)

add_executable(mst ${SOURCES})
target_link_libraries(mst PUBLIC igraph::igraph)
target_link_libraries(mst PRIVATE ${MPI_C_LIBRARIES})

target_compile_options(mst PRIVATE -Wall -Wextra -lm)

set(NP 1 CACHE STRING "Number of processes for MPI")
set(INPUT "../graph.txt" CACHE STRING "Input file")
set(OUTPUT "../mst.txt" CACHE STRING "Output file")

add_custom_target(run
    COMMAND mpirun -np ${NP} ./mst ${INPUT} ${OUTPUT}
    DEPENDS mst
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
