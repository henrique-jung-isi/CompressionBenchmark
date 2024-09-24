cmake_minimum_required(VERSION 3.16)

project(CompressionBenchmark LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_standard_project_setup()

qt_add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
)

install(TARGETS ${CMAKE_PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
