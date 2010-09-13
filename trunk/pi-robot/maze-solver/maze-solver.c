#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

#define BASE_SPEED 50
#define FORWARD_SENSOR_THRESHOLD 60
//#define LEFT_SENSOR_TARGET 60

#define P_GAIN 1/20
#define D_GAIN 2/3

int main()
{
	// initialize sensors and wait for button press
    init_sensors();
	wait_with_message("Maze 2.1");

	//instead of a static value, the robot determines 
	//what's the initial distance at the begining
	int left_sensor_target = read_left_sensor();

    int previous_left_sensor = 0;

    while(1)
    {

		// read
        int left_sensor = read_left_sensor(); // 0 (far away) - 160 (I think close at 8 bit)

		// calculate PD-controller value
        int p = left_sensor - left_sensor_target;
        int d = left_sensor - previous_left_sensor;

        int pd = p * P_GAIN + d * D_GAIN;

		//If the pd is higher or lower than base speed 
		//it will stop the corrosponding wheels
		if (pd > BASE_SPEED)
		{
			pd = BASE_SPEED;
		}else if (pd < -BASE_SPEED)
		{
			pd = -BASE_SPEED;
		}


		if(get_ms() % 250 == 0)
		{
			print_num_xy(0,0,left_sensor,1);
			print_num_xy(0,1,pd,0);
		}

		// set new motor speeds
        int left_motor_speed = BASE_SPEED + pd;
        int right_motor_speed = BASE_SPEED - pd;
		
        set_motors(left_motor_speed, right_motor_speed);

		// store previous distance
        previous_left_sensor = left_sensor;
		
		
    }//end while

}//end main
