#ifndef VOLC_H
#define VOLC_H

#ifdef __cplusplus
extern "C" {
#    define VOLC_STRUCT_LITERAL(TYPE) TYPE
#else
#    define VOLC_STRUCT_LITERAL(TYPE) (TYPE)
#endif


/**
 * Name of the default sound card.
 * Can be passed to `volc_init` to avoid specifying `card`.
 */
#define VOLC_DEF_CARD "default"

/**
 * Name of the default mixer.
 * Can be passed to `volc_init` to avoid specifying `selector`.
 */
#define VOLC_DEF_SEL "Master"

/**
 * Name of the default mixer index.
 * Can be passed to `volc_init` to avoid specifying `selector_index`.
 */
#define VOLC_DEF_SEL_IDX 0

/**
 * Can be thought of as the default channel value.
 * Any nonexistent/unusable channel will be ignored.
 * Can be passed to `volc_volume_ctl` as `channel`.
 */
#define VOLC_ALL_CHANNELS ~0u

/**
 * Sets all defaults at once.
 * Can be passed as a single parameter to `volc_init` to set all the defaults.
 */
#define VOLC_ALL_DEFULTS VOLC_DEF_SEL, VOLC_DEF_SEL_IDX, VOLC_DEF_CARD

/**
 * How much should the volume go up by.
 * Takes float.
 * Can be passed to `volc_volume_ctl` as `new_volume`.
 */
#define VOLC_INC(X) (VOLC_STRUCT_LITERAL(volc_volume_t) {(X), VOLC_VOL_INC})

/**
 * How much should the volume go down by.
 * Takes float.
 * Can be passed to `volc_volume_ctl` as `new_volume`.
 */
#define VOLC_DEC(X) (VOLC_STRUCT_LITERAL(volc_volume_t) {-(X), VOLC_VOL_INC})

/**
 * What should volume be set to.
 * Takes float.
 * Can be passed to `volc_volume_ctl` as `new_volume`.
 */
#define VOLC_SET(X) (VOLC_STRUCT_LITERAL(volc_volume_t) {(X), VOLC_VOL_SET})

/**
 * Means that volume should stay the same
 * Can be passed to `volc_volume_ctl` as `new_volume`.
 */
#define VOLC_SAME (VOLC_STRUCT_LITERAL(volc_volume_t) {0, VOLC_VOL_SAME})

/**
 * Just query current state without changing it.
 * Can be passed to `volc_volume_ctl` as second and last argument.
 */
#define VOLC_GET_VOLUME VOLC_ALL_CHANNELS, VOLC_SAME, VOLC_CHAN_SAME

/*#define VOLC_VERBOSE*/

/**
 * What state a channel is (or should be) in.
 *
 * `VOLC_CHAN_SAME` means that the state of the switch does not change.
 * `VOLC_CHAN_TOGGLE` means that the state of the switch changes to the opposite.
 */
typedef enum chanel_switch {
    VOLC_CHAN_OFF = 0,
    VOLC_CHAN_ON,
    VOLC_CHAN_TOGGLE,
    VOLC_CHAN_SAME,
} channel_switch_t;


typedef struct _snd_mixer snd_mixer_t;
typedef struct _snd_mixer_elem snd_mixer_elem_t;
typedef struct _snd_mixer_selem_id snd_mixer_selem_id_t;

/**
 * Abstraction over the elements needed to change the volume
 */
typedef struct volc {
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;
    char const *card;
} volc_t;

/**
 * How should volume be altered.
 *   `VOLC_VOL_INC`: increases current volume by`volume`.
 *   `VOLC_VOL_SET`: set current volume to `volume`.
 *   `VOLC_VOL_SAME`: do not modify current volume (`volume` is ignored).
 */
typedef enum volc_volume_action {
    VOLC_VOL_INC,
    VOLC_VOL_SET,
    VOLC_VOL_SAME,
} volc_volume_action_t;

/**
 *  Representation of volume change
 *
 * `volume`: volume as a percentage. 100.f == 100%.
 */
typedef struct volc_volume {
    float volume;

    volc_volume_action_t action;

} volc_volume_t;

/** Result of a volume change
 *
 * If err < 0, an error has occurred, check volc_err_str
 * state.switch_pos is either VOLC_CHAN_ON or VOLC_CHAN_OFF.
 * state.volume is the representation of volume as a percentage (0 - 100).
 */
typedef union volc_volume_state {
    long err;

    struct {
        channel_switch_t switch_pos;
        float volume;
    } state;
} volc_volume_state_t;

/*
 * Initialises volc context (needed mainly to hold an `snd_mixer_t *` handle).
 * Args:
 *   `selector`: name of the mixer.
 *   `selector_index`: index of the mixer.
 *   `card`: name of the card.
 * Ret:
 *   On success: `volc_t *` to the new context.
 *   On error: `NULL`.
 */
extern volc_t *volc_init(char const *selector, unsigned int selector_index, char const *card);

/**
 * Frees volc context.
 * Args:
 *   `volc`: pointer to the context to be freed.
 * Ret:
 *   `void`
 */
extern void volc_deinit(volc_t *volc);

/**
 * String explaining the last error that occurred.
 * The pointer is owned by volc and will not change for the lifetime of the program.
 * Args:
 *   `void`
 * Ret:
 *   `const char *` to the error string.
 */
extern char const *volc_err_str(void);

/**
 * Control and query current sound volume.
 * Args:
 *   `volc`: pointer to the volc context.
 *   `channels`: which channel to use (either just 1 or all at once).
 *   `new_volume`: what to set volume to (e.g. VOLC_INC, VOLC_DEC, etc...).
 *   `channel_switch`: Should the channel be turned on, off, toggled or left unchanged.
 * Ret:
 *   `volc_volume_state_t` containing one of:
 *      the new state (volume and switch position) of the last affected channel.
 *      an error value less than 0 in case of an error (see volc_volume_state_t).
 */
extern volc_volume_state_t volc_volume_ctl(/**/
    volc_t *volc,
    unsigned int channels,
    volc_volume_t new_volume,
    channel_switch_t channel_switch);

#ifdef __cplusplus
}

#    include <memory>
#    include <stdexcept>

namespace VOLC {

/**
 * Thrown if volc_init fails
 */
class InitError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Thrown if volc_volume_ctl fails
 */
class VolumeCtlError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * Automatically manage the lifetime of volc_t and make volc_volume_ctl a member function.
 * Deinits volc_t when goes out of scope.
 * Can not be copied, only moved.
 */
class Volc {
    std::unique_ptr<volc_t, decltype(volc_deinit) *> m_ptr;

public:
    /**
     * Equivalent to volc_init
     *
     * throws InitError on error.
     */
    inline Volc(char const *selector, unsigned int selector_index, char const *card)
            : m_ptr {volc_init(selector, selector_index, card), volc_deinit} {
        if (!m_ptr) throw InitError(volc_err_str());
    }

    /*!
     * Equivalent to volc_init(VOLC_ALL_DEFULTS)
     *
     * throws InitError on error.
     */
    inline Volc(): Volc(VOLC_ALL_DEFULTS) { }

    /**
     * Equivalent to volc_volume_ctl
     *
     * throws VolumeCtlError on error
     */
    inline volc_volume_state_t volumeCtl(unsigned int channels, volc_volume_t new_volume, channel_switch_t channel_switch) {
        auto const state = volc_volume_ctl(m_ptr.get(), channels, new_volume, channel_switch);
        if (state.err < 0) throw VolumeCtlError(volc_err_str());
        return state;
    }
};
}  // namespace VOLC
#endif

#endif  /* VOLC_H*/
