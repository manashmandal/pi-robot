#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

/* 2-sensor layout is as follows:
 *
 *  | forward   ^
 *  |           |
 * W|    [ SENSOR_FRONT ]
 * A| <--[SENSOR_ONE    ]
 * L| <--[SENSOR_TWO    ]
 * L|
 *  | backwards
 */

//#define BASE_SPEED 50
//#define FORWARD_SENSOR_THRESHOLD 120
//#define DIFFERENCE_THRESHOLD -30

//#define P_GAIN 1
//#define D_GAIN 1

int main()
{
	// initialize sensors and wait for button press
    init_sensors();
	wait_with_message("Maze 3.4");

	// prompt for base speed
	int BASE_SPEED = 50;//get_value_from_user("speed=? ", 50, 10);
	int FORWARD_SENSOR_THRESHOLD = 100;//get_value_from_user("fwd max=?", 100, 10);
	int DIFFERENCE_THRESHOLD = -40;//get_value_from_user("diff min=", -40, 5);

	int dash1 = 475;//get_value_from_user("dash1=? ", 475, 25);
	int dash2 = 875;//get_value_from_user("dash2=? ", 875, 25);

	int pnum = get_value_from_user("P num=? ", 1, 1);
	int pden = get_value_from_user("Pdenom=?", 1, 1);
	int dnum = get_value_from_user("D num=? ", 1, 1);
	int dden = get_value_from_user("Ddenom=?", 1, 1);

	float P_GAIN = pnum / pden;
	float D_GAIN = dnum / dden;

	int angle_for_left_90 = 80;//get_value_from_user("angle90=", 80, 5);

	int target_difference = get_target_difference();
	delay_ms(777);

    int previous_difference = 0;

    while(1)
    {
		// read left sensors
		int s1 = read_sensor(SENSOR_ONE); // 0 (far away) - 150 (close)
		int s2 = read_sensor(SENSOR_TWO); // 0 (far away) - 150 (close)
        int difference = s1 - s2;

		// print sensor readings periodically
		if(get_ms() % 250 == 0)
		{
			print_num_xy(0,0,s1,1);
			print_num_xy(0,1,s2,0);
		}

		// if there's no wall to left, perform 90 degree left turn
		if(difference < DIFFERENCE_THRESHOLD)
		{
			// stop
			clear();
			print("Go Left");
			play(">g32>>c32");
			halt();
			//delay_ms(1000);

			// go forward a bit
			dash(50, dash1);
			//delay_ms(1000);

			// perform 90 degree turn
			turn_left(BASE_SPEED, angle_for_left_90);
			//delay_ms(1000);

			// go forward a bit
			dash(50, dash2);
			//delay_ms(1000);

			// continue...
			previous_difference = 0;
		}
		// if there's a wall directly in front of us,  perform 90 degree right turn
		else if(read_sensor(SENSOR_FRONT) > FORWARD_SENSOR_THRESHOLD)
		{
			// stop
			clear();
			print("Go Right");
			play(">c32>>g32");
			halt();

			// perform 90 degree turn
			turn_right(BASE_SPEED, 90);

			// continue...
			previous_difference = 0;
		}
		else
		{
			// calculate PD-controller value
	        int p = difference - target_difference;
	        int d = difference - previous_difference;

	        int pd = p * P_GAIN + d * D_GAIN;

			//If the pd is higher or lower than base speed
			//it will stop the corresponding wheels
			//if (pd > BASE_SPEED)
			//{
			//	pd = BASE_SPEED;
			//}else if (pd < -BASE_SPEED)
			//{
			//	pd = -BASE_SPEED;
			//}

			// set new motor speeds
			/*if (difference < target_difference){
				pd = - BASE_SPEED / 4;
			}
			else if (difference > target_difference)
			{
				pd = BASE_SPEED / 4;
			}
			*/
	        int left_motor_speed = BASE_SPEED + pd;
	        int right_motor_speed = BASE_SPEED - pd;
	        set_motors(left_motor_speed, right_motor_speed);

			// store previous difference, for derivative calculation
	        previous_difference = difference;
		}

    }//end while

}//end main
