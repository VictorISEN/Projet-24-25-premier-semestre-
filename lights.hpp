#pragma once
// condition_variable::wait(with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

using namespace std::chrono_literals;

static const sf::Color Orange(255, 165, 0);

const auto time_transit = 3s;
const auto time_waiting = 6s;
const auto delay_crossing = 1s;

enum class Traffic_color
{
    green = 0,
    orange = 1,
    red = 2
};

Traffic_color operator++(Traffic_color& traffic_color);

class Traffic_light
{
private:
    Traffic_color traffic_color_;
    float position_x_;
    float position_y_;

public:
    explicit Traffic_light(const Traffic_color& traffic_color, const float position_x, const float position_y) : traffic_color_{ traffic_color }, position_x_{ position_x }, position_y_{ position_y } {}
    explicit Traffic_light() : traffic_color_{ Traffic_color::red }, position_x_{ 0.0 }, position_y_{ 0.0 } {}
    void operator++()
    {
        ++traffic_color_;
    }
    void set_traffic_color(const Traffic_color& traffic_color)
    {
        traffic_color_ = traffic_color;
    }
    const Traffic_color& get_traffic_color() const
    {
        return traffic_color_;
    }
    float distance_x(const float x) {
        return abs(x - position_x_);
    }
    float distance_y(const float y) {
        return abs(y - position_y_);
    }
};

enum class Crossing_color
{
    green = 0,
    red = 1
};

Crossing_color operator++(Crossing_color& crossing_color);

class Crossing
{
private:
    Crossing_color crossing_color_;
    float position_x_;
    float position_y_;
    int occupation_;

public:
    explicit Crossing(const Crossing_color& crossing_color, const float position_x, const float position_y) : crossing_color_{ crossing_color }, position_x_{ position_x }, position_y_{ position_y }, occupation_{ 0 } {}
    explicit Crossing() : crossing_color_{ Crossing_color::red }, position_x_{ 0.0 }, position_y_{ 0.0 }, occupation_{ 0 } {}
    void operator++()
    {
        ++crossing_color_;
    }
    void set_crossing_color(const Crossing_color& crossing_color)
    {
        crossing_color_ = crossing_color;
    }
    const Crossing_color& get_crossing_color() const
    {
        return crossing_color_;
    }
    bool iseCrossingEmpty() {
        if (occupation_ == 0)
            return true;
        return false;
    }
    float distance_x(const float x) {
        return abs(x - position_x_);
    }
    float distance_y(const float y) {
        return abs(y - position_y_);
    }
};

const sf::Color& get_SFML_color(const Traffic_light& traffic_light);
const sf::Color& get_SFML_color(const Crossing& crossing);
std::ostream& operator<<(std::ostream& os, const Traffic_light& traffic_light);
std::ostream& operator<<(std::ostream& os, const Crossing& crossing);
void print_traffic_light(std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, std::stop_token stop_token);
void run_traffic_light(std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, std::stop_token stop_token);