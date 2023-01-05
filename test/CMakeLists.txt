project(Langulus.Test.Mod.GLFW)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LANGULUS_MOD_GLFW_TEST_SOURCES)
add_executable(Test.Mod.GLFW ${LANGULUS_MOD_GLFW_TEST_SOURCES})

target_link_libraries(Test.Mod.GLFW
	PRIVATE		Langulus.Framework
				Catch2
)

add_dependencies(Test.Mod.GLFW Mod.GLFW)
langulus_copy_dlls(
	TO		Test.Mod.GLFW
	FROM	Langulus.Framework Mod.GLFW
)

add_test(NAME Test.Mod.GLFW COMMAND Test.Mod.GLFW)
