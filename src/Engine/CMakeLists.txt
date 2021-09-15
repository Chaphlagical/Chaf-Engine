SetTarget(
    MODE STATIC
    TARGET_NAME Engine
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
        CHAF_OPENGL_API
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        spdlog
        glfw
        glad
)