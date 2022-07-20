cmake_minimum_required(VERSION 3.10)
project(BasedCharts)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
if(UNIX AND CMAKE_CXX_CLANG_TIDY)
    # Workaround for https://gitlab.kitware.com/cmake/cmake/issues/19772
    # and https://reviews.llvm.org/D34654
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_SOURCE_DIR}/scripts/compile.sh")
endif()

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wno-unknown-pragmas")
string(APPEND CMAKE_C_FLAGS " -Wall -Wno-unknown-pragmas")

SET(CMAKE_CXX_FLAGS_SANITIZE "${CMAKE_CXX_FLAGS_DEBUG}")
SET(CMAKE_C_FLAGS_SANITIZE "${CMAKE_C_FLAGS_DEBUG}")
SET(CMAKE_EXE_LINKER_FLAGS_SANITIZE "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
SET(CMAKE_SHARED_LINKER_FLAGS_SANITIZE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")

string(APPEND CMAKE_CXX_FLAGS_RELEASE " -DQT_RELEASE")

string(APPEND CMAKE_CXX_FLAGS_SANITIZE " -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all")
string(APPEND CMAKE_CXX_FLAGS_SANITIZE " -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null")
string(APPEND CMAKE_CXX_FLAGS_SANITIZE " -fno-sanitize=alignment")
string(APPEND CMAKE_C_FLAGS_SANITIZE " -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all")
string(APPEND CMAKE_C_FLAGS_SANITIZE " -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null")
string(APPEND CMAKE_C_FLAGS_SANITIZE " -fno-sanitize=alignment")
string(APPEND CMAKE_EXE_LINKER_FLAGS_SANITIZE " -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all")
string(APPEND CMAKE_EXE_LINKER_FLAGS_SANITIZE " -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null")
string(APPEND CMAKE_EXE_LINKER_FLAGS_SANITIZE " -fno-sanitize=alignment")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_SANITIZE " -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_SANITIZE " -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_SANITIZE " -fno-sanitize=alignment")

enable_testing()

add_subdirectory(src)
add_subdirectory(test)