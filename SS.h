 
 #define SS_H_


 // DISP (Display)
 #define DISP1 1
 #define DISP2 2
// SS -> (Seven segment)

 #define SS_ENABLE_PORT_DIR DDRB
 #define SS_ENABLE_PORT PORTB
 #define SS1_ENABLE_PIN 6  //enable of first seven segment
 #define SS2_ENABLE_PIN 7  //enable for second seven segment
 #define SS_PORT_DIR DDRD
 #define SS_DATA PORTD


 // SS Functions
 void SS_init(void);
 void SS_send(int N);
 void SS_enable(int disp);
 void SS_disable(void);
 void SS_turn_off(void);
 void SS_write(int N);