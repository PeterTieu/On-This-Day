#ifndef ON_THIS_DAY_DATA
#define ON_THIS_DAY_DATA

#include "splashkit.h"
#include "input_box.h"
#include "network.h"
#include <vector>

#define NUM_INPUTS 4 //Number of inputs on the screen, i.e. D, D, M, M

//On This Day struct
struct on_this_day_data
{
    int box_pointer;                        //Pointer for the date input - used for toggling Input Boxes on and off
    vector<input_box_data> input_boxes;     //Input Boxes toggle on and off
    int input_day;                          //Day inputed by the user, in the form of DD
    int input_month;                        //Month inputed by the user, in the form of MM
    vector<int> date_inputs;                //Date inputed by the user, in the form of DDMM
    bool is_valid_input_date;               //Flag to indicate the that input is valid
    bool is_perform_networking_requested;   //Flag to indacte that networking for JSON has been requested
    network_data network;                   //Network
    int page_number;                        //Page number in the results screen
};

//Declare procedures and functions
void load_resources();                                          //Load all bitmaps in the bundle
on_this_day_data new_on_this_day();                             //Create new On This Day variable
void load_interface(on_this_day_data &on_this_day);             //Show all Input Boxes in the window
void handle_input_main_screen(on_this_day_data &on_this_day);   //Handle user input for the Main Screen
void obtain_input_date(on_this_day_data &on_this_day);          //Obtain the Input Day and Input Month from the Date Input vector
void analyse_input_date(on_this_day_data &on_this_day);         //Analyse the Input Day and Input Month and decide whether they denote valid dates
void update_on_this_day(on_this_day_data &on_this_day);         //Update On This Day
void draw_on_this_day(on_this_day_data &on_this_day);           //Draw the Main Screen

#endif