cmake_minimum_required(VERSION 3.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
	ncurses
	GIT_REPOSITORY https://github.com/mirror/ncurses.git
	GIT_TAG v6.4
)
FetchContent_MakeAvailable(ncurses)

file(GLOB SRC src/*)

project(minesweeper)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE ncurses form)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
