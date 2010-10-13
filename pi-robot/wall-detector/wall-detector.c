#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

int main()
{
	init_sensors();

	int SET_POINT_VALUE = get_int_from_user("SetPnt=?", 30, 5);
	int END_POINT_VALUE = get_int_from_user("EndPnt=?", 145, 1);

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

		left_speed = (front < SET_POINT_VALUE) ? 110 - (1.0 * 0.54838709677419354838709677419355 * front) : 25;
		right_speed = (front < SET_POINT_VALUE) ?  110 - (1.0 * 0.54838709677419354838709677419355 * front) : 25;

		if (set_point == 0 && front > END_POINT_VALUE)
		{
			set_point = 1;
			set_motors(25,25);
		}
		else
		{
			set_motors(left_speed + balance,right_speed - balance);
		}

		if (set_point == 1 && front < END_POINT_VALUE)
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
