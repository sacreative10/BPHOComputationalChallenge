add_executable(BPHOComputationalChallenge main.cpp)
# link statically
if (UNIX)
    target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML glm tbb)
else ()
    target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML glm)
endif ()

if(WIN32)
    if (BUILD_SHARED_LIBS)
        add_custom_command(TARGET BPHOComputationalChallenge POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
                $<TARGET_FILE:sfml-graphics>
                $<TARGET_FILE:sfml-window>
                $<TARGET_FILE:sfml-system>
                $<TARGET_FILE:glm>
                $<TARGET_FILE_DIR:BPHOComputationalChallenge>)
    endif()
endif()