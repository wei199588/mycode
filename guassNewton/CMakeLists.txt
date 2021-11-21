cmake_minimum_required(VERSION 3.5)
project(gaussNewton)

set(CMAKE_CXX_FLAGS "-O1 -falign-functions -falign-jumps")
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src /usr/local/include)
link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} src/main.cpp src/gaussNewton.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

