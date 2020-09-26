#include "splashkit.h"
#include "input_box.h"

using namespace std;

//Return the bitmap of the Input Box's kind
bitmap input_box_bitmap(input_box_kind kind)
{
    //Match passed kind parameter with the right Input Box bitmap
    switch (kind)
    {
        case INPUT_BOX_OFF: //Input Box OFF
            return bitmap_named("input_box_off");
            break;

        default: //Input Box ON
            return bitmap_named("input_box_on");
            break;
    }
}

//Create a new Input Box depending on the passed kind and x-position
input_box_data new_input_box(input_box_kind kind, double x_pos)
{
    input_box_data input_box; //Declare Input Box
    bitmap input_box_bmp;     //Declare bitmap for the Input Box

    input_box_bmp = input_box_bitmap(kind); //Obtain the bitmap of the Input Box based on its kind
    input_box.input_box_sprite = create_sprite(input_box_bmp); //Create the Input Box sprite

    sprite_add_layer(input_box.input_box_sprite, input_box_bitmap(INPUT_BOX_ON), "input_box_on"); //Add ON layer to Day #1
    sprite_hide_layer(input_box.input_box_sprite, 1); //Hide the ON layer

    sprite_set_x(input_box.input_box_sprite, x_pos); //Set the x-coordinate of the Input Box sprite
    sprite_set_y(input_box.input_box_sprite, screen_height()*0.375); //Set the y-coordinate of the Input Box sprite

    return input_box;
}

//Update the Input Box
void update_input_box(input_box_data &input_box)
{
    update_sprite(input_box.input_box_sprite);
}

//Draw the Input Box
void draw_input_box(input_box_data &input_box)
{
    draw_sprite(input_box.input_box_sprite);
}
