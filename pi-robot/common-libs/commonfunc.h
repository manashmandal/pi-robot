#include <pololu/3pi.h>

//Delay required for the robot to turn 1 degree in 1 motor power
#define DELAY_PER_ANGLE_PER_POWER 218.88

//Sensor numbers
#define SENSOR_5 5
#define SENSOR_6 6
#define SENSOR_7 7

#define SENSOR_ONE 5
#define SENSOR_TWO 6
#define SENSOR_FRONT 7

//Initializes the digital sensors
//MODE_8_BIT => 0=far away, 150=close
void init_sensors()
{
	set_analog_mode(MODE_8_BIT);
	DDRC &= ~(1<< PORTC5);
	PORTC &= ~(1<< PORTC5);
}

//Displays a message and waits for B button to press
void wait_with_message(char *str)
{
	 play_frequency(200, 50, 14);
	 delay_ms(75);
	 play_frequency(300, 50, 14);

	 play_frequency(200, 50, 14);
	 delay_ms(75);
	 play_frequency(300, 50, 14);

	lcd_goto_xy(0,0);
	print(str);

	while(!button_is_pressed(BUTTON_B));
	delay_ms(500);
}

int get_value_from_user(char *msg, int default_value, int increment)
{
	int value = default_value;

	while(1)
	{
		clear();

		// print name
		lcd_goto_xy(0,0);
		print(msg);
		print("=?");

		// print current value
		lcd_goto_xy(0,1);
		print_long(value);
		lcd_show_cursor(CURSOR_BLINKING);

		// wait for all buttons to be released, then a press
		while(button_is_pressed(ANY_BUTTON));
		char button = wait_for_button_press(ANY_BUTTON);

		if(button & BUTTON_A)
		{
			play("!c32");
			value = value - increment;
		}
		else if(button & BUTTON_B)
		{
			lcd_hide_cursor();
			clear();
			play("!e32");
			delay_ms(100);
			return value;
		}
		else if(button & BUTTON_C)
		{
			play("!g32");
			value = value + increment;
		}

		if(value < -32000)
		{
			value = -32000 + (32000 % increment);
		}
		if(value > 32000)
		{
			value = 32000 - (32000 % increment);
		}
	}
}

int get_target_difference()
{

	int s1 = 0;
	int s2 = 0;

	while(!button_is_pressed(BUTTON_B))
	{
		if(get_ms() % 500 == 0)
		{
			clear();
			print_xy(0,0,"AlignBot");
			lcd_goto_xy(0,1);
			print_long(read_sensor(SENSOR_ONE)-read_sensor(SENSOR_TWO));
		}
	}

	s1 = read_sensor(SENSOR_ONE); // 0 (far away) - 150 (close)
	s2 = read_sensor(SENSOR_TWO); // 0 (far away) - 150 (close)

	play(">g32>>c32");

	return s1-s2;


}


//Stop both motors
void halt()
{
	set_motors(0,0);
}

//Dash forward
void dash(int power, int delay)
{
	set_motors(power,power);
	delay_ms(delay);
	set_motors(0,0);
}

//Turn the robot to left
void turn_left(int power, int angle)
{
	int delay = (DELAY_PER_ANGLE_PER_POWER * angle)/power;

	set_motors(-power,power);
	delay_ms(delay);
	set_motors(0,0);
}

//Turn the robot to right
void turn_right(int power, int angle)
{
	int delay = (DELAY_PER_ANGLE_PER_POWER * angle)/power;

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
	int i, *pdata, data[25];
	int power = 40;

	for(i=0; i<25; i++)
	{
		// rotate LEFT 15 degrees then stop
		int delay = (DELAY_PER_ANGLE_PER_POWER * 15)/power;
		set_motors(-power,power);
		delay_ms(delay);
		set_motors(0,0);

		// scan forward and store value
		data[i] = analog_read(SENSOR_6);
	}

	pdata = &data[0];
	return pdata;
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

int read_sensor(int sensor)
{
	return analog_read(sensor);
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

void print_xy( int row, int col, char *message, int doClear)
{
	if(doClear)
	{
		clear();
	}
	lcd_goto_xy(row,col);
	print(message);
}

void print_num_xy( int row, int col, long num, int doClear)
{
	if(doClear)
	{
		clear();
	}
	lcd_goto_xy(row,col);
	print_long(num);
}
