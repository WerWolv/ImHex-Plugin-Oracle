cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

# Change this to the name of your plugin #
project(example_cpp)

# Include ImHex cmake module #
set(IMHEX_BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/ImHex)
set(CMAKE_MODULE_PATH "${IMHEX_BASE_FOLDER}/cmake/modules")
include("${IMHEX_BASE_FOLDER}/cmake/build_helpers.cmake")

# Include ImHex
add_subdirectory(${IMHEX_BASE_FOLDER})
addVersionDefines()

# Add your source files here #
add_library(${PROJECT_NAME} SHARED
        source/plugin_example.cpp
)

# Add additional include directories here #
target_include_directories(${PROJECT_NAME} PRIVATE include)
# Add additional libraries here #
target_link_libraries(${PROJECT_NAME} PRIVATE)



# ---- No need to change anything from here downwards unless you know what you're doing ---- #

target_link_libraries(${PROJECT_NAME} PRIVATE libimhex)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(libimhex PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_dependencies(imhex_all ${PROJECT_NAME})
set(PLUGINS ${PLUGINS} ${PROJECT_NAME})
foreach (plugin IN LISTS PLUGINS)
    set_target_properties(${plugin} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
endforeach ()

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".hexplug")

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--allow-multiple-definition -fvisibility=hidden")
endif()

add_compile_definitions(IMHEX_PROJECT_NAME=\"${PROJECT_NAME}\")
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
setupCompilerWarnings(${PROJECT_NAME})

set(LIBROMFS_RESOURCE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/romfs)
set(LIBROMFS_PROJECT_NAME ${PROJECT_NAME})
add_subdirectory(${IMHEX_BASE_FOLDER}/lib/external/libromfs ${CMAKE_CURRENT_BINARY_DIR}/libromfs)
set_target_properties(${LIBROMFS_LIBRARY} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBROMFS_LIBRARY})

