#pragma once
// condition_variable::wait(with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "lights.hpp"
#include "pedestrian.hpp"
#include <cstdlib>
#include <ctime>
#include "bikes.hpp"
#include "car.hpp"

const float l1 = 400;
const float l2 = 600;
void generate_bikes(std::vector<Bike>& bikes) {
    if (bikes.size() >= 20)
        return;
    int rdm = rand() % 250;
    bool freeSpace = true;
    switch (rdm)
    {
    case 0:
        for (int i = 0; i < bikes.size(); i++) {
            if (bikes.at(i).getX() == 413 && bikes.at(i).getY() < height_bike + safe_distance_bike)
                freeSpace = false;
        }
        if (freeSpace == true) {
            int rdm2 = rand() % 3;
            entry dest;
            switch (rdm2)
            {
            case 0:
                dest = entry::bottom;
                break;
            case 1:
                dest = entry::left;
                break;
            case 2:
                dest = entry::right;
                break;
            }
            bikes.push_back(Bike(413, 0, sf::Vector2f(0, 1), dest));
        }
        break;
    case 1:
        for (int i = 0; i < bikes.size(); i++)
        {
            if (bikes.at(i).getX() == 587 && bikes.at(i).getY() > 1000 - height_bike - safe_distance_bike)
                freeSpace = false;
        }
        if (freeSpace == true) {
            int rdm2 = rand() % 3;
            entry dest;
            switch (rdm2)
            {
            case 0:
                dest = entry::top;
                break;
            case 1:
                dest = entry::left;
                break;
            case 2:
                dest = entry::right;
                break;
            }
            bikes.push_back(Bike(587, 1000, sf::Vector2f(0, -1), dest));
        } 
        break;
    case 2:
        for (int i = 0; i < bikes.size(); i++)
        {
            if (bikes.at(i).getY() == 587 && bikes.at(i).getX() < height_bike + safe_distance_bike)
                freeSpace = false;
        }
        if (freeSpace == true) {
            int rdm2 = rand() % 3;
            entry dest;
            switch (rdm2)
            {
            case 0:
                dest = entry::bottom;
                break;
            case 1:
                dest = entry::right;
                break;
            case 2:
                dest = entry::top;
                break;
            }
            bikes.push_back(Bike(0, 587, sf::Vector2f(1, 0), dest));
            bikes.at(bikes.size() - 1).getShape().rotate(90);
        }
        break;
    case 3  :
        for (int i = 0; i < bikes.size(); i++)
        {
            if (bikes.at(i).getY() == 413 && bikes.at(i).getX() > 1900 - height_bike - safe_distance_bike)
                freeSpace = false;
        }
        if (freeSpace == true) {
            int rdm2 = rand() % 3;
            entry dest;
            switch (rdm2)
            {
            case 0:
                dest = entry::bottom;
                break;
            case 1:
                dest = entry::top;
                break;
            case 2:
                dest = entry::left;
                break;
            }
            bikes.push_back(Bike(1900, 413, sf::Vector2f(-1, 0), dest));
            bikes.at(bikes.size() - 1).getShape().rotate(90);
        }
        break;
    default:
        break;
    }
    return;
}

void Bike::turn() {
    if (this->getX() >= l2 || this->getX() <= l1 || this->getY() >= l2 || this->getY() <= l1)
        return;
    if (destination_ == entry::right && direction_ != sf::Vector2f(1, 0) && shape_.getPosition().y >= 577 && shape_.getPosition().y <= 597) {
        direction_ = sf::Vector2f(1, 0);
        shape_.setPosition(shape_.getPosition().x, 587);
        shape_.rotate(90);
    }
    if (destination_ == entry::left && direction_ != sf::Vector2f(-1, 0) && shape_.getPosition().y >= 403 && shape_.getPosition().y <= 423) {
        direction_ = sf::Vector2f(-1, 0);
        shape_.setPosition(shape_.getPosition().x, 413);
        shape_.rotate(90);
    }
    if (destination_ == entry::top && direction_ != sf::Vector2f(0, -1) && shape_.getPosition().x >= 577 && shape_.getPosition().x <= 597) {
        direction_ = sf::Vector2f(0, -1);
        shape_.setPosition(587, shape_.getPosition().y);
        shape_.rotate(90);
    }
    if (destination_ == entry::bottom && direction_ != sf::Vector2f(0, 1) && shape_.getPosition().x >= 403 && shape_.getPosition().x <= 423) {
        direction_ = sf::Vector2f(0, 1);
        shape_.setPosition(413, shape_.getPosition().y);
        shape_.rotate(90);
    }
}

void run_bikes(std::vector<Bike>& bikes, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::stop_token stop_token) {
    srand(time(NULL));
    while (!stop_token.stop_requested())
    {
        generate_bikes(bikes);
        for (auto it = bikes.begin(); it != bikes.end();) {
            Bike& bike = *it;

            bool stop_for_red = false;

            bike.turn();

            if (bike.getDirection().x > 0) { // Vélo se déplaçant horizontalement
                if (bike.getX() > l1 - stop_distance_bike - 2 * crossingWidth && bike.getX() < l1 - 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bike.getDirection().x < 0) { // Vélo se déplaçant horizontalement
                if (bike.getX() < l2 + stop_distance_bike + 2 * crossingWidth && bike.getX() > l2 + 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bike.getDirection().y > 0) { // Vélo se déplaçant verticalement
                if (bike.getY() > l1 - stop_distance_bike - 2 * crossingWidth && bike.getY() < l1 - 2 * crossingWidth && traffic_lights["vauban"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bike.getDirection().y < 0) { // Vélo se déplaçant verticalement
                if (bike.getY() < l2 + stop_distance_bike + 2 * crossingWidth && bike.getY() > l2 + 2 * crossingWidth && traffic_lights["vauban"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }

            // feu a droite solferino
            if (bike.getDirection().x > 0) { // Vélo se déplaçant horizontalement
                if (bike.getX() > l2 + 550 - stop_distance_bike - 2 * crossingWidth && bike.getX() < l2 + 550 - 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bike.getDirection().x < 0) { // Vélo se déplaçant horizontalement
                if (bike.getX() < l2 + 650 + stop_distance_bike + 2 * crossingWidth && bike.getX() > l2 + 650 + 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }

            if (stop_for_red) {
                bike.stop();
            }
            else {
                bool too_close = false;
                for (auto& other_bike : bikes) {
                    if (&other_bike != &bike && bike.getDirection() == other_bike.getDirection()) {
                        if ((bike.getDirection().x > 0 && other_bike.getX() > bike.getX()) ||
                            (bike.getDirection().y > 0 && other_bike.getY() > bike.getY()) ||
                            (bike.getDirection().x < 0 && other_bike.getX() < bike.getX()) ||
                            (bike.getDirection().y < 0 && other_bike.getY() < bike.getY())) {
                            if (bike.getDistanceTo(other_bike) < safe_distance_bike) {
                                too_close = true;
                                break;
                            }
                        }
                    }
                }
                if (!too_close) {
                    bike.accelerate();
                }
                else {
                    bike.stop();
                }
            }

            bike.move();

            // Supprimer la voiture si elle sort de l'écran
            if (bike.getX() > 1900 || bike.getY() > 1000 || bike.getX() < 0 || bike.getY() < 0)
                it = bikes.erase(it);
            else
                ++it;
        }
        std::this_thread::sleep_for(delay_speed);
    }
}