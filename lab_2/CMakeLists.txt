cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

project(lab)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror -O0")


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

# --- END DEPENDENCIES ---