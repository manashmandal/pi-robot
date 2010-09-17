#include <pololu/3pi.h>
#include <avr/pgmspace.h>

#define MOTOR_SPEED 100


const char charge[] PROGMEM = "v12 L16 o4 cfa>cra>c4";
const char oops[] PROGMEM = "v12 L16 o4 f";//rc32<b32c32c#8cr8.erf";
const char doh[] PROGMEM = "v12 L16 o4 c8.e8f#8ag8.e8c8<a<f#<f#<f#<g";

int get_value_from_user(char *msg, int default_value, int increment);

/********************************************************************
The main function
********************************************************************/

int main()
{
	//Initializing the sensors
	set_analog_mode(MODE_8_BIT);
	DDRC &= ~(1<< PORTC5);
	PORTC &= ~(1<< PORTC5);

	//Play a nice tone
	play_from_program_space(oops);
	
	//Some PID related variable initialization
	int Kp = get_value_from_user("Kp=?    ", 1, 1);
	int Ki = get_value_from_user("Ki=?    ", 0, 1);
	int Kd = get_value_from_user("Kd=?    ", 2, 1);

	int p;
	int i = 0;
	int d;
	int pid;

	//Wait till user presses B button
	clear();
	lcd_goto_xy(0,0);
	print("Press B");

	while(!button_is_pressed(BUTTON_B));
	
	delay_ms(1000);

	//Record the initial value of the side sensor
	int initial_side_sensor = analog_read(6);

	int previous_side_sensor = 0;

	while(1)
	{

		//Read the side sensor value
		int side_sensor = analog_read(6);

		//Read the front sensor value
		int front_sensor = analog_read(7);

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
	
		set_motors(MOTOR_SPEED + pid ,MOTOR_SPEED - pid);

		//While the front sensor reades value grater than 55 then turn right
		while (front_sensor > 55)
		{
			//Turn right
			set_motors(MOTOR_SPEED,-MOTOR_SPEED);

			front_sensor = analog_read(7);
		}

		// store previous side sensor value
		previous_side_sensor = side_sensor;

	}//end while(1)

}//end main


/*******************************************************************
//This function gets value from user.
********************************************************************/
int get_value_from_user(char *msg, int default_value, int increment)
{
    int value = default_value;

    while(1)
    {
        clear();

        // print name
        lcd_goto_xy(0,0);
        print(msg);
        print("=?");

        // print current value
        lcd_goto_xy(0,1);
        print_long(value);
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
                value = -32000 + (32000 % increment);
        }
        if(value > 32000)
        {
               value = 32000 - (32000 % increment);
        }
    }
}

/******************************************************************/
