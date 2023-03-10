cmake_minimum_required(VERSION 3.22)
project(networking)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(server)
add_subdirectory(client)
target_include_directories(${PROJECT_NAME} PUBLIC server client)
target_link_directories(${PROJECT_NAME} PRIVATE server client)
target_link_libraries(${PROJECT_NAME}  server client)