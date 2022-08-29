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
application source tree. Note that you would need to link against `asound` and
the math library.

If used in a C++ application, make sure to still compile `volc.c` with a C
compiler.

## Quick start

**C example**

Requires at least C99

``` sh
cc demo.c volc/volc.c -D_DEFAULT_SOURCE -lm -lasound -o demo && ./demo
```

``` c
/*demo.c*/
#include "volc/volc.h"

#include <stdio.h>

int main() {
    volc_t *volc;
     /*Initialise volc context*/
    if (!(volc = volc_init(VOLC_ALL_DEFULTS))) {  /* NULL means an error occurred*/
        printf("initialisation failed: %s\n", volc_err_str());
        return 1;
    }
     /*volc_volume_ctl returns current volume state or an error.
       it can be used to set the volume as well (see the docs).*/
    volc_volume_state_t res = volc_volume_ctl(volc, VOLC_GET_VOLUME);
    if (res.err < 0)
        printf("error getting volume: %s\n", volc_err_str());
    else  /* volume is represented as a percentage. 100.0f == 100%*/
        printf("Mixer is %s. Volume is %f%%\n", res.state.switch_pos ? "ON" : "OFF", res.state.volume);
    volc_deinit(volc);
    return 0;
}
```

**C++ example**

Requires at least C++11

``` sh
cc volc/volc.c -D_DEFAULT_SOURCE -c -o volc.o
c++ demo.cpp volc.o -D_DEFAULT_SOURCE -lm -lasound -o demo && ./demo
```

``` cpp
// demo.cpp
#include "volc/volc.h"

#include <cstdio>

int main() try {
    // Initialise volc context. Throws VOLC::InitError.
    VOLC::Volc volc;
    // Volc::volumeCtl returns current volume state. throws VOLC::VolumeCtlError.
    // Never returns error.
    // it can be used to set the volume as well (see the docs).
    volc_volume_state_t res = volc.volumeCtl(VOLC_GET_VOLUME);
    // volume is represented as a percentage. 100.0f == 100%
    printf("Mixer is %s. Volume is %f%%", res.state.switch_pos ? "ON" : "OFF", res.state.volume);
    return 0;

} catch (VOLC::InitError &e) {
    printf("initialisation failed: %s\n", e.what());
    return 1;
} catch (VOLC::VolumeCtlError &e) {
    printf("error getting volume: %s\n", e.what());
    return 1;
}
```

## Docs

Documentation has been moved into the header file.
