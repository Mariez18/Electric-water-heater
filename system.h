#define system_H_


#include "macros.h"
#include "Cool_Heat.h"
#include "LED.h"
#include "SS.h"
#include "EEPROM.h"
#include "Button.h"
#include "Timer.h"
#include "ADC.h"


#define MAX_TEMP 75
#define MIN_TEMP 35
#define INIT_TEMP 60
#define OFF 0
#define ON  1
#define SETTING 2 
  //////////////// variables//////////////
int Curr_state;
int temp_arr[10];
int desired_temp;
int actual_temp;
int avg_temp;

int temp_measure_f;
int temp_measure_counter;
int setting_counter;

int toggle_f;
int arr_counter;

/////////////////functions//////////////////
void add_new_temp(int * arr , int new_val);
int  calc_avg_temp(int * arr);

void temp_arr_init(void);
void change_temp(void);

void sleep_mode(void);

void system_init(void);
void ON_mode(void);
void OFF_mode(void);
void SET_mode(void);
