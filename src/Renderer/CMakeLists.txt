SetTarget(
    MODE STATIC
    TARGET_NAME Renderer
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
        CHAF_OPENGL_API
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        Engine
        glad
        glm
        glfw
        stb
)