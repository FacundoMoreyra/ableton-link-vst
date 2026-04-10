cmake_minimum_required(VERSION 3.22)

project(LinkBridge VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.12 
)
FetchContent_MakeAvailable(JUCE)

# Ableton Link
set(LINK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../link")
add_library(AbletonLink INTERFACE)
target_include_directories(AbletonLink INTERFACE
    ${LINK_DIR}/include
    ${LINK_DIR}/modules/asio-standalone/asio/include
)

# Platform-specific Link configuration
if(APPLE)
    target_compile_definitions(AbletonLink INTERFACE LINK_PLATFORM_MACOSX=1)
elseif(WIN32)
    target_compile_definitions(AbletonLink INTERFACE LINK_PLATFORM_WINDOWS=1)
    target_link_libraries(AbletonLink INTERFACE ws2_32 winmm)
else()
    target_compile_definitions(AbletonLink INTERFACE LINK_PLATFORM_LINUX=1)
    find_package(Threads REQUIRED)
    target_link_libraries(AbletonLink INTERFACE Threads::Threads)
endif()

juce_add_plugin(LinkBridge
    COMPANY_NAME "LinkBridge"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    FORMATS Standalone VST3
    PRODUCT_NAME "LinkBridge"
    VST3_CATEGORIES "Tools"
    PLUGIN_MANUFACTURER_CODE LkBr
    PLUGIN_CODE LkBr
)

target_sources(LinkBridge PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_compile_definitions(LinkBridge PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

# Linux dependencies
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    
    target_include_directories(LinkBridge PRIVATE 
        ${GTK3_INCLUDE_DIRS}
    )
    
    target_link_libraries(LinkBridge PRIVATE
        ${GTK3_LIBRARIES}
    )
endif()

target_link_libraries(LinkBridge PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_basics
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    AbletonLink
)
