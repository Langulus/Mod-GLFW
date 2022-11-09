project(Langulus.Test.Mod.GLFW)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_executable(Test.Mod.GLFW ${SOURCE_FILES})
target_link_libraries(Test.Mod.GLFW
	PRIVATE		Langulus.Framework
				Catch2
)

add_test(NAME Test.Mod.GLFW COMMAND Test.Mod.GLFW)
