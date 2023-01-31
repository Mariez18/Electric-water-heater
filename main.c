/*
 * final project.c
 *
 * Created: 1/24/2023 10:10:50 PM
 * Author : Dell
 */ 

#include <avr/io.h>
#define F_CPU 1000000ul

#include<util\delay.h>

#include "system.h"


/////////////////////////////LED///////////////////////////

/*led_init() : init. heating element indication led by making it an output pin.*/

void led_init(void)
{
	// PIN 0 in the port is an output pin
	// the LED pin is initially off
	
	set_bit(LED_PORT_DIR,LED_PIN);
	clear_bit(LED_PORT,LED_PIN);
	}
/*LED control : to turn on/off or toggle the LED.*/
void led_on(void)
{
	set_bit(LED_PORT,LED_PIN);
}

void led_off(void)
{
	clear_bit(LED_PORT,LED_PIN);
}

////*cool & heat initialization :to initialize their pins as outputs *////

void heat_initialization(void){
	set_bit(HEATDDR,heatpin);
}

void cool_initialization(void){
	set_bit(COOLDDR,coolpin);
}

///////////cool & heat start///////////
void heat_start(void){
	set_bit(heat_port,heatpin);
}

void cool_start(void){
	set_bit(cool_port,coolpin);
}
////cool & heat stop////
void heat_stop(void)
{
	clear_bit(heat_port,heatpin);
}

void cool_stop(void)
{
	clear_bit(cool_port,coolpin);
}


////////////////////*Seven segment*/////////////////////

/*SS_init to initialize seven segment by making their ports as output & default off */

// Array for Numbers' Values in hexa 
///////////////////  0     1    2    3    4    5    6   7    8    9  //////////////////
uint8_t Nums[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void SS_init(void){
	
	// setting Seven Segment Pins as output pins
	SS_PORT_DIR = 0xFF;
	set_bit(SS_ENABLE_PORT_DIR,SS1_ENABLE_PIN);
	set_bit(SS_ENABLE_PORT_DIR,SS2_ENABLE_PIN);
	
	// Seven segment initially OFF
	SS_disable();
}
/*SS_send : function to send data (the number) to the seven segment */
void SS_send(int N)
{
	SS_DATA = Nums[N];
}
/*ss_enable : function to enable one seven segment and disable the other (as 2 seven segments are on the same bins) */
void SS_enable(int disp){
	if(disp == 1){
		set_bit(SS_ENABLE_PORT,SS1_ENABLE_PIN);
		clear_bit(SS_ENABLE_PORT,SS2_ENABLE_PIN);
	}
	else if (disp == 2){
		set_bit(SS_ENABLE_PORT,SS2_ENABLE_PIN);
		clear_bit(SS_ENABLE_PORT,SS1_ENABLE_PIN);
	}
}
/*ss_turn_off for off mode*/
void SS_turn_off(void)
{
	SS_DATA = 0x00;
}


void SS_disable(void)
{
clear_bit(SS_ENABLE_PORT,SS1_ENABLE_PIN);
clear_bit(SS_ENABLE_PORT,SS2_ENABLE_PIN);
}
 
/*SS_Write: function to calculate the number display on each seven segment */
void SS_write(int N)
{
	int units; //number display on the first SS.  
	int tens; //number display on the second SS.
	
	units = N % 10; //EX:12 % 10 =2
	
	tens = N / 10; //EX: 12/10=1
	
	SS_enable(DISP1); 
	SS_send(units);
	_delay_ms(5);
	
	SS_enable(DISP2);
	SS_send(tens);
	_delay_ms(5);
}

//////BUTTONS/////

/*
Buttons_initialization() : this function made to initialize Buttons as inputs,by making their pins as input pin.
enabling the internal pull-up resistor, and enabling the external interrupt
for the ON/OFF button.*/

void Buttons_initialization(void){
	// Setting the pins as INPUT pins
	clear_bit(DDRPORTB,ON_OFF_PIN);
	clear_bit(DDRPORTB,UP_PIN);
	clear_bit(DDRPORTB,DOWN_PIN);
	
	// Enable internal Pull-Up Resistor
	
	set_bit(PORTB,ON_OFF_PIN);
	set_bit(PORTB,UP_PIN);
	set_bit(PORTB,DOWN_PIN);
	
	
	//  external interrupt (for ON  / OFF button)
	
	// Enable Global Interrupt bit
	sei();
	
	// Trigger with Rising Edge
	set_bit(MCUCSR,ISC2);
	
	// Enable INT2 (External Interrupt)
	set_bit(GICR,INT2);
}


/*UP_pressed() : this Check if UP button is pressed or not and return the result.*/

unsigned char UP_pressed(void)
{
	// Check if pressed for the first time
	if (!(Read_bit(PINB,UP_PIN))){
	
		// wait for 50 ms to make sure it's pressed
		_delay_ms(50);
	}
		// check one more time
		else if (!(Read_bit(PINB,UP_PIN)))
	
			return PRESSED;
		
		else 
		 return NOT_PRESSED;

}
/*DOWN_pressed() :this Check if DOWN button is pressed or not and return the result.*/

unsigned char DOWN_pressed(void){
	// Check if pressed for the first time
	if(!(Read_bit(PINB,DOWN_PIN)))
	{
		// wait for 50 ms to make sure it's pressed
		_delay_ms(50);
	}
		// check one more time
		else if (!(Read_bit(PINB,DOWN_PIN))){
			return PRESSED;
		}
		else return NOT_PRESSED;
	
}

	//////////EEPROM//////////
	
/*To read the data from the EEPROM given the Address.*/

 char EEPROM_read(short address){
	//Setup Address Register
	EEARL = (char)address; //LOW Address
	EEARH = (char)(address>>8); //High Address
	
	// Start Reading and Return Data
	set_bit(EECR,EERE);
}

/* write data in the EEPROM given the address and the data.*/
	
void EEPROM_write(short address,  char data)
{
	unsigned char INT_EN=0;
		
	// Setup Address Register
	EEARL = (char)address;
	EEARH = (char)(address>>8);
	
	// Put Data in Data Register
	EEDR = data;
	
	// Disable Interrupts 
	if((Read_bit(SREG,7)==1)) //SREG:status register
	{
		INT_EN = 1;
		clear_bit(SREG,7);
	}
	
	// Start Writing
	set_bit(EECR,EEMWE);
	set_bit(EECR,EEWE);
	while((Read_bit(EECR,EEWE)==1));
	
	// Back
	if (INT_EN==1)
	{
		set_bit(SREG,7);
	}
	
//////////////////////////TIMER///////////////////////

/*we used timer0:to trigger INT every 100ms to get temp */
/*timer0_init() : to initialize timer(0) to trigger INT every 100ms to get Temp.*/

void timer0_init(){
	
	set_bit(TCCR0,WGM01); //select the mode and register	
	OCR0 = 250; 
	// choose Clock Prescaler (1024) 
	set_bit(TCCR0,CS00);
	set_bit(TCCR0,CS02);
	
	// Enable Timer intrrupt
	sei();
	set_bit(TIMSK,OCIE0);
	
}

/*timer1_init() : to initialize timer(1) to measure time between tasks every 1ms.*/
void timer1_init(){
	
	set_bit(TCCR1B,WGM12); //select the mode and register
	
	// Load Value in OCR1B
	OCR1BH = 0b00000011;
	OCR1BL = 0b11010001;

	//choose Prescaler (1024) ,CS10 1  CS11 0     CS12 1 
	set_bit(TCCR1B,CS10);
	clear_bit(TCCR1B,CS11);
	set_bit(TCCR1B,CS12);
	
	sei();// Enable Global Interrupt 

}

 /*timwe1_interrupt_disable*/
void timer1_intrrupt_disable(){
	
	
	clear_bit(TIMSK,OCIE1B);// Disable Interrupts
}


//timer1_intrrupt_enable() : to start timer(1).

void timer1_intrrupt_enable()
{
	set_bit(TIMSK,OCIE1B);// Enable Interrupts
}

//////////////////////////////ADC//////////////////////////

/*ADC_init (): to initializ the ADC to sense the temperature from the connected sensor */

void ADC_init(){
	
	//  choose the AVCC pin with a capacitor at the AREF pin.
	set_bit(ADMUX,REFS0);
	
	// Choosing Channel One at A0 pin.
	clear_bit(ADMUX,MUX0);
	clear_bit(ADMUX,MUX1);
	clear_bit(ADMUX,MUX2);
	clear_bit(ADMUX,MUX3);
	clear_bit(ADMUX,MUX4);
	
	// Choosing the Data to be (Right) Adjusted in the Data Reg.
	clear_bit(ADMUX,ADLAR);
	
	// Choosing Prescaler (2) for the ADC clock.
	 set_bit(ADCSRA,ADPS0);
	clear_bit(ADCSRA,ADPS1);
	clear_bit(ADCSRA,ADPS2);
	
	// Enable the ADC
	set_bit(ADCSRA,ADEN);
	
}

/*ADC_Read(): to read data from the temperature sensor */

int ADC_read(){
	
	int data;
	
	// Start Conversion
	set_bit(ADCSRA,ADSC);
	
	// Wait for interrupt flag to be set to indicate finishing conversion
	
	while(Read_bit(ADCSRA,ADIF)==0);
	
	// Read Data from the ADC Data Register (LOW First)
	data = ADCL;
	data |= (ADCH<<8);
	
	// Clear Flag (As we are not using the interrupt)
	// It won't be cleared automatically!
	set_bit(ADCSRA,ADIF);
	
	return data;
}
}
///////////////////////////////////////////////SYSTEM//////////////////////////////////////////////////

/*sleep moode:this to save more power when the cpu doesn't need to do any task*/

void sleep_mode(void)
{
//MCUCR -> control register for micro controller unit
// ideal sleep mode
clear_bit(MCUCR,SM0);
clear_bit(MCUCR,SM1);
clear_bit(MCUCR,SM2);

//set the sleep enable
set_bit(MCUCR,SE);
}


/*calculate_average:function to calculate the last 10 measure temperatures*/

int calc_avg_temp(int* arr)
{
int sum = 0;
int avg;

for(int i=0; i<10; i++)
{
sum = sum + arr[i];
}
avg = (int) sum / 10;

return avg;
}


/*change_temp:function to turns on the heater or cooler.*/

void change_temp(void)
{

//our change in temperature is (-+5)

if( (desired_temp < avg_temp + 5) && (desired_temp > avg_temp - 5) )
{
// Actual Temperature equals Desired Temperature(no need for heater or cooler)

heat_stop();
cool_stop();
led_off();
}
else if(desired_temp < avg_temp + 5){
// Actual Temperature is Larger(need the cooler)

heat_stop();
cool_start();
led_on();
}
else if (desired_temp > avg_temp - 5){
// Actual Temperature is Smaller (need the heater)

heat_start();
cool_stop();

if (toggle_f == 1)
{
led_on();
}
else {
led_off();
}
}
}

/* temp_arr_init: function to read the actual first temperature & fill the temperature array*/

void temp_arr_init(void){
int first_temp = ADC_read()*0.5; //multiplied by 0.5 for (the Absolute Accuracy)

for (int i = 0; i<10; i++){
temp_arr[i]= first_temp;
}

avg_temp = calc_avg_temp(temp_arr);
arr_counter = 0;
}
/*add_new_temp: this function is to add new temperature to the temperature array*/
void add_new_temp(int * arr , int new_val){
	int arr_counter;
	for(arr_counter=0;arr_counter<10;arr_counter++)
	{
		arr[arr_counter] = new_val;
	}
	
	if (arr_counter == 9){
		arr_counter = 0;
	}
}

///////*System initialization*///////

void system_init(void)
{
// Modules initialization

ADC_init();
Buttons_initialization();
cool_initialization();
heat_initialization();
led_init();
SS_init();
timer0_init();
timer1_init();
timer1_intrrupt_enable();

// ON_OFF_flag is in OFF mode initially.

Curr_state = OFF;

// setting initial temp. and fill array

if(EEPROM_read(0x0000)!= 0xFF)
{
desired_temp = EEPROM_read(0x0000);
}
else{
desired_temp = INIT_TEMP;
}

temp_arr_init();

// Save desired temp in EEPROM (which is INIT_TEMP)

EEPROM_write(0x0000,(int) desired_temp);

}


/////////////*SETTING MODE*///////

void SET_mode(void)
{
heat_stop();
cool_stop();
led_off();

// to toggle the seven segment while writing the desired temperature.

if(toggle_f == 1)
{
SS_write(desired_temp);
}
else{
SS_turn_off();
}

// if UP or DOWN pressed, change the desired temperature (in the range (35<temp<75))

if((UP_pressed() == PRESSED) && (desired_temp < MAX_TEMP)){
desired_temp = desired_temp + 5;
setting_counter = 0;
SS_write(desired_temp);
}
else if ((DOWN_pressed() == PRESSED) && (desired_temp > MIN_TEMP)){
desired_temp = desired_temp - 5;
setting_counter = 0;
SS_write(desired_temp);
}

EEPROM_write(0x0000,desired_temp);

}

/*ON_mode:this mode waits for the timer to measure temperature by ADC every 100ms & check for UP and DOWN to switch to SET mode*/

void ON_mode(void){

// if 100ms passed and flag is triggered & measure temperature from sensor and add it to the array to get average

if (temp_measure_f == 1){
actual_temp = (ADC_read()*0.5); //multiplied by 0.5 for (the Absolute Accuracy)
add_new_temp(temp_arr , actual_temp);
avg_temp = calc_avg_temp(temp_arr);
temp_measure_f = 0;
}

SS_write(actual_temp);

// turn cooler or heater ON and OFF depending on the average temperature.

change_temp();

// Check if UP or DOWN buttons are pressed to turn on (setting mode)

if(UP_pressed()==PRESSED || DOWN_pressed()==PRESSED){

SS_disable();
setting_counter = 0;
Curr_state = SETTING;

}
}

/*off_mode:to turn off all peripherals and timers*/
void OFF_mode(void){
SS_turn_off();
led_off();
heat_stop();
cool_stop();
sleep_mode();
}

////////////////////////////INTERRUPTS///////////////////////////

/*ON\OFF ISR : to switch between on & off mode*/
 ISR(INT2_vect){
	if(Curr_state==OFF){
		Curr_state = ON;
		desired_temp = EEPROM_read(0x0000);
	}
	else if (Curr_state==ON || Curr_state==SETTING){
		Curr_state = OFF;
	}
}

/*Timer_ISR(A): make the temperature measure flag equal 1 to enable the ADC*/
 ISR(TIMER1_COMPA_vect)
{
	temp_measure_f = 1;
}


int main(void){
	
	system_init();
	
	while(1){
		switch (Curr_state){
			case ON:{
				ON_mode();
				break;
			}
			case OFF:{
				OFF_mode();
				break;
			}
			case SETTING:{
				SET_mode();
				break;
			}
		}
	}
}