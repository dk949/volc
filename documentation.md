# The docs

## Functions

``` c
extern volc_t *volc_init(const char *selector, unsigned int selector_index, const char *card);
```

* Initialises volc context (needed mainly to hold an `snd_mixer_t *` handle).
* Args:
  * `selector`: name of the mixer.
  * `selector_index`: index of the mixer.
  * `card`: name of the card.
* Ret:
  * On success: `volc_t *` to the new context.
  * On error: `NULL`.

``` c
extern void volc_deinit(volc_t *volc);
```

* Frees volc context.
* Args:
  * `volc`: pointer to the context to be freed.
* Ret:
  * `void`

``` c
extern const char *volc_err_str();
```

* Pointer to the explanation of the last error that occurred. The pointer will
  not change for the lifetime of the program.
* Args:
  * `void`
* Ret:
  * `const char *` to the error string.

``` c
extern volc_volume_state_t volc_volume_ctl(volc_t *volc, unsigned int channels, volc_volume_t new_volume, channel_switch_t channel_switch);
```

* Function to control and query current sound volume.
* Args:
  * `volc`: pointer to the void context.
  * `channels`: which channel to use (either just 1 or all at once).
  * `new_volume`: what to set volume to (see [Macros](#Macros) for usage).
  * `channel_switch`: Should the channel be turned on, off, toggled or left
    unchanged.
* Ret:
  * `volc_volume_state_t` containing either the new state (volume and switch
    position) of the last affected channel, or an error value less than 0 in
    case of an error.

## Data types

``` c
typedef struct volc {
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;
    const char *card;

} volc_t;
```

* volc context.

``` c
typedef enum chanel_switch {
    VOLC_CHAN_OFF = 0,
    VOLC_CHAN_ON,
    VOLC_CHAN_TOGGLE,
    VOLC_CHAN_SAME,

} channel_switch_t;
```

* Positions the channel switch could be in.
* `VOLC_CHAN_SAME` means that the state of the switch does not change.
* `VOLC_CHAN_TOGGLE` means that the state of the switch changes to the opposite.

``` c
typedef struct volc_volume {
    float volume;
    enum {
        VOLC_VOL_INC,
        VOLC_VOL_SET,
        VOLC_VOL_SAME,
    } action;

} volc_volume_t;
```

* `volume`: volume as a percentage. 100.f == 100%.
* `action`:
  * `VOLC_VOL_INC`: increases current volume by`volume`.
  * `VOLC_VOL_SET`: set current volume to `volume`.
  * `VOLC_VOL_SAME`: do not modify current volume (`volume` is ignored).

``` c
typedef union volc_volume_state {
    long err;
    struct {
        channel_switch_t switch_pos;
        float volume;
    } state;
} volc_volume_state_t;
```

* `err`: if less than 0, an error occurred.
* `state`:
  * `switch_pos`: channel switch position.
  * `volume`: volume after the operation.

## Macros

``` c
#define VOLC_DEF_SEL      "Master"
```

* Name of the default mixer.
* Can be passed to `volc_init` to avoid specifying `selector`.

``` c
#define VOLC_DEF_SEL_IDX  0
```

* Name of the default mixer index.
* Can be passed to `volc_init` to avoid specifying `selector_index`.

``` c
#define VOLC_DEF_CARD     "default"
```

* Name of the default sound card.
* Can be passed to `volc_init` to avoid specifying `card`.

``` c
#define VOLC_ALL_DEFULTS  VOLC_DEF_SEL, VOLC_DEF_SEL_IDX, VOLC_DEF_CARD
```

* Sets all defaults at once.
* Can be passed as a single parameter to `volc_init` to set all the defaults.

``` c
#define VOLC_ALL_CHANNELS ~0u
```

* Can be thought of as the default channel value.
* Any nonexistent/unusable channel will be ignored.
* Can be passed to `volc_volume_ctl` as `channel`.

``` c
#define VOLC_INC(X)       ((volc_volume_t) {.volume = (X), .action = VOLC_VOL_INC})
```

* How much should the volume go up by.
* Takes float.
* Can be passed to `volc_volume_ctl` as `new_volume`.

``` c
#define VOLC_DEC(X)       ((volc_volume_t) {.volume = -(X), .action = VOLC_VOL_INC})
```

* How much should the volume go down by.
* Takes float.
* Can be passed to `volc_volume_ctl` as `new_volume`.

``` c
#define VOLC_SET(X)       ((volc_volume_t) {.volume = (X), .action = VOLC_VOL_SET})
```

* What should volume be set to.
* Takes float.
* Can be passed to `volc_volume_ctl` as `new_volume`.

``` c
#define VOLC_SAME         ((volc_volume_t) {.volume = 0, .action = VOLC_VOL_SAME})
```

* Means that volume should stay the same
* Can be passed to `volc_volume_ctl` as `new_volume`.

``` c
#define VOLC_GET_VOLUME   VOLC_ALL_CHANNELS, VOLC_SAME, VOLC_CHAN_SAME
```

* Just query current state without changing it.
* Can be passed to `volc_volume_ctl` as second and last argument.

``` c
/* #define VOLC_VERBOSE */
```
* Can be set by user
  * Not set initially
* Enables tracing
  * With an overwhelming amount of output on `stderr`
