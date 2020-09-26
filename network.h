#ifndef ON_THIS_DAY_INPUT_NETWORK_DATA
#define ON_THIS_DAY_INPUT_NETWORK_DATA

#include "splashkit.h"
#include <vector>

//Define Network struct
struct network_data
{
    vector<string> years; //Vector for storing all the values from the "year" key (from the "events" JSON array)
    vector<string> descriptions; //Vector for storing all the values from the "description" key (from the "events" JSON array)
};

network_data perform_networking(int day, int month); //Perform networking and store all the obtained data into Network

#endif