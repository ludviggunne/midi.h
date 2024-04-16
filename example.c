#include <stdio.h>

#define MIDI_IMPLEMENTATION
#include "midi.h"

// Lydian dominant scale
const uint8_t scale[] = { 60, 62, 64, 66, 67, 69, 70, 72 };

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Create midi context and single track
    uint16_t format = MIDI_FORMAT_SINGLE;
    uint16_t division = midi_division_ticks_per_quarter_note(512);
    midi_t midi = midi_create(format, division);

    midi_track_t *track = midi_track_create();

    // Play scale as quarter notes
    midi_message_t msg;
    for (size_t i = 0; i < 8; i++) {

        msg = midi_message_note_on(0, scale[i], 100);
        (void) midi_track_add_midi_message(track, 0, msg);

        msg = midi_message_note_off(0, scale[i], 100);
        (void) midi_track_add_midi_message(track, 512, msg);
    }

    // Finish track and add to context
    midi_track_add_end_of_track_event(track, 512);
    midi_add_track(&midi, track);

    // Output and cleanup
    (void) midi_write(&midi, stdout);
    midi_destroy(&midi);
}
