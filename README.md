# VOLC

A simple volume control library (based on a small subset of the functionality of
`amixer`)

## Compiling

You can use the following command to compile a static library:

``` sh
gcc -c volc.o volc.c -lm -lasound -Wall -Wextra -Wshadow
ar rcs libvolc.a volc.o
```

Another option would be to simply include `volc.c` and `volc.h` into your
application source tree. Note that you would need to ling against `asound` and
the math library.

## Quick start

Try compiling and running the following example with:

``` sh
gcc demo.c volc/volc.c -lm -lasound -o demo && ./demo
```

``` c
// demo.c
#include "volc/volc.h"

#include <stdio.h>

int main() {
    volc_t *volc;
    // Initialise volc context
    if (!(volc = volc_init(VOLC_ALL_DEFULTS))) { // NULL means an error occurred
        puts("initialisation failed");
        puts(volc_err_str());  // check last reported error
        return 1;
    }
    // volc_volume_ctl returns current volume state or an error
    // it can be used to set the volume as well (see the docs).
    volc_volume_state_t res = volc_volume_ctl(volc, VOLC_GET_VOLUME);
    if (res.err < 0) {
        puts("error getting volume");
        puts(volc_err_str());
    } else {
        // volume is represented as a percentage. 100.0f == 100%
        printf("Mixer is %s. Volume is %f%%", res.state.switch_pos ? "ON" : "OFF", res.state.volume);
    }
    volc_deinit(volc);
    return 0;
}
```

## Manual

See [the docs](documentation.md) for details.
