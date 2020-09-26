#ifndef ON_THIS_DAY_INPUT_BOX_DATA
#define ON_THIS_DAY_INPUT_BOX_DATA

#include "splashkit.h"

#define NUM_INPUT_BOXES 4

//Define kinds of the Input Box
enum input_box_kind
{
   INPUT_BOX_OFF, //OFF kind
   INPUT_BOX_ON,  //ON kind
};

//Input Box struct
struct input_box_data
{
    sprite input_box_sprite; //Sprite of Input Box
};

//Declare procedures and fucntions
input_box_data new_input_box(input_box_kind kind, double x_pos); //Create a new Input Box depending on the passed kind and x-position
void update_input_box(input_box_data &input_box);                //Update the Input Box
void draw_input_box(input_box_data &input_box);                  //Draw the Input Box

#endif