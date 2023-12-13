#include "music_notes.h"
#include "pins.h"

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
    {NOTE_A3,4}, {NOTE_D4,4}, {NOTE_B3,4}, {NOTE_C4,2},
};

MusicNote_t *currentMelody;
uint8_t numNotesCurrentMelody = 0;

unsigned long lastNoteChange = 0;
uint8_t currentNote = 0;
unsigned long currentNoteDuration = 0;

void melodyInit()
{
    currentMelody = WeWishYouMelody;
    numNotesCurrentMelody = sizeof(WeWishYouMelody)/sizeof(WeWishYouMelody[0]);
}

void melodyStart()
{
    currentNote = 0;
    lastNoteChange = millis() - 10000; // Setting it way back to force next note to play in melodyStep()
    currentNoteDuration = 100;
}

void melodyNext()
{
}

void melodyStep()
{
    // to distinguish the notes, set a minimum time between them.
    // the note's duration   30% seems to work well:
    if (millis() - lastNoteChange > currentNoteDuration * 1.30)
    {
        noTone(SPEAKERPIN);

        if (currentNote >= numNotesCurrentMelody)
        {
            currentNote = 0;
            currentNoteDuration = 2000;
        }
        else
        {
            currentNoteDuration = 1000/currentMelody[currentNote].duration;
            lastNoteChange = millis();
            tone(SPEAKERPIN, currentMelody[currentNote].note, currentNoteDuration);

            currentNote++;
        }
    }
}

void melodyStop()
{

}

/*void PlayMelody(MusicNote_t *melody, int numNotes)
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

void loop()
{
    PlayMelody(WeWishYouMelody, sizeof(WeWishYouMelody)/sizeof(WeWishYouMelody[0]));
    delay(2000);
    PlayMelody(MarioMelody, sizeof(MarioMelody)/sizeof(MarioMelody[0]));
    delay(2000);
}*/
