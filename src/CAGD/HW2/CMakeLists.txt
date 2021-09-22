SetTarget(
    MODE EXE
    TARGET_NAME HW2
    DEFINE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/"
        NOMINMAX
    INC
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/vendor
    LIB
        CAGD
        Eigen
)