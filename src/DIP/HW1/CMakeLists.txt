SetTarget(
    MODE EXE
    TARGET_NAME HW1
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        DIP
        opencv_core
)