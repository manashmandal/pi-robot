#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

int main()
{
	init_sensors();
	wait_with_message("Press B");
	count_down(2);

	
	int front = 0;
	int left = 0;
	int right = 0;
	int balance = 0;

	int left_speed = 0;
	int right_speed = 0;

	int set_point = 0;

	while(1)
	{
		left = analog_read(6);
		right = analog_read(5);
		front = analog_read(7);
		
		balance = 0;
		if (left > 20 || right > 20)
		{
			balance = right - left - 20;
		}

		left_speed = 110 - (0.54838709677419354838709677419355 * front);
		right_speed = 110 - (0.54838709677419354838709677419355 * front);

		if (set_point == 0 && front > 162)
		{
			set_point = 1;
			set_motors(25,25);
		}
		else
		{
			set_motors(left_speed + balance,right_speed - balance);
		}

		if (set_point == 1 && front < 150)
		{
			break;
		}


	}


	halt();

	clear();
	print("f=");
	print_long(front);

	// end
	while(1);
}
