// condition_variable::wait(with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "lights.hpp"

using namespace std::chrono_literals;

const sf::Color& get_SFML_color(const Traffic_light& traffic_light)
{
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::green:
        return sf::Color::Green;
    case Traffic_color::red:
        return sf::Color::Red;
    }
    return Orange;
}

const sf::Color& get_SFML_color(const Crossing& crossing)
{
    switch (crossing.get_crossing_color())
    {
    case Crossing_color::green:
        return sf::Color::Green;
    }

    return sf::Color::Red;
}

void print_traffic_light(std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, std::stop_token stop_token)
{
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(1s);
        std::cout << "feux rue solferino " << traffic_lights["solferino"] << " feux bd vauban : " << traffic_lights["vauban"] <<
            " pietons haut : " << crossings["top"] << " pietons bas : " << crossings["bottom"] << " pietons gauche : " << crossings["left"] << " pietons droite : " << crossings["right"] << std::endl;
    }
}

void run_traffic_light(std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, std::stop_token stop_token)
{
    traffic_lights["solferino"].set_traffic_color(Traffic_color::green);
    traffic_lights["vauban"].set_traffic_color(Traffic_color::red);
    std::this_thread::sleep_for(delay_crossing);
    crossings["top"].set_crossing_color(Crossing_color::green);
    crossings["bottom"].set_crossing_color(Crossing_color::green);
    crossings["left"].set_crossing_color(Crossing_color::red);
    crossings["right"].set_crossing_color(Crossing_color::red);
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(time_waiting);
        crossings["top"].set_crossing_color(Crossing_color::red);
        crossings["bottom"].set_crossing_color(Crossing_color::red);
        crossings["left"].set_crossing_color(Crossing_color::red);
        crossings["right"].set_crossing_color(Crossing_color::red);
        std::this_thread::sleep_for(delay_crossing);

        if (traffic_lights["solferino"].get_traffic_color() == Traffic_color::green)
        {
            ++traffic_lights["solferino"];
            std::this_thread::sleep_for(delay_crossing);
            ++traffic_lights["vauban"];
            std::this_thread::sleep_for(delay_crossing);
            ++crossings["left"];
            ++crossings["right"];
        }
        else
        {
            ++traffic_lights["vauban"];
            std::this_thread::sleep_for(delay_crossing);
            ++traffic_lights["solferino"];
            std::this_thread::sleep_for(delay_crossing);
            ++crossings["top"];
            ++crossings["bottom"];
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Crossing& crossing)
{
    std::string ptr;
    switch (crossing.get_crossing_color())
    {
    case Crossing_color::red:
        ptr = "Red";
        break;
    case Crossing_color::green:
        ptr = "Green";
        break;
    }
    os << ptr;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Traffic_light& traffic_light)
{
    std::string ptr;
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::red:
        ptr = "Red";
        break;
    case Traffic_color::green:
        ptr = "Green";
        break;
    case Traffic_color::orange:
        ptr = "Orange";
    }
    os << ptr;
    return os;
}


Crossing_color operator++(Crossing_color& crossing_color)
{
    switch (crossing_color)
    {
    case Crossing_color::red:
        crossing_color = Crossing_color::green;
        break;

    case Crossing_color::green:
        crossing_color = Crossing_color::red;
    }
    return crossing_color;
}

Traffic_color operator++(Traffic_color& traffic_color)
{
    switch (traffic_color)
    {
    case Traffic_color::red:
        traffic_color = Traffic_color::green;
        break;

    case Traffic_color::green:
        traffic_color = Traffic_color::orange;
        std::this_thread::sleep_for(time_transit);
        traffic_color = Traffic_color::red;
    }
    return traffic_color;
}
