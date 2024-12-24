#pragma once
#include <iostream>
#include <thread>
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

const float safe_distance_car = 210.0f;
const float stop_distance_car = 120.0f;
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
    float speed_;
    float acceleration_;
    bool in_crossing_;
    bool can_turn_;
    sf::Texture texture_; // Ajout de la texture pour les voitures
    

public:
    Car(float x, float y, sf::Vector2f direction, entry destination)
        : shape_(sf::Vector2f(width_car, height_car)), direction_(direction), speed_(max_speed_car), destination_(destination), acceleration_(acceleration_car), in_crossing_(false), can_turn_(false) {
        shape_.setPosition(x, y);
        shape_.setOrigin(width_car / 2, height_car / 2);
        static sf::Texture carTexture;
        // Chargement de la texture
        if (carTexture.getSize().x == 0) {
            if (!carTexture.loadFromFile("car.png")) {
                std::cerr << "Erreur : Impossible de charger l'image car_texture.png" << std::endl;
            }
        }
        shape_.setTexture(&carTexture);



    }

    void move() {
        shape_.move(direction_ * speed_);
        if (direction_ == sf::Vector2f(0, -1)) {
            shape_.setRotation(0);
        }
        else if (direction_ == sf::Vector2f(0, 1)) {
            shape_.setRotation(180);
        }
        else if (direction_ == sf::Vector2f(-1, 0)) {
            shape_.setRotation(270);
        }
        else if (direction_ == sf::Vector2f(1, 0)) {
            shape_.setRotation(90);
        }
    }

    void stop() {
        if (speed_ > 0) {
            speed_ -= acceleration_;
            if (speed_ < 0) speed_ = 0;
        }
    }

    void accelerate() {
        if (speed_ < max_speed_car) {
            speed_ += acceleration_;
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
