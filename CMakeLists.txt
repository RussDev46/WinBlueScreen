cmake_minimum_required(VERSION 3.24)

# Project WinBlueScreen	
project(WinBlueScreen VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")


# exe
set(SOURCE_FILES src/main.cpp src/winGUI.cpp src/blueScreen.cpp)
set(RESOURCE_FILES res/WinBlueScreen.rc res/qr.bmp res/WinBlueScreenIcon.ico )


add_executable(WinBlueScreen ${SOURCE_FILES} ${RESOURCE_FILES})
set_target_properties(WinBlueScreen PROPERTIES RESOURCE "${RESOURCE_FILES}")
