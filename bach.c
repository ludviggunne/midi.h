#include <stdio.h>

#define MIDI_IMPLEMENTATION
#include "midi.h"

#define END    -1
#define C(x)   ((x) * 12 + 0)
#define Cs(x)  ((x) * 12 + 1)
#define Db(x)  ((x) * 12 + 1)
#define D(x)   ((x) * 12 + 2)
#define Ds(x)  ((x) * 12 + 3)
#define Eb(x)  ((x) * 12 + 3)
#define E(x)   ((x) * 12 + 4)
#define F(x)   ((x) * 12 + 5)
#define Fs(x)  ((x) * 12 + 6)
#define Gb(x)  ((x) * 12 + 6)
#define G(x)   ((x) * 12 + 7)
#define Gs(x)  ((x) * 12 + 8)
#define Ab(x)  ((x) * 12 + 8)
#define A(x)   ((x) * 12 + 9)
#define As(x)  ((x) * 12 + 10)
#define Bb(x)  ((x) * 12 + 10)
#define B(x)   ((x) * 12 + 11)

const uint8_t chords[] = {
    C(5), E(5), G(5), C(6), E(6),
    C(5), D(5), A(5), D(6), F(6),
    B(4), D(5), G(5), D(6), F(6),
    C(5), E(5), G(5), C(6), E(6),
    C(5), E(5), A(5), E(6), A(6),
    C(5), D(5), Fs(5), A(5), D(6),
    B(4), D(5), G(5), D(6), G(6),
    B(4), C(5), E(5), G(5), C(6),
    A(4), C(5), E(5), G(5), C(6),
    D(4), A(4), D(5), Fs(5), C(6),
    G(4), B(4), D(5), G(5), B(5),
    G(4), Bb(4), E(5), G(5), Cs(6),
    F(4), A(4), D(5), A(5), D(6),
    F(4), Ab(4), D(5), F(5), B(5),
    E(4), G(4), C(5), G(5), C(6),
    E(4), F(4), A(4), C(5), F(5),
    D(4), F(4), A(4), C(5), F(5),
    G(3), D(4), G(4), B(4), F(5),
    C(4), E(4), G(4), C(5), E(5),
    C(4), G(4), Bb(4), C(5), E(5),
    F(3), F(4), A(4), C(5), F(5),
    Fs(3), C(4), A(4), C(5), Eb(5),
    Ab(3), F(4), B(4), C(5), D(5),
    G(3), F(4), G(4), B(4), D(5),
    G(3), E(4), G(4), C(5), E(5),
    G(3), D(4), G(4), C(5), F(5),
    G(3), D(4), G(4), B(4), F(5),
    G(3), Eb(4), A(4), C(5), Fs(5),
    G(3), E(4), G(4), C(5), G(5),
    G(3), D(4), G(4), C(5), F(5),
    G(3), D(4), G(4), B(4), F(5),
    C(3), C(4), G(4), Bb(4), E(5),
};

const size_t chords_size = sizeof(chords) / sizeof(*chords);

const uint8_t coda[] = {
    C(3), C(4), F(4), A(4), C(5), F(5), C(5), A(4),
    C(5), A(4), F(4), A(4), F(4), D(4), F(4), D(4),
    C(3), B(3), G(5), B(5), D(6), F(6), D(6), B(5),
    D(6), B(5), G(5), B(5), D(5), F(5), E(5), D(5),
};

const size_t coda_size = sizeof(coda) / sizeof(*coda);

const uint8_t end_chord[] = {
    C(3), C(4), E(5), G(5), C(6)
};

const size_t end_chord_size = sizeof(end_chord) / sizeof(*end_chord);

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Create midi context and single track
    uint16_t format = MIDI_FORMAT_SINGLE;
    uint16_t division = midi_division_ticks_per_quarter_note(2048);
    midi_t midi = midi_create(format, division);

    midi_track_t *track = midi_track_create();

    // Add title
    (void) midi_track_add_meta_event_text(track, 0,
                                          MIDI_TEXT_SEQUENCE_OR_TRACK_NAME,
                                          "Example track");

    // Play scale as quarter notes
    midi_message_t msg;
    uint32_t len = 512;

    //goto _coda;
    for (size_t i = 0; i < chords_size; i += 5) {

        for (size_t r = 0; r < 2; r++) {
            msg = midi_message_note_on(0, chords[i], 100);
            midi_track_add_midi_message(track, 0, msg);

            msg = midi_message_note_on(0, chords[i + 1], 70);
            midi_track_add_midi_message(track, len, msg);

            for (size_t j = 0; j < 6; j++) {
                size_t k = chords[i + 2 + j % 3];
                msg = midi_message_note_on(0, k, 100);
                uint32_t l = j == 0 ? len : 0;
                midi_track_add_midi_message(track, l, msg);

                msg = midi_message_note_off(0, k, 100);
                midi_track_add_midi_message(track, len, msg);
            }

            msg = midi_message_note_off(0, chords[i], 100);
            midi_track_add_midi_message(track, 0, msg);

            msg = midi_message_note_off(0, chords[i + 1], 100);
            midi_track_add_midi_message(track, 0, msg);
        }
    }

    int rit_len;

    goto _coda;
 _coda:
    if (1)
        rit_len = len;
    for (size_t i = 0; i < coda_size; i += 16) {

        msg = midi_message_note_on(0, coda[i], 100);
        midi_track_add_midi_message(track, 0, msg);

        msg = midi_message_note_on(0, coda[i + 1], 100);
        midi_track_add_midi_message(track, rit_len, msg);

        for (size_t j = 2; j < 16; j++) {
            size_t k = coda[i + j];
            msg = midi_message_note_on(0, k, 100);
            uint32_t l = j == 2 ? rit_len : 0;
            midi_track_add_midi_message(track, l, msg);

            msg = midi_message_note_off(0, k, 100);
            midi_track_add_midi_message(track, rit_len, msg);

            rit_len += 10;
        }

        msg = midi_message_note_off(0, coda[i], 100);
        midi_track_add_midi_message(track, 0, msg);

        msg = midi_message_note_off(0, coda[i + 1], 100);
        midi_track_add_midi_message(track, 0, msg);
    }

    for (size_t i = 0; i < end_chord_size; i++) {
        msg = midi_message_note_on(0, end_chord[i], 100);
        midi_track_add_midi_message(track, i ? len / 2 : 0, msg);
    }

    for (size_t i = 0; i < end_chord_size; i++) {
        msg = midi_message_note_off(0, end_chord[i], 100);
        midi_track_add_midi_message(track, i ? len * 16 : 0, msg);
    }

    // Finish track and add to context
    midi_track_add_end_of_track_event(track, 512);
    midi_add_track(&midi, track);

    // Output and cleanup
    (void) midi_write(&midi, stdout);
    midi_destroy(&midi);
}
