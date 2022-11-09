project(Langulus.Test.Module.GLFW)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)

# Configure Catch2 library                                                  
message(STATUS "Configuring external project: Catch2...")
download_project(
    PROJ                Catch2
    GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
    GIT_TAG             v2.x
    UPDATE_DISCONNECTED 1
)

add_subdirectory(${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR})
add_executable(Test.Module.GLFW ${SOURCE_FILES})
target_link_libraries(Test.Module.GLFW PRIVATE Catch2)

if (MSVC)
    target_compile_options(Test.Module.GLFW PRIVATE /bigobj)
endif()

add_test(NAME Test.Module.GLFW COMMAND Test.Module.GLFW)
