cmake_minimum_required(VERSION 3.24)
project(LangulusModGLFW
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for managing native windows using the GLFW library"
    HOMEPAGE_URL    https://langulus.com
)

# Configure GLFW library, it will be statically built inside this module        
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Configure GLFW library                                                        
fetch_external_module(
    glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         4a33051 # 3.3-stable
)

file(GLOB_RECURSE
	LANGULUS_MOD_GLFW_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp source/*.hpp
)

# Build the module                                                              
add_library(LangulusModGLFW SHARED ${LANGULUS_MOD_GLFW_SOURCES})

target_link_libraries(LangulusModGLFW
    PRIVATE     Langulus
                glfw
)

if(LANGULUS_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()