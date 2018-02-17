file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(project1 ${SOURCES})

add_custom_command(TARGET project1 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/lib $<TARGET_FILE_DIR:project1>)
