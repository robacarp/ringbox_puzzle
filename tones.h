#include <math.h>

#define C  32.7
#define CS 34.6
#define D  36.7
#define DS 38.8
#define E  41.2
#define F  43.6
#define FS 46.2
#define G  48.9
#define GS 51.9
#define A  55
#define AS 58.2
#define B  61.7
#define NOTE(frequency, octave) (frequency * (pow( 2, octave - 1)))
#define PWM_WAIT(frequency) (int)(1000000 / (2 * frequency))
