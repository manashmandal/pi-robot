
// Introductory messages.  The "PROGMEM" identifier causes the data to
// go into program space.
const char welcome_line1[] PROGMEM = "Starting";
const char welcome_line2[] PROGMEM = " \xf7Robot";

const char main_menu_intro_line1[] PROGMEM = "  Main";
const char main_menu_intro_line2[] PROGMEM = "  Menu";

const char menu_line[] PROGMEM = "Line";
const char menu_wall[] PROGMEM = "Wall";
const char menu_sumo[] PROGMEM = "Sumo";
const char menu_maze[] PROGMEM = "Maze";

const char menu_line2[] PROGMEM = "\x7f" "A \xa5" "B C\x7e";
const char back_line2[] PROGMEM = "\6B";

void run_line();
void run_wall();
void run_sumo();
void run_maze();
typedef void (*function)();
const function main_menu_functions[] = { run_line, run_wall, run_sumo, run_maze };
const char *main_menu_options[] = { menu_line, menu_wall, menu_sumo, menu_maze };
const char main_menu_length = sizeof(main_menu_options)/sizeof(main_menu_options[0]);

// A couple of simple tunes, stored in program space.
const char welcome[] PROGMEM = ">g32>>c32";
const char beep_button_a[] PROGMEM = "!c32";
const char beep_button_b[] PROGMEM = "!e32";
const char beep_button_c[] PROGMEM = "!g32";
const char go[] PROGMEM = "L16 cdegreg4";

// Data for generating the characters used in load_custom_characters
// and display_readings.  By reading levels[] starting at various
// offsets, we can generate all of the 7 extra characters needed for a
// bargraph.  This is also stored in program space.
const char levels[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

// This character is a back arrow.
const prog_char back_arrow[] PROGMEM = {
	0b00000,
	0b00010,
	0b00001,
	0b00101,
	0b01001,
	0b11110,
	0b01000,
	0b00100,
};

// 10 levels of bar graph characters
const char bar_graph_characters[10] = {' ',0,0,1,2,3,3,4,5,255};

// motor speeds
int m1_speed = 0;
int m2_speed = 0;

// Display battery voltage and wait for button press
void show_batt_wait_for_button_press();

// waits for a button, plays the appropriate beep, and returns the
// button or buttons that were pressed
char wait_for_button_and_beep();
