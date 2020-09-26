//By: Peter Tieu
//Link to GitHub: https://github.com/PeterTieu/On-This-Day
//Link to YouTube Demonstration: https://www.youtube.com/watch?v=XPp5gm2ga-Q

#include "splashkit.h"
#include "on_this_day.h"
#include "network.h"

int main()
{
    on_this_day_data on_this_day; //Declare On This Day variable

    load_resources();                       //Load all Bitmaps
    open_window("On This Day", 1000, 800);  //Open the window

    on_this_day = new_on_this_day(); //Create new On This Day
    load_interface(on_this_day);     //Show all Input Boxes in the window

    //For as long as the user has not requested to quit the window, e.g. by closing it
    while (!quit_requested())
    {
        process_events();           //Allow SplashKit to react to user interactions
        clear_screen(COLOR_WHITE);  //Clear to the screen

        handle_input_main_screen(on_this_day);  //Handle input fromt the user
        obtain_input_date(on_this_day);         //Obtain the input date from the user inputs
        update_on_this_day(on_this_day);        //Update On This Day
        draw_on_this_day(on_this_day);          //Draw On This Day
        analyse_input_date(on_this_day);        //Decide whether the user has inputed a valid date

        refresh_screen(60); //Refresh the window at 60fps
    }

    return 0;
}
