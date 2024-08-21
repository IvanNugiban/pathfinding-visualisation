cmake_minimum_required(VERSION 3.16)
project(PathfindingVisualisation LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(BUILD_SHARED_LIBS ON)      # Determines whether TGUI build a static or dynamic/shared library
set(TGUI_BACKEND SFML_GRAPHICS)  # Sets which backend TGUI will use

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
  TGUI
  GIT_REPOSITORY https://github.com/texus/TGUI
  GIT_TAG v1.4.0  
)
FetchContent_MakeAvailable(TGUI)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.rc)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
add_executable(main WIN32 ${WIN32_RESOURCES}  src/main.cpp  "include/window.h" "src/window.cpp" "include/resources.h"  "include/grid.h" "src/grid.cpp" "include/ui.h" "src/ui.cpp" "include/settings.h" "include/utils.h" "include/audio.h" "src/audio.cpp")
target_link_libraries(main PRIVATE sfml-graphics sfml-audio)
target_link_libraries(main PRIVATE TGUI::TGUI)
target_compile_features(main PRIVATE cxx_std_17)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/bin/resources
)
add_dependencies(main copy_assets)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()
