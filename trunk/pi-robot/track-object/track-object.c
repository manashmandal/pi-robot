#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>
main()
{
	init_sensors();
	wait_with_message("Track");
	
	while(1)
	{
		int front_sensor = analog_read(7);
		int left_sensor = analog_read(6);
		int right_sensor = analog_read(5);

		int left_balance = front_sensor - left_sensor;
		int right_balance = front_sensor - right_sensor;

		if(get_ms() % 300 == 0)
		{
			clear();
			lcd_goto_xy(0,0);
			print_long(left_balance);
			lcd_goto_xy(4,0);
			print_long(right_balance);

			lcd_goto_xy(0,1);
			print("power");
		}	
		
		set_motors(left_balance, right_balance);	

	}	
}
