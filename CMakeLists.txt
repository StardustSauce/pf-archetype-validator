# Declare the minimum required CMake version
cmake_minimum_required(VERSION 3.7)

# Name the project
project(validator)

# Define a variable containing a list of source files for the project
set(
  PROG_SRCS
  resources/icons/appicon.rc
  resources/icons/d20-Normal.xpm
  source/main.cpp
  source/main-window.cpp
  source/archetype-table.cpp
  source/archetype.cpp
  source/pf-class.cpp
)

# Request the required wxWidgets libs
find_package(wxWidgets 3.1 COMPONENTS core base REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})
# set(OpenCV_DIR "<cusompath>" CACHE PATH '' ${SHOULD_FORCE_CACHE})
# find_package(OpenCV REQUIRED CONFIG)

# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 ${PROG_SRCS})

# Link required libraries to the executable
if(WIN32)
  target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} -static)
else()
  target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
endif()


install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
