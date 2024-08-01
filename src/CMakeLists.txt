add_executable(BPHOComputationalChallenge main.cpp)
# link statically
target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML glm tbb)

