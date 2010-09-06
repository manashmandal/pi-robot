#include <pololu/3pi.h>

//Delay required for the robot to turn 1 degree in 1 motor power
#define DELAY_PER_ANGLE_PER_POWER 218.88

//Sencer numbers
#define SENCER_5 5
#define SENCER_6 6
#define SENCER_7 7

//Initializes the digital sencers
void init_sencers()
{
	DDRC &= ~(1<< PORTC5);
	PORTC &= ~(1<< PORTC5);
}

//Displayes a message and waits for B button to press
void wait_with_message(char *str)
{
	play("L16 ceg>c");

	lcd_goto_xy(0,0);
	print(str);

	while(!button_is_pressed(BUTTON_B));
}

//Turn the robot to left
void turn_left(int power)
{
	int delay = (DELAY_PER_ANGLE_PER_POWER * 90)/power;

	set_motors(-power,power);
	delay_ms(delay);
	set_motors(0,0);
}

//Turn the robot to left
void turn_right(int power)
{
	int delay = (DELAY_PER_ANGLE_PER_POWER * 90)/power;

	set_motors(power,-power);
	delay_ms(delay);
	set_motors(0,0);
}

//Scans 360 degree and reports an array of distance data
int* scan_360(int sencer)
{
	int data[10];

	set_motors(-40,40);

	int delay = (DELAY_PER_ANGLE_PER_POWER * 360)/40;

	delay_ms(1970);

	set_motors(0,0);
	return data;
}

//Display a countdown for count, increment by one second
void count_down(int count)
{
	int i = 1;
	clear();
	while(i <= count)
	{
		lcd_goto_xy(0,0);
		print_long(i);
		delay_ms(1000);
		clear();
		i++;
	}
}

//Read forward sencer data
int read_forward_sencer()
{
	return analog_read(SENCER_6);
}

//Read left sencer data
int read_left_sencer()
{
	return analog_read(SENCER_7);
}

//Read right sencer data
int read_right_sencer()
{
	return analog_read(SENCER_5);
}
