#include <pololu/3pi.h>

//Delay required for the robot to turn 1 degree in 1 motor power
#define DELAY_PER_ANGLE_PER_POWER 218.88

//Sensor numbers
#define SENSOR_5 5
#define SENSOR_6 6
#define SENSOR_7 7

//Initializes the digital sensors
void init_sensors()
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

// Back up slightly to the left
void back_up_left()
{
	set_motors(-50,-90);
}

// Back up slightly to the right
void back_up_right()
{
	set_motors(-90,-50);
}

// Turn to the right in place
void turn_right_in_place()
{
	set_motors(50, -50);
}

// Turn to the left in place
void turn_left_in_place()
{
	set_motors(-50, 50);
}

//Scans 360 degree and reports an array of distance data
int* scan_360(int sensor)
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

//Read forward sensor data
int read_forward_sensor()
{
	return analog_read(SENSOR_6);
}

//Read left sensor data
int read_left_sensor()
{
	return analog_read(SENSOR_7);
}

//Read right sensor data
int read_right_sensor()
{
	return analog_read(SENSOR_5);
}
