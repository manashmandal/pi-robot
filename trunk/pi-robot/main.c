/*
 * 
 * Authors: Kaushik Hazra, Matt Odille
 *
 */
#include "pirobot.h"
#include "line.h"
#include "maze.h"
#include "sumo.h"
#include "wall.h"

// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.
#include <pololu/3pi.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

void run_line()
{
	line_main();
}

void run_wall()
{
	clear();
	print("wall n/a");
	delay_ms(5000);
}

void run_sumo()
{
	clear();
	print("sumo n/a");
	delay_ms(5000);
}

void run_maze()
{
	clear();
	print("maze n/a");
	delay_ms(5000);
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
	// This must be called at the beginning of 3pi code, to set up the
	// sensors.  We use a value of 2000 for the timeout, which
	// corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
	pololu_3pi_init(2000);
	load_custom_characters(); // load the custom characters
	
	play_from_program_space(welcome);
	delay_ms(1000);
}

void menu_select()
{
	static int menu_index = 0;

	print_two_lines_delay_1s(main_menu_intro_line1,main_menu_intro_line2);

	while(1)
	{
		clear();
		lcd_goto_xy(0,1);
		print_from_program_space(menu_line2);
		lcd_goto_xy(0,0);
		print_from_program_space(main_menu_options[menu_index]);
		lcd_show_cursor(CURSOR_BLINKING);
		// the cursor will be blinking at the end of the option name
	
		// wait for all buttons to be released, then a press
		while(button_is_pressed(ANY_BUTTON));
		char button = wait_for_button_press(ANY_BUTTON);

		if(button & BUTTON_A)
		{
			play_from_program_space(beep_button_a);
			menu_index --;
		}
		else if(button & BUTTON_B)
		{
			lcd_hide_cursor();
			clear();

			play_from_program_space(beep_button_b);
			wait_for_button_release(button);

			while(!button_is_pressed(BUTTON_B))
			{
				lcd_goto_xy(0,1);
				print_from_program_space(back_line2);
				lcd_goto_xy(0,0);
				main_menu_functions[menu_index]();
			}

			set_motors(0,0);
			stop_playing();
			m1_speed = 0;
			m2_speed = 0;
			red_led(0);
			green_led(0);
			play_from_program_space(beep_button_b);

			return;
		}
		else if(button & BUTTON_C)
		{
			play_from_program_space(beep_button_c);
			menu_index ++;
		}

		if(menu_index < 0)
			menu_index = main_menu_length-1;
		if(menu_index >= main_menu_length)
			menu_index = 0;
	}
}

// This is the main function, where the code starts.  All C programs
// must have a main() function defined somewhere.
int main()
{
	// set up the 3pi
	initialize();

	// This is the "main loop" - it will run forever.
	while(1)
	{
		menu_select();
	}
}

// Local Variables: **
// mode: C **
// c-basic-offset: 4 **
// tab-width: 4 **
// indent-tabs-mode: t **
// end: **
