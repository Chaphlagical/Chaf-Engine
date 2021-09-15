SetTarget(
    MODE STATIC
    TARGET_NAME Editor
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        Gui
        Scene
        Engine
        Renderer
        dirent
)