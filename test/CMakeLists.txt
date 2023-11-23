file(GLOB_RECURSE
	LANGULUS_MOD_GLFW_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModGLFWTest ${LANGULUS_MOD_GLFW_TEST_SOURCES})

target_link_libraries(LangulusModGLFWTest
	PRIVATE		Langulus
				Catch2
				glfw
)

add_test(
	NAME		LangulusModGLFWTest
	COMMAND		LangulusModGLFWTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)