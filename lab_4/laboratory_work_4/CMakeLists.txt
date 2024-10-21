cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

project(lab)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -lcurl -O3")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/inc")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies")




# --- START PROJECT ---

file(GLOB source CONFIGURE_DEPENDS 
                "${SRC_DIR}/*.cpp"
)



add_executable(${PROJECT_NAME} ${source})


target_include_directories(${PROJECT_NAME} PUBLIC "${INC_DIR}")

# --- END PROJECT ---

# ==================

# --- START DEPENDENCIES ---

# --- END DEPENDENCIES ---