#define Cool_Heat_H_


#define Cool_Heat
//#include "std_macros.h"

#define HEATDDR DDRB
#define heat_port PORTB
#define heatpin 4  //Heat led
void heat_initialization(void);
void heat_start(void);
void heat_stop(void);

/*/////////////////////////////*/

#define cool_port PORTB
#define COOLDDR DDRB
#define coolpin 5  //FAN
void cool_initialization(void);
void cool_start(void);
void cool_stop(void);