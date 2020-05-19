cmake_minimum_required (VERSION 2.8)
project (timestables)

set (CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR})

# It looks like that, by using this, I got rid of a compiler-segfault  
# when building the openAL-part, plus an "access beyond end of merged section"-error
set (USE_BINARYBUILDER_LIBUV 0)

# If no build-type specified, then set it to Release
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE Release ... FORCE )
ENDIF()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DIMGUI_IMPL_OPENGL_LOADER_GLAD -DTWEAKIMGUI -DTT_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# we want the libraries to be found, somewhere in the same dir
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "./;./external/openal-soft")


include_directories(
    include
    external/openal-soft/common
    external/openal-soft/include
    external/glfw/include
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
include_directories(
    external/imgui
    external/imgui/examples)
ENDIF()



IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set( timestables_SOURCES 
        src/main.cpp
        src/game.cpp
        src/bloxel.cpp
        src/sprite.cpp
        src/framebuffers.cpp
        src/gamelogic.cpp
        src/audio.cpp
        src/text.cpp
        src/loadwav.cpp
        src/glad.c
        src/stb_image_implementation.cpp
        external/imgui/examples/imgui_impl_glfw.cpp 
        external/imgui/examples/imgui_impl_opengl3.cpp
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_widgets.cpp
        )
ELSE()
    set( timestables_SOURCES 
        src/main.cpp
        src/game.cpp
        src/bloxel.cpp
        src/sprite.cpp
        src/framebuffers.cpp
        src/gamelogic.cpp
        src/text.cpp
        src/audio.cpp
        src/loadwav.cpp
        src/glad.c
        src/stb_image_implementation.cpp
        )

ENDIF()

add_executable( tables ${timestables_SOURCES})

# glfw
set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" )
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set (GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set (GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set (GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
set (GLFW_VULKAN_STATIC OFF CACHE BOOL "Assume the Vulkan loader is linked with the application")


add_subdirectory(external/glfw)
target_link_libraries(tables glfw)

# OpenAL
set (ALSOFT_DLOPEN OFF CACHE BOOL "Check for the dlopen API for loading optional libs")
set (ALSOFT_UTILS OFF CACHE BOOL "Build utility programs")
set (ALSOFT_NO_CONFIG_UTIL OFF CACHE BOOL "Disable building the alsoft-config utility")
set (ALSOFT_EXAMPLES OFF CACHE BOOL "Build example programs")
set (ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "Install alsoft.conf sample configuration file")
set (ALSOFT_TESTS OFF CACHE BOOL "Build and install test programs")
set (ALSOFT_INSTALL_HRTF_DEFS OFF CACHE BOOL "Install HRTF definition files")
set (ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "Install AmbDec preset files")
set (ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "Install example programs (alplay, alstream, ...)")
set (ALSOFT_INSTALL_UTILS OFF CACHE BOOL "Install utility programs (openal-info, alsoft-config, ...)")
set (ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "Update git build version info")

add_subdirectory(external/openal-soft)
target_link_libraries(tables OpenAL)

if (WIN32)
    set (${OpenAL_CXX_FLAGS} "-G MSYS Makefiles")
endif (WIN32)

if (UNIX)
	target_link_libraries( tables
		GL 
		X11 
		pthread 
		Xrandr 
		Xi
		dl ) 
endif (UNIX)

ADD_CUSTOM_TARGET(copy_shader_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
    COMMENT "Copy shaderfiles-directory to build tree"
    VERBATIM)

ADD_CUSTOM_TARGET(copy_resource_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/resources" "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Copy resource-directory to build tree"
    VERBATIM)

# copy fontfile
file( COPY ${PROJECT_SOURCE_DIR}/external/imgui/misc/fonts/Roboto-Medium.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources )

## Where are the additional libraries installed? Note: provide includes
## path here, subsequent checks will resolve everything else
## set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../assimp/build/code" )
#
##target_link_libraries(demo PUBLIC ${ASSIMP_LIBRARY})
#
##link_directories("${CMAKE_SOURCE_DIR}/../assimp/build/code")
##message(STATUS "Jaja:")
##message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}  <<<")
## Compiler Condition (gcc ; g++)
##if ( "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" )
##  message( STATUS "Setting the flags for ${CMAKE_C_COMPILER_ID} compiler" )
##  add_definitions( --std=c99 )
##endif()
#
##SET(CMAKE_EXE_LINKER_FLAGS 
##              "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib")
#message(STATUS "${CMAKE_EXE_LINKER_FLAGS}  <<<")
#
## Check for OS
#if( UNIX )
#    set( LEARN_CMAKE_DEFINITIONS
#        "${LEARN_CMAKE_DEFINITIONS} -Wall -Wextra -Werror -Wno-deprecated-declarations -Wno-unused-parameter -Wno-comment" )
#endif()
