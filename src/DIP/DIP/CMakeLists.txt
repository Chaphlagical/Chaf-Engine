SetTarget(
    MODE STATIC
    TARGET_NAME DIP
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        Engine
        Renderer
        Editor
        opencv_core
)