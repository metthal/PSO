cmake_minimum_required(VERSION 2.8)
project(PSO CXX)

if (WIN32)
	link_directories("${PROJECT_SOURCE_DIR}/SFML/link")
    set(SFML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/SFML/include")
	set(SFML_LIBRARIES "sfml-main" "sfml-system" "sfml-window" "sfml-graphics")
else()
    find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
endif()

add_subdirectory(src)
