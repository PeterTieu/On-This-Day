#include "splashkit.h"
#include "on_this_day.h"
#include "input_box.h"
#include "network.h"

using namespace std;

//Load all bitmaps in the bundle
void load_resources()
{
    load_resource_bundle("on_this_day_bundle", "on_this_day.txt");
}

//Create new On This Day variable
on_this_day_data new_on_this_day()
{
    on_this_day_data on_this_day; //Declare On This Day variable

    //Initialise all the fields of On This Day
    on_this_day.box_pointer = 0;
    on_this_day.is_valid_input_date = false;
    on_this_day.is_perform_networking_requested = false;
    on_this_day.page_number = 1;

    return on_this_day;
}


//============================================ MAIN SCREEN ================================================================================

//Switch the Input Box to display the ON layer
void input_box_switch_to_on(on_this_day_data &on_this_day, int box_index)
{
    sprite_show_layer(on_this_day.input_boxes[box_index].input_box_sprite, static_cast<int>(1)); //Hide the OFF layer
    sprite_hide_layer(on_this_day.input_boxes[box_index].input_box_sprite, static_cast<int>(0)); //Show the ON layer
}

//Switch the Input Box to display the OFF layer
void input_box_switch_to_off(on_this_day_data &on_this_day, int box_index)
{
    sprite_show_layer(on_this_day.input_boxes[box_index].input_box_sprite, static_cast<int>(0)); //Show the OFF layer
    sprite_hide_layer(on_this_day.input_boxes[box_index].input_box_sprite, static_cast<int>(1)); //Hide the ON layer
}

//Toggle the Input Box at the current Box Pointer to the "ON" state and all the other ones to the "OFF" state
void toggle_to_next_input_box(on_this_day_data &on_this_day)
{
    //Iterate over all the Input Boxes
    for (int i = 0; i < on_this_day.input_boxes.size(); i++)
    {
        //If the Box Pointer equals the iterated Input Box
        if (on_this_day.box_pointer == i)
        {
            input_box_switch_to_on(on_this_day, i); //Toggle this Input Box to the "ON" state
        }
        else
        {
            input_box_switch_to_off(on_this_day, i); //Toggle all other Input Boxes to the "OFF" state
        }
    }

    //If the Box Pointer exceeds the number of Input Boxes less one
    if (on_this_day.box_pointer == NUM_INPUT_BOXES - 1)
    {
        on_this_day.box_pointer = -1; //Re-assign the Box Pointer to -1
    }
}

//Show all Input Boxes in the window
void load_interface(on_this_day_data &on_this_day)
{
    input_box_data input_box_day_1, input_box_day_2, input_box_month_1, input_box_month_2; //Declare all Input Boxes
    int x = 180;                                                                           //Declare and initialise the x-coordinate to begin at 180

    //Create all Input Boxes
    input_box_day_1 = new_input_box(INPUT_BOX_OFF, x);          //Input Box for Day #1
    input_box_day_2 = new_input_box(INPUT_BOX_OFF, x += 150);   //Input Box for Day #2
    input_box_month_1 = new_input_box(INPUT_BOX_OFF, x += 250); //Input Box for Month #1
    input_box_month_2 = new_input_box(INPUT_BOX_OFF, x += 150); //Ihput Box for Month #2

    //Add the Input Box to the Input Boxes vector of On This Day
    on_this_day.input_boxes.push_back(input_box_day_1);   //Input Box for Day #1
    on_this_day.input_boxes.push_back(input_box_day_2);   //Input Box for Day #2
    on_this_day.input_boxes.push_back(input_box_month_1); //Input Box for Month #1
    on_this_day.input_boxes.push_back(input_box_month_2); //Ihput Box for Month #2

    toggle_to_next_input_box(on_this_day); //Toggle the Input Box at the index indicated by the Box Pointer to "ON" state.
                                           //This is be the first Input Box since Box Pointer is initialised to 0, i.e. Day #1
}

//Handle user input for the Main Screen
void handle_input_main_screen(on_this_day_data &on_this_day)
{
    //If the key typed is 'C' or 'Space'
    if (key_typed(C_KEY) || key_typed(SPACE_KEY))
    {
        on_this_day.date_inputs.clear();       //Clear all elements in the Date Inputs vector
        on_this_day.box_pointer = 0;           //Reset the Box Pointer to 0
        toggle_to_next_input_box(on_this_day); //Toggle the Input Box at the index indicated by the Box Pointer to "ON" state.
                                               //This is be the first Input Box since Box Pointer is reset to 0, i.e. Day #1
    }

    //If the size of the Date Inputs vector is less than or equal to 3, i.e. not all Input Boxes have been filled yet, i.e. not all data for DD/MM have been obtained yet
    if (on_this_day.date_inputs.size() <= 3)
    {
        //Handle input for keys 0-9
        //Numbers 48 to 57 correspond to the key_code enumerated values for the keys 0-9
        //E.g. 48 is the value for NUM_0_KEY
        //49 is the value for NUM_1_KEY
        //57 is the value for NUM_9_KEY, etc.
        for (int i = 48; i < 58; i++)
        {
            //Iterate through all the key_code values
            if (key_typed(static_cast<key_code>(i)))
            {
                toggle_to_next_input_box(on_this_day);             //Toggle the Input Box at the index indicated by the Box Pointer to "ON" state.
                int entered_number = i - 48;                       //Obtain the number entered by the user
                                                                   //If i=48, the number entered is 0
                                                                   //If i=49, then the number entered is 1
                                                                   //If i=57, then the number entered is 9
                on_this_day.date_inputs.push_back(entered_number); //Add the entered number to the Date Inputs vector
                on_this_day.box_pointer++;                         //Increment the Box Pointer
            }
        }
    }
}

//Concatenate two integer values into one
int concatenate(int a, int b)
{
    string s1 = to_string(a); //Convert first integer value to string
    string s2 = to_string(b); //Convert second integetr value to string

    string s = s1 + s2; //Concatenate both strings

    int c = stoi(s); // Convert the concatenated string to an integer

    return c; //Return the concatenated integer
}

//Obtain the Input Day and Input Month from the Date Input vector
void obtain_input_date(on_this_day_data &on_this_day)
{
    //If the size of the Date Input vector is equal to 4, i.e. the DD/MM values have been entered by the user
    if (on_this_day.date_inputs.size() == 4)
    {
        on_this_day.input_day = concatenate(on_this_day.date_inputs[0], on_this_day.date_inputs[1]);   //Concatenate the first two values of the Date Input vector
                                                                                                       //and store the returned value into the Day Input
        on_this_day.input_month = concatenate(on_this_day.date_inputs[2], on_this_day.date_inputs[3]); //Concatenate the second two values of the Date Input vector
                                                                                                       //and store the returned value into the Month Input
    }
}

//Request valid date from the user
void request_valid_input_date(on_this_day_data &on_this_day)
{
    on_this_day.is_valid_input_date = false; //Set the flag of the Valid Input Date to false

    draw_text("Invalid Date", COLOR_RED, load_font("ZebulonBold", "ZebulonBold"), 30, screen_width() / 2 - 150, screen_height() / 2 + 50);                      //Display notify that the date is invalid
    draw_text("Please enter valid date", COLOR_YELLOW_GREEN, load_font("ZebulonBold", "ZebulonBold"), 25, screen_width() / 2 - 270, screen_height() / 2 + 100); //Display request for valid date
}

//Analyse the Input Day and Input Month and decide whether they denote valid dates
void analyse_input_date(on_this_day_data &on_this_day)
{
    //If the size of the Date Input vector is equal to 4, i.e. the DD/MM values have been entered by the user
    if (on_this_day.date_inputs.size() == 4)
    {
        //If the Input Month or Input Date is equal to 0
        if (on_this_day.input_month == 00 || on_this_day.input_day == 00)
        {
            request_valid_input_date(on_this_day); //Request valid date from the user
        }
        //If the Input Month is greater than 12
        else if (on_this_day.input_month > 12)
        {
            request_valid_input_date(on_this_day); //Request valid date from the user
        }
        //If the Input Month is 2 but the Input Date is greater than 29
        else if (on_this_day.input_month == 2 && on_this_day.input_day > 29)
        {
            request_valid_input_date(on_this_day);
        }
        //If the Input Month is on a month that has 30 days but the Input Day is greater than 30
        else if ((on_this_day.input_month == 4 || on_this_day.input_month == 6 || on_this_day.input_month == 9 || on_this_day.input_month == 11) && on_this_day.input_day > 30)
        {
            request_valid_input_date(on_this_day); //Request valid date from the user
        }
        //IF the Input Month is on a month that has 31 days but the Input Day is greater than 31
        else if ((on_this_day.input_month == 1 || on_this_day.input_month == 3 || on_this_day.input_month == 5 || on_this_day.input_month == 7 || on_this_day.input_month == 8 || on_this_day.input_month == 10 || on_this_day.input_month == 12) && on_this_day.input_day > 31)
        {
            request_valid_input_date(on_this_day); //Request valid date from the user
        }
        //If none of the above conditions are met, i.e. the input date must be valid by this point
        else
        {
            on_this_day.is_valid_input_date = true;
        }
    }
}


//============================================ RESULTS SCREEN ================================================================================

//Perform networking to obtain all the data, then store them into the Network field of On This Day
void fetch_networking_data(on_this_day_data &on_this_day)
{
    //If the flag for the Valid Input Date is true and the flag for the Perform Networking Requested is false
    if (on_this_day.is_valid_input_date == true && on_this_day.is_perform_networking_requested == false)
    {
        on_this_day.network = perform_networking(on_this_day.input_day, on_this_day.input_month); //Perform networking and store all the data into the Network field of On This Day
        on_this_day.is_perform_networking_requested = true;                                       //Toggle the flag to Perform Networking Request to true, as networking has now been requested
        on_this_day.page_number = 1;                                                              //Initialise the results page number to 1
    }
}

//Draw the results page
void draw_results_for_page(on_this_day_data &on_this_day, int recent_event_placing, int older_event_placing)
{
    int y_pos = 35; //Declare and initialise the y-coordinate of the first result to 35

    draw_bitmap("background_on_this_day_2", 0, 0); //Draw the background for the results page

    draw_text("THIS DAY IN HISTORY: ", COLOR_YELLOW, load_font("open_sans_bold", "open_sans_bold"), 20, 180, 5);                                                           //Draw the heading
    draw_text(to_string(on_this_day.input_day) + " / " + to_string(on_this_day.input_month), COLOR_LIGHT_BLUE, load_font("open_sans_bold", "open_sans_bold"), 32, 410, 0); //Draw the date

    draw_bitmap("left_arrow_key", screen_width() * 0.55, 0); //Draw the 'left arrow' key picture
    draw_bitmap("right_arrow_key", screen_width() * 0.6, 0); //Draw the 'right arrow' key picture
    draw_bitmap("c_key", screen_width() * 0.93, 0);          //Draw the 'c' key picture

    draw_text("YEAR", COLOR_ORANGE, load_font("open_sans_bold", "open_sans_bold"), 15, 10, 40);  //Draw the Year column heading
    draw_text("EVENT", COLOR_ORANGE, load_font("open_sans_bold", "open_sans_bold"), 15, 63, 40); //Draw the Event column heading

    //Iterate through the elements of the Network vector
    for (int i = on_this_day.network.years.size() - recent_event_placing; i > on_this_day.network.years.size() - older_event_placing; i--)
    {
        y_pos += 28; //Increment the y-coordinate by 28. This is vertical space between results

        draw_text(on_this_day.network.years[i], COLOR_YELLOW, load_font("open_sans_bold", "open_sans_bold"), 12, 20, y_pos); //Draw Year data

        //If the Description data of Network contains more than 160 characters (i.e. it exceeds the width of the screen)
        if (on_this_day.network.descriptions[i].size() > 160)
        {
            draw_text(on_this_day.network.descriptions[i].substr(0, 160),
                      COLOR_WHITE, load_font("open_sans_regular", "open_sans_regular"), 12, 60, y_pos); //Draw the first line of the Description data
            draw_text(on_this_day.network.descriptions[i].substr(160, on_this_day.network.descriptions[i].size()),
                      COLOR_WHITE, load_font("open_sans_regular", "open_sans_regular"), 12, 60, y_pos += 16); //Draw the second line of the Description data, below the first line
        }
        //If the Description data of Network contains less than 160 characters (i.e. it fits within the width of the screen)
        else
        {
            draw_text(on_this_day.network.descriptions[i], COLOR_WHITE, load_font("open_sans_regular", "open_sans_regular"), 12, 60, y_pos); //Draw the Description data
        }
    }
}

//Handle user input for the Results Screen
void handle_input_results_screen(on_this_day_data &on_this_day)
{
    //If the key typed is the 'right key' or 'down key'
    if (key_typed(RIGHT_KEY) || key_typed(DOWN_KEY))
    {
        on_this_day.page_number++; //Increment the Page Number
    }

    //If the key typed is the 'left key' or 'up key'
    if (key_typed(LEFT_KEY) || key_typed(UP_KEY))
    {
        on_this_day.page_number--; //Decrement the Page Number
    }

    //If the key typed is the 'c key' or 'space key' - signal to go back to the Main Screen
    if (key_typed(C_KEY) || key_typed(SPACE_KEY))
        {
            on_this_day.date_inputs.clear(); //Clear all elements of the Date Inputs vector
            on_this_day.is_valid_input_date = false; //Set the flag of the Valid Input Date to false
            on_this_day.is_perform_networking_requested = false; //Set the flag of the Perform Networking Requested to false
            on_this_day.box_pointer = 0; //Re-assign the Box Pointer to 0

            toggle_to_next_input_box(on_this_day); //Toggle the Input Box at the index indicated by the Box Pointer to "ON" state
        }
}

//Set the page limit of the Results Screen
void set_page_limit(on_this_day_data &on_this_day, int limit)
{
    //If the Page Number equals 0
    if (on_this_day.page_number == 0)
    {
        on_this_day.page_number = 1; //Re-assign the Page Number to 1
    }

    //If the Page Number equals 1 over the limit
    if (on_this_day.page_number == limit + 1)
    {
        on_this_day.page_number = limit; //Re-assign the Page Number to the limit
    }
}

//Draw the Results Screen
void draw_results_screen(on_this_day_data &on_this_day)
{
    //If there are less than 25 results (inclusive)
    if (on_this_day.network.years.size() <= 25)
    {
        //If the page number is 1
        if (on_this_day.page_number == 1)
        {
            draw_results_for_page(on_this_day, 1, on_this_day.network.years.size()); //Draw all the results
        }

        handle_input_results_screen(on_this_day); //Handle the user inputs
        set_page_limit(on_this_day, 1); //Set the page limit to 1
    }

    //If there are between 25 (non-inclusive) and 50 (inclusive) results
    if (on_this_day.network.years.size() > 25 && on_this_day.network.years.size() <= 50)
    {
        //Display results according the Page Number
        switch (on_this_day.page_number)
        {
        case 1: //Page 1
            draw_results_for_page(on_this_day, 1, 25); //Draw the first 25 results
            break;
        case 2: //Page 2
            draw_results_for_page(on_this_day, 26, on_this_day.network.years.size()); //Draw the remaining results
            break;
        }

        handle_input_results_screen(on_this_day); //Handle the user inputs
        set_page_limit(on_this_day, 2); //Set the page limit to 2
    }

    //If there are between 50 (non-inclusive) and 75 (inclusive) results
    if (on_this_day.network.years.size() > 50 && on_this_day.network.years.size() <= 75)
    {
        //Display results according the Page Number
        switch (on_this_day.page_number)
        {
        case 1: //Page 1
            draw_results_for_page(on_this_day, 1, 25); //Draw the first 25 results
            break;
        case 2: //Page 2
            draw_results_for_page(on_this_day, 26, 50); //Draw the next 25 results
            break;
        case 3: //Page 3
            draw_results_for_page(on_this_day, 51, on_this_day.network.years.size()); //Draw the remaining results
            break;
        }

        handle_input_results_screen(on_this_day); //Handle the user inputs
        set_page_limit(on_this_day, 3); //Set the page limit to 3
    }

    //If there are between 75 (non-inclusive) and 100 (inclusive) results
    if (on_this_day.network.years.size() > 75 && on_this_day.network.years.size() <= 100)
    {
        //Display results according the Page Number
        switch (on_this_day.page_number)
        {
        case 1: //Page 1
            draw_results_for_page(on_this_day, 1, 25); //Draw the first 25 results
            break;
        case 2: //Page 2
            draw_results_for_page(on_this_day, 26, 50); //Draw the next 25 results
            break;
        case 3: //Page 3
            draw_results_for_page(on_this_day, 51, 75); //Draw the next 25 results
            break;
        case 4: //Page 4
            draw_results_for_page(on_this_day, 76, on_this_day.network.years.size()); //Draw the remaining results
            break;
        }

        handle_input_results_screen(on_this_day); //Handle the user inputs
        set_page_limit(on_this_day, 4); //Set the page limit to 4
    }

    //If there are greater than 100 (non-inclusive) results
    if (on_this_day.network.years.size() > 100)
    {
        //Display results according the Page Number
        switch (on_this_day.page_number)
        {
        case 1: //Page 1
            draw_results_for_page(on_this_day, 1, 25); //Draw the first 25 results
            break;
        case 2: //Page 2
            draw_results_for_page(on_this_day, 26, 50); //Draw the next 25 results
            break;
        case 3:
            draw_results_for_page(on_this_day, 51, 75); //Draw the next 25 results
            break;
        case 4:
            draw_results_for_page(on_this_day, 76, 100); //Draw the next 25 results
            break;
        case 5:
            draw_results_for_page(on_this_day, 101, on_this_day.network.years.size()); //Draw the remaining results
            break;
        }

        handle_input_results_screen(on_this_day); //Handle the user inputs
        set_page_limit(on_this_day, 5); //Set the page limit to 5
    }
}


//============================================ UPDATE AND DRAW MAIN SCREEN ================================================================================

//Update On This Day
void update_on_this_day(on_this_day_data &on_this_day)
{
    //Iterate over all the elements of the Input Box
    for (int i = 0; i < on_this_day.input_boxes.size(); i++)
    {
        update_input_box(on_this_day.input_boxes[i]); //Date the Input Box
    }
}

//Draw the Main Screen
void draw_on_this_day(on_this_day_data &on_this_day)
{
    draw_bitmap("background_on_this_day_1", 0, 0); //Draw the background

    //If the flag for the Valid Input Date is false, i.e. either the user has not finished entering all the required data or the Date Input is not valid
    if (on_this_day.is_valid_input_date == false)
    {
        draw_text("ON THIS DAY", COLOR_GOLD, load_font("ZebulonBold", "ZebulonBold"), 50, screen_width() / 2 - 220, screen_height() * 0.05); //Draw the heading

        draw_text("Enter a date to reveal significant events on that day in history", COLOR_BLUE_VIOLET, load_font("ZebulonBold", "ZebulonBold"), 15,
                    screen_width() / 2 - 410, screen_height() * 0.22); //Draw the instructions

        //Draw the input guides, i.e. "M", "M", "/", "D", "D"
        draw_text("D", COLOR_GOLD, load_font("ZebulonBold", "ZebulonBold"), 30,
                    sprite_x(on_this_day.input_boxes[0].input_box_sprite) + 40, sprite_y(on_this_day.input_boxes[0].input_box_sprite) - 40); //Draw "D" above the first Input Box
        draw_text("D", COLOR_GOLD, load_font("ZebulonBold", "ZebulonBold"), 30,
                    sprite_x(on_this_day.input_boxes[1].input_box_sprite) + 40, sprite_y(on_this_day.input_boxes[1].input_box_sprite) - 40); //Draw "D" above the first Input Box
        draw_text("/", COLOR_GOLD, load_font("ZebulonBold", "ZebulonBold"), 100, screen_width() / 2 - 40, 290); //Draw "/" between the "DD" and "MM" Input Boxes
        draw_text("M", COLOR_GOLD, load_font("ZebulonBold", "ZebulonBold"), 30,
                    sprite_x(on_this_day.input_boxes[2].input_box_sprite) + 35, sprite_y(on_this_day.input_boxes[2].input_box_sprite) - 40); //Draw "M" above the first Input Box
        draw_text("M", COLOR_GOLD, load_font("ZebulonBold", "ZebulonBold"), 30,
                    sprite_x(on_this_day.input_boxes[3].input_box_sprite) + 35, sprite_y(on_this_day.input_boxes[3].input_box_sprite) - 40); //Draw "M" above the first Input Box

        //Draw instructions to "clear" inputs
        draw_text("CLEAR :", COLOR_ORANGE, load_font("ZebulonBold", "ZebulonBold"), 25, screen_width() * 0.21, screen_height() * 0.87); //Draw "CLEAR" word
        draw_bitmap("c_key", screen_width() * 0.37, screen_height() * 0.85); //Draw 'C' key picture
        draw_text("or", COLOR_YELLOW, load_font("ZebulonBold", "ZebulonBold"), 16, screen_width() * 0.45, screen_height() * 0.87); //Draw "OR" word
        draw_bitmap("space_key", screen_width() * 0.52, screen_height() * 0.86); //Draw 'Spae' key picture

        //Iterate over all the Input Boxes
        for (int i = 0; i < on_this_day.input_boxes.size(); i++)
        {
            draw_input_box(on_this_day.input_boxes[i]); //Draw the Input Box
        }

        //Iterate over all the elements of the Date Inputs vector
        for (int i = 0; i < on_this_day.date_inputs.size(); i++)
        {
            int date_input = on_this_day.date_inputs[i]; //Get the input data
            double x_pos_of_box = sprite_x(on_this_day.input_boxes[i].input_box_sprite); //Get the x-coordinate of the corresponding Input Box
            double y_pos_of_box = sprite_y(on_this_day.input_boxes[i].input_box_sprite); //Get the y-coordinate of the corersponding Input Box

            draw_text(to_string(date_input), COLOR_BLACK, load_font("ZebulonBold", "ZebulonBold"), 55,
                                x_pos_of_box + 30, y_pos_of_box + 20); //Draw the input data on top of the corresponding Input Box
        }
    }
    //If the flag for the Valid Input Date is true, i.e. the user has finished entering all the required data and the Date Input is valid
    else
    {
        fetch_networking_data(on_this_day); //Perform networking to obtain all the data, then store them into the Network field of On This Day
        draw_results_screen(on_this_day); //Draw the Results Screen
    }
}
