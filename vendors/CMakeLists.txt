include(FetchContent)


if (CMAKE_USE_HOST_SFML)
    find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)
else ()
    FetchContent_Declare(
            sfml
            GIT_REPOSITORY "https://github.com/SFML/SFML"
            GIT_TAG "2.6.1"
    )
    add_subdirectory(sfml)
endif ()
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG 7b6314f47d2aaa3758cfeeca66af34f5c9309ca4 # latest docking branch as of 07/08/24
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG 2.6.x
)
add_subdirectory(imgui-sfml)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
)
FetchContent_MakeAvailable(glm)
