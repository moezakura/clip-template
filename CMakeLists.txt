cmake_minimum_required(VERSION 3.16)
project(clip-template VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)

# Find X11 for window management
find_package(X11 REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/templatemanager.cpp
    src/configreader.cpp
    src/clipboardhandler.cpp
    src/keyboardhandler.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/templatemanager.h
    src/configreader.h
    src/clipboardhandler.h
    src/keyboardhandler.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${YAML_CPP_INCLUDE_DIRS}
    ${X11_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    ${YAML_CPP_LIBRARIES}
    ${X11_LIBRARIES}
    Xtst
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Install config file
install(FILES config/templates.yaml
    DESTINATION share/${PROJECT_NAME}
)

# Create config directory during build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/config)