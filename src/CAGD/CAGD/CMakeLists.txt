SetTarget(
    MODE STATIC
    TARGET_NAME CAGD
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
    INC
        ${PROJECT_SOURCE_DIR}/src
    LIB
        Engine
        Editor
        implot
        Eigen
)