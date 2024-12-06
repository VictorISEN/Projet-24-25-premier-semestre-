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
#include "bus.hpp"
#include "car.hpp"

const float l1 = 400;
const float l2 = 600;

void generate_buses(std::vector<Bus>& buses) {
    if (buses.size() >= 4)
        return;

    int rdm = rand() % 500;
    bool freeSpace = true;
    switch (rdm)
    {
    case 0:
        for (int i = 0; i < buses.size(); i++) {
            if (buses.at(i).getX() == 448 && buses.at(i).getY() < height_bus + safe_distance_bus)
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
            buses.push_back(Bus(448, 0, sf::Vector2f(0, 1), dest));
        }
        break;
    case 1:
        for (int i = 0; i < buses.size(); i++)
        {
            if (buses.at(i).getX() == 552 && buses.at(i).getY() > 1000 - height_bus - safe_distance_bus)
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
            buses.push_back(Bus(552, 1000, sf::Vector2f(0, -1), dest));
        }
        break;
    case 2:
        for (int i = 0; i < buses.size(); i++)
        {
            if (buses.at(i).getY() == 552 && buses.at(i).getX() < height_bus + safe_distance_bus)
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
            buses.push_back(Bus(0, 552, sf::Vector2f(1, 0), dest));
            buses.at(buses.size() - 1).getShape().rotate(90);
        }
        break;
    case 3:
        for (int i = 0; i < buses.size(); i++)
        {
            if (buses.at(i).getY() == 448 && buses.at(i).getX() > 1900 - height_bus - safe_distance_bus)
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
            buses.push_back(Bus(1900, 448, sf::Vector2f(-1, 0), dest));
            buses.at(buses.size() - 1).getShape().rotate(90);
        }
        break;
    default:
        break;
    }
    return;
}

void run_buses(std::vector<Bus>& buses, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::stop_token stop_token) {
    srand(time(NULL));
    while (!stop_token.stop_requested())
    {
        generate_buses(buses);
        for (auto it = buses.begin(); it != buses.end();) {
            Bus& bus = *it;

            bool stop_for_red = false;

            if (bus.getDirection().x > 0) { // Bus se déplaçant horizontalement
                if (bus.getX() > l1 - stop_distance_bus - 2 * crossingWidth && bus.getX() < l1 - 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bus.getDirection().x < 0) { // Bus se déplaçant horizontalement
                if (bus.getX() < l2 + stop_distance_bus + 2 * crossingWidth && bus.getX() > l2 + 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bus.getDirection().y > 0) { // Bus se déplaçant verticalement
                if (bus.getY() > l1 - stop_distance_bus - 2 * crossingWidth && bus.getY() < l1 -  2 * crossingWidth && traffic_lights["vauban"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }
            if (bus.getDirection().y < 0) { // Bus se déplaçant verticalement
                if (bus.getY() < l2 + stop_distance_bus + 2 * crossingWidth && bus.getY() > l2 + 2 * crossingWidth && traffic_lights["vauban"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }

            // feu a droite solferino
                if (bus.getDirection().x > 0) { // Bus se déplaçant horizontalement
                    if (bus.getX() > l2 + 550 - stop_distance_bus - 2 * crossingWidth && bus.getX() < l2 + 550 - 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                        stop_for_red = true;
                    }
                }
            if (bus.getDirection().x < 0) { // Bus se déplaçant horizontalement
                if (bus.getX() < l2 + 650 + stop_distance_bus + 2 * crossingWidth && bus.getX() > l2 + 650 + 2 * crossingWidth && traffic_lights["solferino"].get_traffic_color() != Traffic_color::green) {
                    stop_for_red = true;
                }
            }

            if (stop_for_red) {
                bus.stop();
            }
            else {
                bool too_close = false;
                for (auto& other_bus : buses) {
                    if (&other_bus != &bus && bus.getDirection() == other_bus.getDirection()) {
                        if ((bus.getDirection().x > 0 && other_bus.getX() > bus.getX()) ||
                            (bus.getDirection().y > 0 && other_bus.getY() > bus.getY()) ||
                            (bus.getDirection().x < 0 && other_bus.getX() < bus.getX()) ||
                            (bus.getDirection().y < 0 && other_bus.getY() < bus.getY())) {
                            if (bus.getDistanceTo(other_bus) < safe_distance_bus) {
                                too_close = true;
                                break;
                            }
                        }
                    }
                }
                if (!too_close) {
                    bus.accelerate();
                }
                else {
                    bus.stop();
                }
            }

            bus.move();

            // Supprimer la voiture si elle sort de l'écran
            if (bus.getX() > 1900 || bus.getY() > 1000 || bus.getX() < 0 || bus.getY() < 0)
                it = buses.erase(it);
            else
                ++it;
        }
        std::this_thread::sleep_for(delay_speed);
    }
}