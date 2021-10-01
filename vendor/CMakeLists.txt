# entt
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/entt/include")

# Eigen
add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/Eigen")

# dirent
add_library(dirent INTERFACE)
target_include_directories(dirent INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/dirent/include")

# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/glm")

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb_image")

# imgui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ImGui")
set(IMGUI_FILES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imconfig.h"
    "${IMGUI_DIR}/imgui.h"
    "${IMGUI_DIR}/imgui_internal.h"
    "${IMGUI_DIR}/imstb_rectpack.h"
    "${IMGUI_DIR}/imstb_textedit.h"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imstb_truetype.h")
add_library(imgui STATIC ${IMGUI_FILES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR})
set_property(TARGET imgui PROPERTY FOLDER "Extern/")

# glad
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad")
set(GLAD_FILES
    "${GLAD_DIR}/src/glad.c")
add_library(glad STATIC ${GLAD_FILES})
target_include_directories(glad PUBLIC ${GLAD_DIR} ${GLAD_DIR}/include)
set_property(TARGET glad PROPERTY FOLDER "Extern/")

# GLFW
add_subdirectory(GLFW)
set_property(TARGET glfw PROPERTY FOLDER "Extern/GLFW3/")
set_property(TARGET uninstall PROPERTY FOLDER "Extern/GLFW3/")

# spdlog
add_subdirectory(spdlog)
set_property(TARGET spdlog PROPERTY FOLDER "Extern/spdlog")

# implot
set(IMPLOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/implot")
set(IMPLOT_FILES
    "${IMPLOT_DIR}/implot.cpp"
    "${IMPLOT_DIR}/implot.h"
    "${IMPLOT_DIR}/implot_demo.cpp"
    "${IMPLOT_DIR}/implot_internal.h"
    "${IMPLOT_DIR}/implot_items.cpp")
add_library(implot STATIC ${IMPLOT_FILES})
target_include_directories(implot PUBLIC ${IMPLOT_DIR} ${IMPLOT_DIR})
target_include_directories(implot PUBLIC ${IMGUI_DIR} ${IMGUI_DIR})
set_property(TARGET implot PROPERTY FOLDER "Extern/")