#include "splashkit.h"
#include "network.h"
#include <string>

using namespace std;

//Store data obtained from the vector of JSON objects (from the "events" key) into Network
void parse_data_to_network(network_data &network, vector<json> json_events)
{
    //Iterate over the JSON Events vector
    for (int i = 0; i < json_events.size(); i++)
    {
        network.years.push_back(json_read_string(json_events[i], "year")); //Obtain the string value of the JSON object with "year" key, and add it as an element to the Years vector of Network
        network.descriptions.push_back(json_read_string(json_events[i], "description")); //Obtain the string value of the JSON object with "description" key, and add it as an element to the Descriptions vector of Network

    }
}

//Print all data in Network to the terminal from latest event to earliest event (useful for Debugging)
void print_network_to_terminal(network_data &network, vector<json> json_events)
{
    //Iterate through the JSON Events vector
    for (int i=json_events.size()-1; i > 0; i--)
    {
        write_line("YEAR: " + network.years[i]);          //Print out the Year string of the current index
        write_line("EVENT: " + network.descriptions[i]);  //Print out the Description string of the current index
        write_line();
    }
}

//Perform networking and store all the obtained data into Network
network_data perform_networking(int day, int month)
{
    network_data network;        //Declare Network variable for storing all retrieved data
    http_response http_response; //Declare http_response variable for storing the data returned from a HTTP request
    string output_string;        //Declare string for storing string output of HTTP request as a string
    json json_obtained;          //Declare JSON object for storing JSON output of the HTTP request
    vector<json> json_events;    //Declare vector for storing all the values from the "events" key


    //API Information: https://byabbe.se/on-this-day/#/default/get__month___day__events_json
    //Example URL: https://byabbe.se/on-this-day/6/30/events.json
    http_response = http_get("https://byabbe.se/on-this-day/" + to_string(month) + "/" + to_string(day) + "/events.json", 443); //Make a get request to access a resource on the internet
    output_string = http_response_to_string(http_response); //Obtain the output of the HTTP response, convert it to a string, and store it into string

    json_obtained = create_json(output_string); //Obtain JSON object from the output

    //Obtain the JSON Array "events" from the JSON Object
    json_read_array(json_obtained, "events", json_events); //Take "events" key from JSON object "json_obtained" and puts it into a JSON array "json_events"


    parse_data_to_network(network, json_events); //Store data obtained from the vector of JSON objects (from the "events" key) into Network
    // print_network_to_terminal(network, json_events);

    free_json(json_obtained); //Free all resources afer completion of networking

    return network; //Return Network
}