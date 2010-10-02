#include <pololu/3pi.h>
#include <common-libs/commonfunc.h>

#define TIER_1_SPEED 110
#define TIER_1_DISTANCE 20

#define TIER_2_SPEED 100
#define TIER_2_DISTANCE 45

#define TIER_3_SPEED 60
#define TIER_3_DISTANCE 55

#define TIER_4_SPEED 25
#define TIER_4_DISTANCE 70

#define TIER_5_SPEED 25
#define TIER_5_DISTANCE 140

#define TIER_6_SPEED 25
//#define TIER_6_DISTANCE 125

int main2()
{
	//wait_with_message("Press B");
	init_sensors();

	//int TIER_6_SPEED = get_value_from_user("6speed=?",25, 1);
	int TIER_6_DISTANCE = get_value_from_user("6dist=? ",135, 1);

	count_down(2);
 
	// Tier 1
	set_motors(TIER_1_SPEED,TIER_1_SPEED);
	while(analog_read(7) < TIER_1_DISTANCE);

	// Tier 2
	set_motors(TIER_2_SPEED,TIER_2_SPEED);
	while(analog_read(7) < TIER_2_DISTANCE);

	// Tier 3
	set_motors(TIER_3_SPEED,TIER_3_SPEED);
	while(analog_read(7) < TIER_3_DISTANCE);

	// Tier 4
	set_motors(TIER_4_SPEED,TIER_4_SPEED);
	while(analog_read(7) < TIER_4_DISTANCE);

	// Tier 5
	set_motors(TIER_5_SPEED,TIER_5_SPEED);
	while(analog_read(7) < TIER_5_DISTANCE);

	// Tier 6
	set_motors(TIER_6_SPEED,TIER_6_SPEED);
	while(analog_read(7) > TIER_6_DISTANCE);
	halt();

	// finally, blindly go a liiiiiittle bit closer
	delay_ms(250);
	//set_motors(25, 25);
	//delay_ms(350);
	//halt();

	while(1)
	{
		int front = analog_read(7);
		int left  = analog_read(6);

		if(get_ms() % 300 == 0)
		{
			clear();
			lcd_goto_xy(0,0);
			print_long(front);
			lcd_goto_xy(0,1);
			print_long(left);
			print("  ");
			print_long(front - left);

		}
	}

	// Finish
	while(1)
	{
		int front = analog_read(7);
		int left  = analog_read(6);
		int right = analog_read(5);

		// break if too close
		if(analog_read(7) >= TIER_5_DISTANCE)
		{
			halt();

			// blindly go a liiiiiittle bit closer
			//set_motors(left_balance, right_balance);
			//delay_ms(20);
			break;
		}

		int left_balance  = front - left;
		int right_balance = front - right;

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

	// end
	while(1);
}
