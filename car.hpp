#pragma once
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "pedestrian.hpp"
#include "lights.hpp"
#include "bus.hpp"
#include "bikes.hpp"

const float safe_distance_car = 210.0f; // Distance minimale entre deux voitures
const float stop_distance_car = 120.0f; // Distance pour commencer à s'arrêter au feu rouge
const float max_speed_car = 20;
const float width_car = 30;
const float height_car = 60;
const float acceleration_car = 2.0f;
const auto delay_speed = 0.04s;

class Car {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    entry destination_;
    float speed_; // Vitesse actuelle
    float acceleration_; // Accélération
    bool in_crossing_;
    bool can_turn_;

public:
    Car(float x, float y, sf::Vector2f direction, entry destination)
        : shape_(sf::Vector2f(width_car, height_car)), direction_(direction), speed_(max_speed_car), destination_(destination), acceleration_(acceleration_car), in_crossing_(false), can_turn_(false) { // Réduire l'accélération
        shape_.setPosition(x, y);
        if(destination_ == entry::top)
            shape_.setFillColor(sf::Color::Yellow);
        if (destination_ == entry::bottom)
            shape_.setFillColor(sf::Color::Magenta);
        if (destination_ == entry::left)
            shape_.setFillColor(sf::Color::Red);
        if (destination_ == entry::right)
            shape_.setFillColor(sf::Color::Blue);
        shape_.setOrigin(width_car / 2, height_car / 2);
    }

    void move() {
        shape_.move(direction_ * speed_);
    }

    void stop() {
        if (speed_ > 0) {
            speed_ -= acceleration_; // Décélération progressive
            if (speed_ < 0) speed_ = 0;
        }
    }

    void accelerate() {
        if (speed_ < max_speed_car) {
            speed_ += acceleration_; // Accélération progressive
            if (speed_ > max_speed_car) speed_ = max_speed_car;
        }
    }

    sf::RectangleShape& getShape() { return shape_; }

    float getX() const { return shape_.getPosition().x; }
    float getY() const { return shape_.getPosition().y; }

    sf::Vector2f getDirection() const { return direction_; }

    bool isInCrossing() const { return in_crossing_; }
    void setInCrossing(bool value) { in_crossing_ = value; }

    float getDistanceTo(const Car& other) const {
        return std::sqrt(std::pow(other.getX() - getX(), 2) + std::pow(other.getY() - getY(), 2));
    }

    entry getDestination() {
        return destination_;
    }

    bool getCanTurn() {
        return can_turn_;
    }

    void setCanTurn(bool canturn) {
        can_turn_ = canturn;
    }

    void turn();
    bool canTurn(bool CTop, bool CBottom, bool CLeft, bool CRight, std::vector<Bus>& buses, std::vector<Bike>& bikes, std::vector<Car>& cars);

};

void run_cars(std::vector<Car>& cars, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::vector<Bus>& buses, std::vector<Bike>& bikes, std::vector<pedestrian>& pedestrians, std::stop_token stop_token);

void generate_cars(std::vector<Car>& cars);