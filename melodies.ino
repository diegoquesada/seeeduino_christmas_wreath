#include "music_notes.h"

typedef struct MusicNote_t
{
    uint16_t note;
    uint16_t duration;
};

MusicNote_t MarioMelody[] = {
    {NOTE_E4, 8}, {NOTE_E4, 4}, {NOTE_E4, 4}, {NOTE_C4, 8}, {NOTE_E4,4}, {NOTE_G4,2}, {NOTE_G3,2},
    {NOTE_C4,3}, {NOTE_G3,3}, {NOTE_E3,3}, {NOTE_A3,4}, {NOTE_B3,4}, {NOTE_AS3,8}, {NOTE_A3,4}, {NOTE_G3,8}, {NOTE_E4,8}, {NOTE_G4,8}, {NOTE_A4,4}, {NOTE_F4,8}, {NOTE_G4,4}, {NOTE_E4,3}, {NOTE_C4,8}, {NOTE_D4,8}, {NOTE_B3,3},
    {NOTE_C4,3}, {NOTE_G3,3}, {NOTE_E3,3}, {NOTE_A3,4}, {NOTE_B3,4}, {NOTE_AS3,8}, {NOTE_A3,4}, {NOTE_G3,8}, {NOTE_E4,8}, {NOTE_G4,8}, {NOTE_A4,4}, {NOTE_F4,8}, {NOTE_G4,4}, {NOTE_E4,3}, {NOTE_C4,8}, {NOTE_D4,8}, {NOTE_B3,2},
/*    {NOTE_G4,}, {NOTE_FS4,}, {NOTE_E4,}, {NOTE_DS4,}, {NOTE_E4,}, {NOTE_GS3,}, {NOTE_A3,}, {NOTE_C4,}, {NOTE_A3,}, {NOTE_C4,}, {NOTE_D4,}, {NOTE_G4,}, {NOTE_FS4,}, {NOTE_E4,}, {NOTE_DS4,}, {NOTE_E4,}, {NOTE_C5,}, {NOTE_C5,}, {NOTE_C5,},
    {NOTE_G4,}, {NOTE_FS4,}, {NOTE_E4,}, {NOTE_DS4,}, {NOTE_E4,}, {NOTE_GS3,}, {NOTE_A3,}, {NOTE_C4,}, {NOTE_A3,}, {NOTE_C4,}, {NOTE_D4,}, {NOTE_DS4,}, {NOTE_D4,}, {NOTE_C4,},
    {NOTE_C4,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_D4,}, {NOTE_E4,}, {NOTE_C4,}, {NOTE_A3,}, {NOTE_G3,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_D4,}, {NOTE_E4,},
    {NOTE_C4,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_C4,}, {NOTE_D4,}, {NOTE_E4,}, {NOTE_C4,}, {NOTE_A3,}, {NOTE_G3,}*/
};
/*8,8,8,4,4,8,8,4,8,8,3,8,8,8,4,4,4,8,2,
8,8,8,4,4,8,8,4,8,8,3,3,3,1,
8,4,4,8,4,8,4,8,2,8,4,4,8,4,1,
8,4,4,8,4,8,4,8,2*/

// notes in the melody:
MusicNote_t WeWishYouMelody[] = {
    {NOTE_G3,4},
    {NOTE_C4,4}, {NOTE_C4,8}, {NOTE_D4,8}, {NOTE_C4,8}, {NOTE_B3,8},
    {NOTE_A3,4}, {NOTE_A3,4}, {NOTE_A3,4},
    {NOTE_D4,4}, {NOTE_D4,8}, {NOTE_E4,8}, {NOTE_D4,8}, {NOTE_C4,8},
    {NOTE_B3,4}, {NOTE_G3,4}, {NOTE_G3,4},
    {NOTE_E4,4}, {NOTE_E4,8}, {NOTE_F4,8}, {NOTE_E4,8}, {NOTE_D4,8},
    {NOTE_C4,4}, {NOTE_A3,4}, {NOTE_G3,8}, {NOTE_G3,8},
    {NOTE_A3,4}, {NOTE_D4,4}, {NOTE_B3,4}, {NOTE_C4,2}
};

void PlayMelody(MusicNote_t *melody, int numNotes)
{
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < numNotes; thisNote++  ) {
        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000/melody[thisNote].duration;
        tone(SPEAKER, melody[thisNote].note,noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration   30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(8);
    }
}

/*void loop()
{
    PlayMelody(WeWishYouMelody, sizeof(WeWishYouMelody)/sizeof(WeWishYouMelody[0]));
    delay(2000);
    PlayMelody(MarioMelody, sizeof(MarioMelody)/sizeof(MarioMelody[0]));
    delay(2000);
}*/

MusicNote_t **currentMelody;

uint32_t lastNoteChange = 0;
uint8_t currentNote = 0;
uint32_t currentNoteDuration = 0;

void melodyInit()
{
    currentMelody = WeWishYouMelody;
}

void melodyStart()
{
    currentNote = 0;
    currentNoteDuration = 0;
}

void melodyNext()
{

}

void melodyStep()
{
    if (millis() - lastNoteChange > currentMelody[currentNote]->duration)
    {
        currentNote++;
    }
}

void melodyStop()
{

}
