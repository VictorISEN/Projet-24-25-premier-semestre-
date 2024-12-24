#pragma once
// condition_variable::wait(with predicate)

#include "bus.hpp"
#include "car.hpp"
#include "bikes.hpp"

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

void Bus::turn() {
    if (this->getX() >= l2 || this->getX() <= l1 || this->getY() >= l2 || this->getY() <= l1)
        return;
    if (destination_ == entry::right && direction_ != sf::Vector2f(1, 0) && shape_.getPosition().y >= 542 && shape_.getPosition().y <= 562) {
        direction_ = sf::Vector2f(1, 0);
        shape_.setPosition(shape_.getPosition().x, 552);
        shape_.rotate(90);
    }
    if (destination_ == entry::left && direction_ != sf::Vector2f(-1, 0) && shape_.getPosition().y >= 438 && shape_.getPosition().y <= 458) {
        direction_ = sf::Vector2f(-1, 0);
        shape_.setPosition(shape_.getPosition().x, 448);
        shape_.rotate(90);
    }
    if (destination_ == entry::top && direction_ != sf::Vector2f(0, -1) && shape_.getPosition().x >= 542 && shape_.getPosition().x <= 562) {
        direction_ = sf::Vector2f(0, -1);
        shape_.setPosition(552, shape_.getPosition().y);
        shape_.rotate(90);
    }
    if (destination_ == entry::bottom && direction_ != sf::Vector2f(0, 1) && shape_.getPosition().x >= 438 && shape_.getPosition().x <= 458) {
        direction_ = sf::Vector2f(0, 1);
        shape_.setPosition(448, shape_.getPosition().y);
        shape_.rotate(90);
    }
}

bool Bus::canTurn(bool CTop, bool CBottom, bool CLeft, bool CRight, std::vector<Bus>& buses, std::vector<Bike>& bikes, std::vector<Car>& cars) {
    if (can_turn_ == true)
        return true;
    if ((destination_ == entry::top && direction_ == sf::Vector2f(0, -1)) ||
        (destination_ == entry::bottom && direction_ == sf::Vector2f(0, 1)) ||
        (destination_ == entry::left && direction_ == sf::Vector2f(-1, 0)) ||
        (destination_ == entry::right && direction_ == sf::Vector2f(1, 0)))
        return true;

    // comme pour les voitures mais la si un bus tourne sur sa droite il ne doit que verifier les velos
    // qui sont dans le meme sens que lui et si il tourne a sa aguche il doit verifier les 2 sens de voitures
    // et le sens opposé de velo et bus

    // on verifife que les voitures n'empechent pas de tourner (a guache)
    for (int i = 0; i < cars.size(); i++)
    {
        // si il tourne vers le haut
        if (this->getDestination() == entry::top && this->getDirection().x == 1) {
            if ((cars.at(i).getDirection().x == -1 && cars.at(i).getX() >= 515 - 40 && cars.at(i).getX() <= 515 + safe_distance_car + 100 && cars.at(i).getDestination() != entry::top) ||
                (cars.at(i).getDirection().x == 1 && cars.at(i).getX() <= 515 + 40 && cars.at(i).getX() >= 515 - safe_distance_car - 100 && cars.at(i).getDestination() != entry::top) || CTop == false)
                return false;
        }
        // si il tourne vers le bas
        if (this->getDestination() == entry::bottom && this->getDirection().x == -1) {
            if ((cars.at(i).getDirection().x == 1 && cars.at(i).getX() >= 485 - safe_distance_car - 100 && cars.at(i).getX() <= 458 + 40 && cars.at(i).getDestination() != entry::bottom) ||
                (cars.at(i).getDirection().x == -1 && cars.at(i).getX() <= 485 + safe_distance_car + 100 && cars.at(i).getX() >= 458 - 40 && cars.at(i).getDestination() != entry::bottom) || CBottom == false)
                return false;
        }
        // si il tourne vers la gauche
        if (this->getDestination() == entry::left && this->getDirection().y == 1) {
            if ((cars.at(i).getDirection().y == 1 && cars.at(i).getY() >= 485 - safe_distance_car - 100 && cars.at(i).getY() <= 458 + 40 && cars.at(i).getDestination() != entry::left) ||
                (cars.at(i).getDirection().y == -1 && cars.at(i).getY() <= 485 + safe_distance_car + 100 && cars.at(i).getY() >= 458 - 40 && cars.at(i).getDestination() != entry::left) || CLeft == false)
                return false;
        }
        // si il tourne vers la droite
        if (this->getDestination() == entry::right && this->getDirection().y == -1) {
            if ((cars.at(i).getDirection().y == -1 && cars.at(i).getY() >= 515 - 40 && cars.at(i).getY() <= 515 + safe_distance_car + 100 && cars.at(i).getDestination() != entry::right) ||
                (cars.at(i).getDirection().y == 1 && cars.at(i).getY() <= 515 + 40 && cars.at(i).getY() >= 515 - safe_distance_car - 100 && cars.at(i).getDestination() != entry::right) || CRight == false)
                return false;
        }
    }

    //on verifie que les velos n empechent pas de tourner
    for (int i = 0; i < bikes.size(); i++)
    {
        // si il tourne vers le haut
        if (this->getDestination() == entry::top && this->getDirection().x != 0) {
            if ((bikes.at(i).getDirection().x == -1 && bikes.at(i).getDestination() != entry::top && bikes.at(i).getX() >= 515 - 20 && bikes.at(i).getX() <= 515 + safe_distance_bike + 50) || CTop == false)
                return false;
        }
        // si il tourne vers le bas
        if (this->getDestination() == entry::bottom && this->getDirection().x != 0) {
            if ((bikes.at(i).getDirection().x == 1 && bikes.at(i).getDestination() != entry::bottom && bikes.at(i).getX() >= 485 - safe_distance_bike - 50 && bikes.at(i).getX() <= 458 + 20) || CBottom == false)
                return false;
        }
        // si il tourne vers la gauche
        if (this->getDestination() == entry::left && this->getDirection().y != 0) {
            if ((bikes.at(i).getDirection().y == 1 && bikes.at(i).getDestination() != entry::left && bikes.at(i).getY() >= 485 - safe_distance_bike - 50 && bikes.at(i).getY() <= 458 + 20) || CLeft == false)
                return false;
        }
        // si il tourne vers la droite
        if (this->getDestination() == entry::right && this->getDirection().y != 0) {
            if ((bikes.at(i).getDirection().y == -1 && bikes.at(i).getDestination() != entry::right && bikes.at(i).getY() >= 515 - 20 && bikes.at(i).getY() <= 515 + safe_distance_bike + 50) || CRight == false)
                return false;
        }
    }

    //on verfie que les bus n empechent pas de tourner vers la gauche
    for (int i = 0; i < buses.size(); i++)
    {
        if (buses.at(i).getShape().getPosition() != shape_.getPosition()) {
            if (this->getDestination() == entry::top && this->getDirection().x == 1) {
                if ((buses.at(i).getDirection().x == -1 && buses.at(i).getDestination() != entry::top && buses.at(i).getDestination() != entry::bottom && buses.at(i).getX() >= 515 - 40 && buses.at(i).getX() <= 515 + safe_distance_bus + 50) || CTop == false)
                    return false;
            }
            // si il tourne vers le bas
            if (this->getDestination() == entry::bottom && this->getDirection().x == -1) {
                if ((buses.at(i).getDirection().x == 1 && buses.at(i).getDestination() != entry::bottom && buses.at(i).getX() >= 485 - safe_distance_bus - 50 && buses.at(i).getX() <= 458 + 40) || CBottom == false)
                    return false;
            }
            // si il tourne vers la gauche
            if (this->getDestination() == entry::left && this->getDirection().y == 1) {
                if ((buses.at(i).getDirection().y == 1 && buses.at(i).getDestination() != entry::left && buses.at(i).getDestination() != entry::right && buses.at(i).getY() >= 485 - safe_distance_bus - 50 && buses.at(i).getY() <= 458 + 40) || CLeft == false)
                    return false;
            }
            // si il tourne vers la droite
            if (this->getDestination() == entry::right && this->getDirection().y == -1) {
                if ((buses.at(i).getDirection().y == -1 && buses.at(i).getDestination() != entry::right && buses.at(i).getY() >= 515 - 40 && buses.at(i).getY() <= 515 + safe_distance_bus + 50) || CRight == false)
                    return false;
            }
        }
    }
    return true;
}

void run_buses(std::vector<Bus>& buses, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::vector<Car>& cars, std::vector<Bike>& bikes, std::vector<pedestrian>& pedestrians, std::stop_token stop_token) {
    srand(time(NULL));
    bool CTop = true, CBottom = true, CLeft = true, CRight = true;
    while (!stop_token.stop_requested())
    {
        generate_buses(buses);

        // on regarde si le spassages piétons sont libres
        CTop = true;
        CBottom = true;
        CLeft = true;
        CRight = true;
        freeCrossing(pedestrians, traffic_lights, crossings, CTop, CBottom, CLeft, CRight);

        for (auto it = buses.begin(); it != buses.end();) {
            Bus& bus = *it;

            bool stop_for_red = false;
            bool can_turn = true;

            bus.turn();

            if ((bus.getDestination() == entry::top && bus.getX() >= 515 - safe_distance_bus && bus.getX() <= 515 + safe_distance_bus) ||
                (bus.getDestination() == entry::bottom && bus.getX() >= 485 - safe_distance_bus && bus.getX() <= 485 + safe_distance_bus) ||
                (bus.getDestination() == entry::left && bus.getY() >= 485 - safe_distance_bus && bus.getY() <= 485 + safe_distance_bus) ||
                (bus.getDestination() == entry::right && bus.getY() >= 515 - safe_distance_bus && bus.getY() <= 515 + safe_distance_bus)) {
                if (bus.getCanTurn() == false) {
                    can_turn = bus.canTurn(CTop, CBottom, CLeft, CRight, buses, bikes, cars);
                    bus.setCanTurn(can_turn);
                }
            }

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
                if (!too_close && can_turn) {
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