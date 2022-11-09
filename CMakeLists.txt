project(Langulus.Mod.GLFW)

# We're importing symbols from Langulus.Framework                           
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
remove_definitions(-DLANGULUS_EXPORT_ALL)
if(NOT WIN32)
    add_compile_options(-fvisibility=hidden)
endif()

# Configure GLFW library, it will be statically built inside this module    
set(BUILD_SHARED_LIBS OFF CACHE BOOL
    "Build and link GLFW statically" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL
    "Save some calories on building GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL
    "Save some calories on building GLFW tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL
    "Save some calories on building GLFW documentation" FORCE)

message(STATUS "Configuring external project: GLFW...")
download_project(
    PROJ                glfw
    GIT_REPOSITORY      https://github.com/glfw/glfw.git
    GIT_TAG             3.3-stable
    UPDATE_DISCONNECTED 1
)
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
include_directories(${glfw_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source SOURCE_FILES)
add_library(Mod.GLFW SHARED ${SOURCE_FILES})

target_link_libraries(Mod.GLFW
    PRIVATE     Langulus.Framework
                glfw
)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()