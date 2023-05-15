project(LangulusModGLFWTest)

add_executable(LangulusModGLFWTest
	Main.cpp
	TestWindow.cpp
)

target_link_libraries(LangulusModGLFWTest
	PRIVATE		Langulus
				Catch2
)

add_test(
	NAME		LangulusModGLFWTest
	COMMAND		LangulusModGLFWTest
)

langulus_copy_dlls(LangulusModGLFWTest ON LangulusModGLFWTest)
langulus_copy_dlls_advanced(LangulusModGLFWTest
	TO			LangulusModGLFWTest
	FROM		LangulusModGLFW
)