cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

project(lab)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)



set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/inc")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies")




# --- START PROJECT ---


file(GLOB source CONFIGURE_DEPENDS 
                "${SRC_DIR}/*.c"
)



add_executable(${PROJECT_NAME} ${source})


target_include_directories(${PROJECT_NAME} PUBLIC "${INC_DIR}")

# --- END PROJECT ---

# ==================

# --- START DEPENDENCIES ---

# find_package(SDL2 REQUIRED)
# target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

# set(DEPS_DIR "${PROJECT_SOURCE_DIR}/dependencies")

# target_include_directories(${PROJECT_NAME} PUBLIC "${DEPS_DIR}/glad/include/")

# add_subdirectory("${DEPS_DIR}/glfw")
# target_link_libraries(${PROJECT_NAME} "glfw")

# find_package(OpenGL REQUIRED)
# target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# add_library(gladlib STATIC "${DEPS_DIR}/glad/src/gl.c")
# target_include_directories(gladlib PUBLIC "${DEPS_DIR}/glad/include/")
# target_link_libraries(${PROJECT_NAME} gladlib)

# file(GLOB imguisource CONFIGURE_DEPENDS "${SRC_DIR}/vendor/imgui/*.cpp")
# add_library(imguilib STATIC "${imguisource}")
# target_link_libraries(imguilib "glfw")
# target_link_libraries(${PROJECT_NAME} imguilib)

# --- END DEPENDENCIES ---