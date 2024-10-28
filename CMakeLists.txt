cmake_minimum_required(VERSION 3.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
	ncurses
	GIT_REPOSITORY https://github.com/mirror/ncurses.git
	GIT_TAG v6.4
)
FetchContent_MakeAvailable(ncurses)

project(minesweeper)
file(GLOB SRCS src/*)
file(GLOB INCS include/*)
add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_sources(${PROJECT_NAME} PUBLIC ${SRCS} ${INCS})
target_link_libraries(${PROJECT_NAME} PRIVATE ncurses)
