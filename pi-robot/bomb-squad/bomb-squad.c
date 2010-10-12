#include <pololu/3pi.h>

#define DELAY_PER_ANGLE_PER_POWER 218.88

int main()
{
	// wait
	wait_with_message("Press B");

	// init and calibrate light sensors
	unsigned int sensors[5];
	pololu_3pi_init_disable_emitter_pin(2000); // (2000 for the timeout corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor)
	wait_for_button_release(BUTTON_B);
	delay_ms(1000);
	for(int counter=0;counter<80;counter++)
	{
		if(counter < 20 || counter >= 60)
			set_motors(40,-40);
		else
			set_motors(-40,40);

		calibrate_line_sensors(IR_EMITTERS_ON);

		delay_ms(20);
	}
	set_motors(0,0);


	// init IR sensors
	set_analog_mode(MODE_8_BIT);
	DDRC &= ~(1<< PORTC5);
	PORTC &= ~(1<< PORTC5);

	// wait
	wait_with_message("Press B");
	delay_ms(1000);

	int left_speed = 110;
	int right_speed = 110;
	int set_point = 0;

	while(1)
	{
		// check light sensors for our boundary
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		if(position > 5 && position < 1000)
		{
			turn_right(55,65);
		}
		else if(position > 1000 && position < 1800)
		{
			turn_right(55,95);
		}
		else if(position > 1800 && position < 3000)
		{
			turn_left(55,95);
		}
		else if(position > 3000 && position < 3995)
		{
			turn_left(55,65);
		}

		// check IR sensors
		int left = analog_read(6);
		int right = analog_read(5);
		int front = analog_read(7);

		/*if((get_ms() % 300) == 0)
		{
			clear();
			lcd_goto_xy(0,0);
			print_long(left);
			lcd_goto_xy(0,1);
			print_long(right);
		}*/

		int balance = 0;
		if (left > 20 || right > 20)
		{
			balance = right - left - 20;
		}

		if (set_point == 0 && front > 162)
		{
			set_point = 1;
			set_motors(25,25);
		}
		else
		{
			set_motors(left_speed + balance,right_speed - balance);
		}

	}


	halt();

	clear();
	//print("f=");
	//print_long(front);

	// end
	while(1);
}

//////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////
void turn_left(int power, int angle)
{
	int delay = (DELAY_PER_ANGLE_PER_POWER * angle)/power;

	set_motors(-power,power);
	delay_ms(delay);
	set_motors(0,0);
}

//////////////////////////////////////////////////////////////////
void turn_right(int power, int angle)
{
	int delay = (DELAY_PER_ANGLE_PER_POWER * angle)/power;

	set_motors(power,-power);
	delay_ms(delay);
	set_motors(0,0);
}

