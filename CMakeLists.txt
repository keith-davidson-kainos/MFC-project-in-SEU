cmake_minimum_required(VERSION 3.20)

project(MFCProjectInSEU_DateConversionTests LANGUAGES CXX)

option(BUILD_DATECONV_TESTS "Build DateConversionCore tests" ON)

add_library(DateConversionCore STATIC
    scr/DateConversionCore/DateConversionCore.cpp
)

target_include_directories(DateConversionCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/scr/DateConversionCore
)

target_compile_features(DateConversionCore PUBLIC cxx_std_17)

if(BUILD_DATECONV_TESTS)
    include(CTest)
    enable_testing()

    find_package(GTest CONFIG QUIET)

    if(NOT GTest_FOUND)
        include(FetchContent)

        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        )

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    add_executable(DateConversionCoreTests
        tests/Calculate_atom.Tests/main.cpp
        tests/Calculate_atom.Tests/DateConversionCoreTests.cpp
        tests/Calculate_atom.Tests/pch.cpp
    )

    target_include_directories(DateConversionCoreTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/Calculate_atom.Tests
    )

    target_link_libraries(DateConversionCoreTests
        PRIVATE
            DateConversionCore
    )

    if(TARGET GTest::gtest)
        target_link_libraries(DateConversionCoreTests PRIVATE GTest::gtest)
    elseif(TARGET gtest)
        target_link_libraries(DateConversionCoreTests PRIVATE gtest)
    else()
        message(FATAL_ERROR "GoogleTest target not found after configuration.")
    endif()

    add_test(NAME DateConversionCoreTests COMMAND DateConversionCoreTests)
endif()
