project(Langulus.Mod.GLFW)

# Configure GLFW library, it will be statically built inside this module    
set(BUILD_SHARED_LIBS OFF CACHE BOOL
    "Build and link GLFW statically" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL
    "Save some calories on building GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL
    "Save some calories on building GLFW tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL
    "Save some calories on building GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

download_project(
    PROJ                glfw
    GIT_REPOSITORY      https://github.com/glfw/glfw.git
    GIT_TAG             3.3-stable
    UPDATE_DISCONNECTED 1
)
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
include_directories(${glfw_SOURCE_DIR}/include)

if (MSVC)
    add_compile_options(/bigobj /W4)
    get_target_property(CURRENT_GLFW_OPTIONS glfw COMPILE_OPTIONS)
    # Remove annoying D9025 warning                                         
    list(REMOVE_ITEM CURRENT_GLFW_OPTIONS /W4)
    set_target_properties(glfw PROPERTIES COMPILE_OPTIONS "${CURRENT_GLFW_OPTIONS}")
endif()

# Build the module                                                          
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source LANGULUS_MOD_GLFW_SOURCES)
add_library(Mod.GLFW SHARED ${LANGULUS_MOD_GLFW_SOURCES})

target_link_libraries(Mod.GLFW
    PRIVATE     Langulus.Framework
                glfw
)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()