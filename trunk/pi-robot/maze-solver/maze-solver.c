#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

#define BASE_SPEED 200
#define FORWARD_SENSOR_THRESHOLD 50
#define LEFT_SENSOR_TARGET 100

#define P_GAIN (1/3)
#define D_GAIN 20

int main()
{
	// initialize sensors and wait for button press
    init_sensors();
	wait_with_message("Maze");

    int previous_left_sensor = 0;

    while(1)
    {
        // check for wall in front
        int forward_sensor = read_forward_sensor();
        if (forward_sensor < FORWARD_SENSOR_THRESHOLD)
		{
            turn_right_in_place();
            continue;
        }

		// read
        int left_sensor = read_left_sensor(); // 0 (far away) - 650 (close)

		// calculate PD-controller value
        int p = left_sensor - LEFT_SENSOR_TARGET;
        int d = left_sensor - previous_left_sensor;
        int pd = (p * P_GAIN) + (d * D_GAIN);

		// set new motor speeds
        int left_motor_speed = BASE_SPEED + pd;
        int right_motor_speed = BASE_SPEED - pd;
        set_motors(left_motor_speed, right_motor_speed);

		// store previous distance
        previous_left_sensor = left_sensor;
		
        // after every 14 seconds back up for 1 second (in case it gets stuck)
        //if (get_ms() % 15000 > 14000)
		//{
        //    back_up_left();
        //    continue;
        //}
		
    }//end while

}//end main
