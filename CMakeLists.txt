cmake_minimum_required(VERSION 3.23)
project(LogicAPI)

set(CMAKE_CXX_STANDARD 23)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS  "-static")

#set(CMAKE_TOOLCHAIN_FILE "C:/Programming/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(Crow_DIR "C:/Programming/vcpkg/installed/x64-windows/share/crow")
set(Boost_INCLUDE_DIR "C:/Program Files (x86)/boost/lib/cmake/Boost-1.81.0")

# ...somewhere after the call to project

#include_directories("C:/Programming/vcpkg/installed/x64-windows/include")
#link_directories("C:/Programming/vcpkg/installed/x64-windows/lib")

find_package(Threads REQUIRED)
#find_package(Boost COMPONENTS system REQUIRED)
find_package(Crow CONFIG REQUIRED)

find_package(ZLIB)
find_package(OpenSSL)

if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (MSVC)
    set(Boost_USE_STATIC_LIBS "On")
    find_package( Boost 1.70 COMPONENTS system thread regex REQUIRED )
else()
    find_package( Boost 1.70 COMPONENTS system thread REQUIRED )
endif()

include_directories(${Boost_INCLUDE_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories("${PROJECT_INCLUDE_DIR}")

add_executable(LogicAPI main.cpp Source/ExpressionSymbol.cpp Source/LogicCalculator.cpp Source/Operation.cpp Source/Variable.cpp Headers/ExpressionSymbol.h Headers/LogicCalculator.h Headers/Operation.h Headers/Variable.h)
#${Boost_LIBRARIES}
target_link_libraries(LogicAPI PUBLIC Threads::Threads Crow::Crow setupapi.lib wsock32.lib ws2_32.lib)
