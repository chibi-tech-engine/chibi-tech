include(FetchContent)

FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

set(BUILD_SHARED_LIBS   OFF CACHE BOOL  "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL  "")
set(GLFW_BUILD_DOCS     ON  CACHE BOOL  "")
set(GLFW_INSTALL        ON  CACHE BOOL  "")

FetchContent_MakeAvailable(glfw)
