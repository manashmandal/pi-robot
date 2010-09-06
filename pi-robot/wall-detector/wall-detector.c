#include <commonfunc.h>

main()
{
	while(1)
	{
		wait_with_message("Prss B");

		count_down(3);

		init_sencers();
		
		set_motors(40,40);

		int minimum_distance = 600;

		while(1)
		{
			int forward_sencer = read_forward_sencer();
			int left_sencer = read_left_sencer();
			int right_sencer = read_right_sencer();

			if (forward_sencer >= minimum_distance 
				|| left_sencer >= minimum_distance 
				|| right_sencer >= minimum_distance)
			{
				set_motors(0,0);
				break;
			}

		}
	}
}
