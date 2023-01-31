#ifndef BUTTON
#define BUTTON


#define PRESSED 1
#define NOT_PRESSED 0

#define PORTB_ PORTB
#define DDRPORTB DDRB

#define ON_OFF_PIN 2
#define UP_PIN 1
#define DOWN_PIN 3

// Buttons Reading Functions
void Buttons_initialization(void);
void ON_OFF(void);

unsigned char UP_pressed(void);
unsigned char DOWN_pressed(void);


#endif