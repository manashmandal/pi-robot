#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

/********************************************************************
The main function
********************************************************************/

int main()
{
	init_sensors();

	while(1)
	{
		//Read the sensor values
		int left = read_left();
		int front = read_front();
		int right = read_right();

		//Print the sensor values to screen
		//+-----------+
		//|   FRONT   |
		//|LEFT  RIGHT|
		//+-----------+
		if((get_ms() % 300) == 0)
		{
			clear();
			lcd_goto_xy(2,0);
			print_long(front);
			lcd_goto_xy(0,1);
			print_long(left);
			lcd_goto_xy(5,1);
			print_long(right);
		}
	}//end while(1)

}//end main

/******************************************************************/
