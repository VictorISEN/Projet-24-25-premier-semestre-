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
#include "car.hpp"

const float l1 = 400;
const float l2 = 600;
void generate_cars(std::vector<Car>& cars) {
    if (cars.size() >= 15)
        return;
        
    int rdm = rand() % 100;
    bool freeSpace = true;
    switch (rdm)
    {
    case 0:
        for (int i = 0; i < cars.size(); i++) {
            if (cars.at(i).getX() == 485 && cars.at(i).getY() < width_car + safe_distance_car)
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
            cars.push_back(Car(485, 0, sf::Vector2f(0, 1), dest));
        }  
        break;
    case 1:
        for (int i = 0; i < cars.size(); i++)
        {
            if (cars.at(i).getX() == 515 && cars.at(i).getY() > 1000 - width_car - safe_distance_car)
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
            cars.push_back(Car(515, 1000, sf::Vector2f(0, -1), dest));
        }
        break;
    case 2:
        for (int i = 0; i < cars.size(); i++)
        {
            if (cars.at(i).getY() == 515 && cars.at(i).getX() < width_car + safe_distance_car)
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
            cars.push_back(Car(0, 515, sf::Vector2f(1, 0), dest));
            cars.at(cars.size() - 1).getShape().rotate(90);
        }
        break;
    case 3:
        for (int i = 0; i < cars.size(); i++)
        {
            if (cars.at(i).getY() == 485 && cars.at(i).getX() > 1900 - width_car - safe_distance_car)
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
                dest = entry::top;
                break;
            }
            cars.push_back(Car(1900, 485, sf::Vector2f(-1, 0), dest));
            cars.at(cars.size() - 1).getShape().rotate(90);
        }
        break;
    default:
        break;
    }
    return;
}

void Car::turn() {
    if (this->getX() >= l2 || this->getX() <= l1 || this->getY() >= l2 || this->getY() <= l1)
        return;
    if (destination_ == entry::right && direction_ != sf::Vector2f(1, 0) && shape_.getPosition().y >= 510 && shape_.getPosition().y <= 530) {
        direction_ = sf::Vector2f(1, 0);
        shape_.setPosition(shape_.getPosition().x, 515);
        shape_.rotate(90);
    }
    if (destination_ == entry::left && direction_ != sf::Vector2f(-1, 0) && shape_.getPosition().y >= 470 && shape_.getPosition().y <= 490) {
        direction_ = sf::Vector2f(-1, 0);
        shape_.setPosition(shape_.getPosition().x, 485);
        shape_.rotate(90);
    }
    if (destination_ == entry::top && direction_ != sf::Vector2f(0, -1) && shape_.getPosition().x >= 510 && shape_.getPosition().x <= 530) {
        direction_ = sf::Vector2f(0, -1);
        shape_.setPosition(515, shape_.getPosition().y);
        shape_.rotate(90);
    }
    if (destination_ == entry::bottom && direction_ != sf::Vector2f(0, 1) && shape_.getPosition().x >= 470 && shape_.getPosition().x <= 490) {
        direction_ = sf::Vector2f(0, 1);
        shape_.setPosition(485, shape_.getPosition().y);
        shape_.rotate(90);
    }
}

void run_cars(std::vector<Car>& cars, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::stop_token stop_token) {
    srand(time(NULL));
    while (!stop_token.stop_requested())
    {
        generate_cars(cars);
        for (auto it = cars.begin(); it != cars.end();) {
            Car& car = *it;

            bool stop_for_red = false;

            car.turn();

            if (car.getDirection().x > 0) { // Voiture se déplaçant horizontalement
                if (car.getX() > l1 - stop_distance_car - 2 * crossingWidth && car.getX() < l1 - 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (car.getDirection().x < 0) { // Voiture se déplaçant horizontalement
                if (car.getX() < l2 + stop_distance_car + 2 * crossingWidth && car.getX() > l2 + 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (car.getDirection().y > 0) { // Voiture se déplaçant verticalement
                if (car.getY() > l1 - stop_distance_car - 2 * crossingWidth && car.getY() < l1 - 2 * crossingWidth && traffic_lights["vauban"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (car.getDirection().y < 0) { // Voiture se déplaçant verticalement
                if (car.getY() < l2 + stop_distance_car + 2 * crossingWidth && car.getY() > l2 + 2 * crossingWidth && traffic_lights["vauban"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }

            // feu a droite solferino
            if (car.getDirection().x > 0) { // Voiture se déplaçant horizontalement
                if (car.getX() > l2 + 550 - stop_distance_car - 2 * crossingWidth && car.getX() < l2 + 550 - 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (car.getDirection().x < 0) { // Voiture se déplaçant horizontalement
                if (car.getX() < l2 + 650 + stop_distance_car + 2 * crossingWidth && car.getX() > l2 + 650 + 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }

            if (stop_for_red) {
                car.stop();
            }
            else {
                bool too_close = false;
                for (auto& other_car : cars) {
                    if (&other_car != &car && car.getDirection() == other_car.getDirection()) {
                        if ((car.getDirection().x > 0 && other_car.getX() > car.getX()) ||
                            (car.getDirection().y > 0 && other_car.getY() > car.getY()) ||
                            (car.getDirection().x < 0 && other_car.getX() < car.getX()) ||
                            (car.getDirection().y < 0 && other_car.getY() < car.getY())) {
                            if (car.getDistanceTo(other_car) < safe_distance_car) {
                                too_close = true;
                                break;
                            }
                        }
                    }
                }
                if (!too_close) {
                    car.accelerate();
                }
                else {
                    car.stop();
                }
            }

            car.move();

            // Supprimer la voiture si elle sort de l'écran
            if (car.getX() > 1900 || car.getY() > 1000 || car.getX() < 0 || car.getY() < 0) {
                it = cars.erase(it);
            }
            else
                ++it;
        }
        std::this_thread::sleep_for(delay_speed);
    }
}