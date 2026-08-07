cmake_minimum_required(VERSION 3.15)

project(ThePankeyEngine_DataStructure_Map)

if(NOT TARGET ThePankeyEngine_DataStructure_Map)

    add_library(ThePankeyEngine_DataStructure_Map INTERFACE)

    target_include_directories(
        ThePankeyEngine_DataStructure_Map
          INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

endif()

