#include <core/engine.h>

#include <stdio.h>

int
main(void)
{
    printf("Hello from bootstrap\n");

    engine_state* engine = engine_create();
    if (!engine) {
        return 1;
    }

    // Begin the engine loop.
    if (!engine_run(engine)) {
        return 2;
    }

    return 0;
}
