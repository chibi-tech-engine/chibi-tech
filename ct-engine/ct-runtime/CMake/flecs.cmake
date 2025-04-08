include(FetchContent)

FetchContent_Declare(flecs
  GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
  GIT_TAG v4.0.5
)

set(FLECS_STATIC OFF CACHE BOOL  "")
set(FLECS_SHARED ON  CACHE BOOL  "")
set(FLECS_PIC    ON  CACHE BOOL  "")
set(FLECS_TESTS  OFF CACHE BOOL  "")

FetchContent_MakeAvailable(flecs)
