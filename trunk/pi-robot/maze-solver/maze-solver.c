#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <common-libs/commonfunc.h>

//#define MOTOR_SPEED 125


const char charge[] PROGMEM = "v12 L16 o4 cfa>cra>c4";
const char oops[] PROGMEM = "v12 L16 o4 f";//rc32<b32c32c#8cr8.erf";

float get_float_from_user(char *msg, float default_value, float increment);

/********************************************************************
The main function
********************************************************************/

int main()
{
	//Initializing the sensors
	init_sensors();

	//Play a nice tone
	play_from_program_space(oops);
	while(is_playing());
	
	//Some PID related variable initialization
	int MOTOR_SPEED = 100;//get_int_from_user("Speed=? ", 100, 1);
	float Kp = 0.7;//get_float_from_user("Kp=0.0? ", 0.7, 0.01);
	float Ki = 0.0;//get_float_from_user("Ki=0.0? ", 0.0, 0.01);
	float Kd = 0.9;//get_float_from_user("Kd=0.0? ", 0.9, 0.01);
	int FRONT_DISTANCE = get_int_from_user("Front=? ", 55, 5);
	int p;
	int i = 0;
	int d;
	int pid;
	
	//Wait till user chooses left or right
	clear();
	lcd_goto_xy(0,0);
	print("A=left");
	lcd_goto_xy(0,1);
	print("C=right");
	char button = wait_for_button_press(ANY_BUTTON);
	int follow_left = 1; // default = left
	if(button & BUTTON_C)
		follow_left = 0;
	wait_for_button_release(button);;
	
	//Wait till user presses B button
	clear();
	lcd_goto_xy(0,0);
	print("Press B");
	while(!button_is_pressed(BUTTON_B));
	delay_ms(1000);
	//play_from_program_space(charge);
	//while(is_playing());

	//Record the initial value of the side sensor
	int initial_side_sensor = follow_left ? read_left() : read_right();
	int previous_side_sensor = 0;
	
	while(1)
	{
		//Read the side sensor value
		int side_sensor = follow_left ? read_left() : read_right();

		//Read the front sensor value
		int front_sensor = read_front();

		//Print the sensor values to screen
		if((get_ms() % 300) == 0)
		{
			clear();
			lcd_goto_xy(0,0);
			print_long(front_sensor);
			lcd_goto_xy(5,0);
			print_long(side_sensor);
		}

		//Calculating PID from side sensor
		pid = 0;

		p = side_sensor - initial_side_sensor;
		i = i + p;
		d = side_sensor - previous_side_sensor;
		pid = (Kp * p) + (Ki * i) + (Kd * d);
		
		int L_speed = follow_left ? MOTOR_SPEED + pid : MOTOR_SPEED - pid;
		int R_speed = follow_left ? MOTOR_SPEED - pid : MOTOR_SPEED + pid;
		set_motors(L_speed ,R_speed);

		//While the front sensor reades value grater than 55 then turn right
		while (front_sensor > FRONT_DISTANCE)
		{
			//Turn
			int L_speed = follow_left ? MOTOR_SPEED : -MOTOR_SPEED;
			int R_speed = follow_left ? -MOTOR_SPEED : MOTOR_SPEED;

			set_motors(L_speed,R_speed);

			front_sensor = read_front();
		}

		// store previous side sensor value
		previous_side_sensor = side_sensor;

	}//end while(1)

}//end main


/*******************************************************************
//This function gets value from user.
********************************************************************/
float get_float_from_user(char *msg, float default_value, float increment)
{
    float value = default_value;

    while(1)
    {
        clear();

        // print name
        lcd_goto_xy(0,0);
        print(msg);
        print("=?");

        // print current value
        lcd_goto_xy(0,1);
        print_long(value * 1/increment);
		print("/100");
        lcd_show_cursor(CURSOR_BLINKING);

        // wait for all buttons to be released, then a press
        while(button_is_pressed(ANY_BUTTON));
        char button = wait_for_button_press(ANY_BUTTON);

        if(button & BUTTON_A)
        {
                play("!c32");
                value = value - increment;
        }
        else if(button & BUTTON_B)
        {
                lcd_hide_cursor();
                clear();
                play("!e32");
                delay_ms(100);
                return value;
        }
        else if(button & BUTTON_C)
        {
                play("!g32");
                value = value + increment;
        }

        if(value < -32000)
        {
                value = -32000 + (32000 % (int)increment);
        }
        if(value > 32000)
        {
               value = 32000 - (32000 % (int)increment);
        }
    }
}

/******************************************************************/
