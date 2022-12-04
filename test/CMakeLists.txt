project(Langulus.Test.Mod.GLFW)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_executable(Test.Mod.GLFW ${SOURCE_FILES})
target_link_libraries(Test.Mod.GLFW
	PRIVATE		Mod.GLFW
				Catch2
)

add_custom_command(
    TARGET Test.Mod.GLFW POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Langulus.Framework>" "$<TARGET_FILE_DIR:Test.Mod.GLFW>"
    COMMENT "Copying `$<TARGET_FILE:Langulus.Framework>` to `$<TARGET_FILE_DIR:Test.Mod.GLFW>`"
)

add_custom_command(
    TARGET Test.Mod.GLFW POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Mod.GLFW>" "$<TARGET_FILE_DIR:Test.Mod.GLFW>"
    COMMENT "Copying `$<TARGET_FILE:Mod.GLFW>` to `$<TARGET_FILE_DIR:Test.Mod.GLFW>`"
)

add_test(NAME Test.Mod.GLFW COMMAND Test.Mod.GLFW)
