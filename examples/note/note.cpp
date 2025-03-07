#include "note.h"
//

Table note({},{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"});

void initializeNote() {
    note.data = {
        {12, 13, 22, 23, 32, 42, 43, 52, 53, 62, 63, 72},            // noteid
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},                    // noteordem
        {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}, // note
        {1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 7},                      // soundid
        {"C", "C", "D", "D", "E", "F", "F", "G", "G", "A", "A", "B"},     // sound
        {2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 3, 2},                      // accidentalid
        {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0},                       // accidental_value
        {"Dó", "Dó#", "Ré", "Ré#", "Mi", "Fá", "Fá#", "Sol", "Sol#", "Lá", "Lá#", "Sí"} // note pt-br
    };

    note.rowNameToIndex = {
        {"noteid", 0},
        {"noteordem", 1},
        {"note", 2},
        {"soundid", 3},
        {"sound", 4},
        {"accidentalid", 5},
        {"accidental_value", 6},
        {"note_pt_br", 7}
    };
}

