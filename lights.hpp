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

const float crossingLightRadius = 10;
const float trafficLightRadius = 25;

const float crossingWidth = 50;

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
    sf::CircleShape shape_;

public:
    explicit Traffic_light(const Traffic_color& traffic_color, const float position_x, const float position_y)
        : traffic_color_{ traffic_color } {
        shape_.setRadius(trafficLightRadius);
        shape_.setPosition(position_x, position_y);
        shape_.setOrigin(trafficLightRadius / 2, trafficLightRadius / 2);
    }

    explicit Traffic_light(){}

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
        return abs(x - shape_.getOrigin().x);
    }

    float distance_y(const float y) {
        return abs(y - shape_.getOrigin().y);
    }

    float getPosition_x() {
        return shape_.getOrigin().x;
    }

    float getPosition_y() {
        return shape_.getOrigin().y;
    }

    sf::CircleShape& getShape() {
        return shape_;
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
    sf::CircleShape shape_;
    int occupation_;

public:
    explicit Crossing(const Crossing_color& crossing_color, const float position_x, const float position_y) 
        : crossing_color_{ crossing_color }, occupation_{ 0 } {
        shape_.setRadius(crossingLightRadius);
        shape_.setPosition(position_x, position_y);
        shape_.setOrigin(crossingLightRadius / 2, crossingLightRadius / 2);
    }

    explicit Crossing(){}

    void operator++()
    {
        ++crossing_color_;
    }

    void set_crossing_color(const Crossing_color& crossing_color)
    {
        crossing_color_ = crossing_color;
        if(crossing_color == Crossing_color::green)
            shape_.setFillColor(sf::Color::Green);
        else
            shape_.setFillColor(sf::Color::Red);
    }

    void set_crossing_color(const sf::Color& color)
    {
        if (color == sf::Color::Red)
            crossing_color_ = Crossing_color::red;
        if (color == sf::Color::Green)
            crossing_color_ = Crossing_color::green;
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
        return abs(x - shape_.getOrigin().x);
    }

    float distance_y(const float y) {
        return abs(y - shape_.getOrigin().y);
    }

    float getPosition_x() {
        return shape_.getOrigin().x;
    }

    float getPosition_y() {
        return shape_.getOrigin().y;
    }

    sf::CircleShape& getShape() {
        return shape_;
    }
};

const sf::Color& get_SFML_color(const Traffic_light& traffic_light);
const sf::Color& get_SFML_color(const Crossing& crossing);
const sf::Color& get_SFML_color(const Traffic_color& color);
const sf::Color& get_SFML_color(const Crossing_color& color);
std::ostream& operator<<(std::ostream& os, const Traffic_light& traffic_light);
std::ostream& operator<<(std::ostream& os, const Crossing& crossing);
void print_traffic_light(std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, std::stop_token stop_token);
void run_traffic_light(std::map<std::string, Traffic_light>& traffic_lights, std::map<std::string, Crossing>& crossings, std::stop_token stop_token);