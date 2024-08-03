add_executable(BPHOComputationalChallenge main.cpp)
# link statically
if (CMAKE_PLATFORM STREQUAL "linux")
    target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML glm tbb)
else ()
    target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML glm tbb)
endif ()