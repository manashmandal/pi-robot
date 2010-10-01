#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <common-libs/commonfunc.h>

#define DISTANCE 60

main()
{

	init_sensors();
	wait_with_message("Bomb 0.6");
	pololu_3pi_init(2000);

	//Start tracking
	int front_sensor = 0;
	set_motors(-50,50);
	
	long entered_object = 0;
	long left_object = 0;
	while (1)
	{
		front_sensor = analog_read(7);

		if (front_sensor > DISTANCE )
		{
			break;
		}
	}


	//Move to the object
	set_motors(0,0);
	delay_ms(300);

	set_motors(50,50);


}
