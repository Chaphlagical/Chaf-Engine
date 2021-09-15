SetTarget(
    MODE STATIC
    TARGET_NAME Gui
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
        CHAF_OPENGL_API
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        imgui
        Engine
        glad
        glfw
)