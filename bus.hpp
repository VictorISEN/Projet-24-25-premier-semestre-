#pragma once
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "pedestrian.hpp"
#include "lights.hpp"

const float safe_distance_bus = 350.0f; // Distance minimale entre deux voitures
const float stop_distance_bus = 190.0f; // Distance pour commencer à s'arrêter au feu rouge
const float max_speed_bus = 20;
const float acceleration_bus = 1.3;
const float width_bus = 150;
const float height_bus = 130;

class Car;
class Bike;

class Bus {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    entry destination_;
    float speed_; // Vitesse actuelle
    float acceleration_; // Accélération
    bool in_crossing_;
    bool can_turn_;
    sf::Texture texture_;

public:
    Bus(float x, float y, sf::Vector2f direction, entry destination)
        : shape_(sf::Vector2f(width_bus, height_bus)), direction_(direction), speed_(max_speed_bus), destination_(destination), acceleration_(acceleration_bus), in_crossing_(false), can_turn_(false) { // Réduire l'accélération
        shape_.setPosition(x, y);
        static sf::Texture carTexture;
        // Chargement de la texture
        if (carTexture.getSize().x == 0) {
            if (!carTexture.loadFromFile("bus.png")) {
                std::cerr << "Erreur : Impossible de charger l'image car_texture.png" << std::endl;
            }
        }
        shape_.setTexture(&carTexture);
        shape_.setOrigin(width_bus / 2, height_bus / 2);
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
            speed_ -= acceleration_; // Décélération progressive
            if (speed_ < 0) speed_ = 0;
        }
    }

    void accelerate() {
        if (speed_ < max_speed_bus) {
            speed_ += acceleration_; // Accélération progressive
            if (speed_ > max_speed_bus) speed_ = max_speed_bus;
        }
    }

    sf::RectangleShape& getShape() { return shape_; }

    float getX() const { return shape_.getPosition().x; }
    float getY() const { return shape_.getPosition().y; }

    sf::Vector2f getDirection() const { return direction_; }

    bool isInCrossing() const { return in_crossing_; }
    void setInCrossing(bool value) { in_crossing_ = value; }

    float getDistanceTo(const Bus& other) const {
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

void run_buses(std::vector<Bus>& buses, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::vector<Car>& cars, std::vector<Bike>& bikes, std::vector<pedestrian>& pedestrians, std::stop_token stop_token);

void generate_buses(std::vector<Bus>& buses);