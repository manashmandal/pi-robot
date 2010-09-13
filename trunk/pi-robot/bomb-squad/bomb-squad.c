#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

#define TARGET_RANGE_MIN 10
//#define TARGET_RANGE_MAX 115
#define SENSOR_MAX 150

int main()
{
	// initialize sensors and wait for button press
    init_sensors();
	wait_with_message("Bomb 0.1");
	
/*
	int i, distances[25];
	int power = 40;
	int max = 0;
	int max_index = 0;

	while(1)
	{
		// scan all around
		clear();		
		for(i=0; i<25; i++)
		{
			// rotate LEFT 15 degrees then stop
			int delay = (DELAY_PER_ANGLE_PER_POWER * 15)/power;
			set_motors(-power,power);
			delay_ms(delay);
			set_motors(0,0);
		
			// scan forward and store value
			delay_ms(250);
			distances[i] = analog_read(SENSOR_6);
			delay_ms(250);
			
			// print curr value
			lcd_goto_xy(0,0);
			print_long(i);
			print("=");
			lcd_goto_xy(0,1);
			print_long(distances[i]);
			
			// update max, if necessary
			if(distances[i] > max)
			{
				max = distances[i];
				max_index = i;
			}
		}

		// now rotate to proper angle
		turn_left(40, 15*max_index);

		// dash forward to push the ball
		delay_ms(1000);
		set_motors(80,80);
		delay_ms(1500);
		halt();
		while(1);

	}
*/
	int TARGET_RANGE_MAX = get_value_from_user("MAX");
	delay_ms(350);

	int dist = 0;
	int previous_success = 0;

    while(1)
    {
		// rotate right 90 degrees
		turn_right(40, 90);

		// scan until object detected
		dist = SENSOR_MAX - read_forward_sensor();
		while(dist > TARGET_RANGE_MAX && !previous_success)
		{
			clear();
			print_long(dist);

			// check for success
			if(dist > TARGET_RANGE_MAX)
			{
				previous_success = 0;
			}
			else
			{
				previous_success = 1;
			}
			
			// rotate some
			delay_ms(250);
			turn_left(40, 10);
			delay_ms(250);

			// read again
			dist = SENSOR_MAX - read_forward_sensor();
		}
		
		// print last value
		clear();
		print_long(dist);

		// rotate a bit more
		delay_ms(250);
		turn_left(40, 20);
		delay_ms(250);
		
		// dash forward
		delay_ms(1000);
		set_motors(80,80);
		delay_ms(1500);
		halt();

		while(1);

    }//end while

}//end main
