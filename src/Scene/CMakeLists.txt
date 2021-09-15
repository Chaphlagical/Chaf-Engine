SetTarget(
    MODE STATIC
    TARGET_NAME Scene
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        entt
        Engine
        glm
        imgui
)