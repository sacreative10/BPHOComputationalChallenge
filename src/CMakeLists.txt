add_executable(BPHOComputationalChallenge main.cpp)
target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML glm)

## we need the ability to build for android devices
#if (ANDROID)
#    add_library(BPHOComputationalChallenge SHARED main.cpp)
#    target_link_libraries(BPHOComputationalChallenge PRIVATE ImGui-SFML::ImGui-SFML)
#endif ()
