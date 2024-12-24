#pragma once

#include "bikes.hpp"
#include "car.hpp"
#include "bus.hpp"

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

bool Bike::canTurn(bool CTop, bool CBottom, bool CLeft, bool CRight, std::vector<Bus>& buses, std::vector<Bike>& bikes, std::vector<Car>& cars) {
    if (can_turn_ == true)
        return true;
    if ((destination_ == entry::top && direction_ == sf::Vector2f(0, -1)) ||
        (destination_ == entry::bottom && direction_ == sf::Vector2f(0, 1)) ||
        (destination_ == entry::left && direction_ == sf::Vector2f(-1, 0)) ||
        (destination_ == entry::right && direction_ == sf::Vector2f(1, 0)))
        return true;
    // comme pour les bus mais la si un velo tourne sur sa droite il ne doit verifier aucun vehicule
    // qet si il tourne a sa aguche il doit verifier les 2 sens de voitures et bus et le sens opposé de velos

    // on verifife que les voitures n'empechent pas de tourner (a guache)
    for (int i = 0; i < cars.size(); i++)
    {
        // si il tourne vers le haut
        if (this->getDestination() == entry::top && this->getDirection().x == 1) {
            if ((cars.at(i).getDirection().x == -1 && cars.at(i).getX() >= 515 - 100 && cars.at(i).getX() <= 515 + safe_distance_car + 400 && cars.at(i).getDestination() != entry::top) ||
                (cars.at(i).getDirection().x == 1 && cars.at(i).getX() <= 515 + 100 && cars.at(i).getX() >= 515 - safe_distance_car - 400 && cars.at(i).getDestination() != entry::top) || CTop == false)
                return false;
        }
        // si il tourne vers le bas
        if (this->getDestination() == entry::bottom && this->getDirection().x == -1) {
            if ((cars.at(i).getDirection().x == 1 && cars.at(i).getX() >= 485 - safe_distance_car - 400 && cars.at(i).getX() <= 458 + 100 && cars.at(i).getDestination() != entry::bottom) ||
                (cars.at(i).getDirection().x == -1 && cars.at(i).getX() <= 485 + safe_distance_car + 400 && cars.at(i).getX() >= 458 - 100 && cars.at(i).getDestination() != entry::bottom) || CBottom == false)
                return false;
        }
        // si il tourne vers la gauche
        if (this->getDestination() == entry::left && this->getDirection().y == 1) {
            if ((cars.at(i).getDirection().y == 1 && cars.at(i).getY() >= 485 - safe_distance_car - 400 && cars.at(i).getY() <= 458 + 100 && cars.at(i).getDestination() != entry::left) ||
                (cars.at(i).getDirection().y == -1 && cars.at(i).getY() <= 485 + safe_distance_car + 400 && cars.at(i).getY() >= 458 - 100 && cars.at(i).getDestination() != entry::left) || CLeft == false)
                return false;
        }
        // si il tourne vers la droite
        if (this->getDestination() == entry::right && this->getDirection().y == -1) {
            if ((cars.at(i).getDirection().y == -1 && cars.at(i).getY() >= 515 - 100 && cars.at(i).getY() <= 515 + safe_distance_car + 400 && cars.at(i).getDestination() != entry::right) ||
                (cars.at(i).getDirection().y == 1 && cars.at(i).getY() <= 515 + 100 && cars.at(i).getY() >= 515 - safe_distance_car - 400 && cars.at(i).getDestination() != entry::right) || CRight == false)
                return false;
        }
    }

    // on verifife que les bus n'empechent pas de tourner (a guache)
    for (int i = 0; i < buses.size(); i++)
    {
        // si il tourne vers le haut
        if (this->getDestination() == entry::top && this->getDirection().x == 1) {
            if ((buses.at(i).getDirection().x == -1 && buses.at(i).getX() >= 515 - 100 && buses.at(i).getX() <= 515 + safe_distance_bus + 400 && buses.at(i).getDestination() != entry::top) ||
                (buses.at(i).getDirection().x == 1 && buses.at(i).getX() <= 515 + 100 && buses.at(i).getX() >= 515 - safe_distance_bus - 400 && buses.at(i).getDestination() != entry::top) || CTop == false)
                return false;
        }
        // si il tourne vers le bas
        if (this->getDestination() == entry::bottom && this->getDirection().x == -1) {
            if ((buses.at(i).getDirection().x == 1 && buses.at(i).getX() >= 485 - safe_distance_bus - 400 && buses.at(i).getX() <= 458 + 100 && buses.at(i).getDestination() != entry::bottom) ||
                (buses.at(i).getDirection().x == -1 && buses.at(i).getX() <= 485 + safe_distance_bus + 400 && buses.at(i).getX() >= 458 - 100 && buses.at(i).getDestination() != entry::bottom) || CBottom == false)
                return false;
        }
        // si il tourne vers la gauche
        if (this->getDestination() == entry::left && this->getDirection().y == 1) {
            if ((buses.at(i).getDirection().y == 1 && buses.at(i).getY() >= 485 - safe_distance_bus - 400 && buses.at(i).getY() <= 458 + 100 && buses.at(i).getDestination() != entry::left) ||
                (buses.at(i).getDirection().y == -1 && buses.at(i).getY() <= 485 + safe_distance_bus + 400 && buses.at(i).getY() >= 458 - 100 && buses.at(i).getDestination() != entry::left) || CLeft == false)
                return false;
        }
        // si il tourne vers la droite
        if (this->getDestination() == entry::right && this->getDirection().y == -1) {
            if ((buses.at(i).getDirection().y == -1 && buses.at(i).getY() >= 515 - 100 && buses.at(i).getY() <= 515 + safe_distance_bus + 400 && buses.at(i).getDestination() != entry::right) ||
                (buses.at(i).getDirection().y == 1 && buses.at(i).getY() <= 515 + 100 && buses.at(i).getY() >= 515 - safe_distance_bus - 400 && buses.at(i).getDestination() != entry::right) || CRight == false)
                return false;
        }
    }

    //on verfie que les vélos n empechent pas de tourner vers la gauche
    for (int i = 0; i < bikes.size(); i++)
    {
        if (bikes.at(i).getShape().getPosition() != shape_.getPosition()) {
            if (this->getDestination() == entry::top && this->getDirection().x == 1) {
                if ((bikes.at(i).getDirection().x == -1 && bikes.at(i).getDestination() != entry::top && bikes.at(i).getDestination() != entry::bottom && bikes.at(i).getX() >= 515 - 40 && bikes.at(i).getX() <= 515 + safe_distance_bike + 50) || CTop == false)
                    return false;
            }
            // si il tourne vers le bas
            if (this->getDestination() == entry::bottom && this->getDirection().x == -1) {
                if ((bikes.at(i).getDirection().x == 1 && bikes.at(i).getDestination() != entry::bottom && bikes.at(i).getX() >= 485 - safe_distance_bike - 50 && bikes.at(i).getX() <= 458 + 40) || CBottom == false)
                    return false;
            }
            // si il tourne vers la gauche
            if (this->getDestination() == entry::left && this->getDirection().y == 1) {
                if ((bikes.at(i).getDirection().y == 1 && bikes.at(i).getDestination() != entry::left && bikes.at(i).getDestination() != entry::right && bikes.at(i).getY() >= 485 - safe_distance_bike - 50 && bikes.at(i).getY() <= 458 + 40) || CLeft == false)
                    return false;
            }
            // si il tourne vers la droite
            if (this->getDestination() == entry::right && this->getDirection().y == -1) {
                if ((bikes.at(i).getDirection().y == -1 && bikes.at(i).getDestination() != entry::right && bikes.at(i).getY() >= 515 - 40 && bikes.at(i).getY() <= 515 + safe_distance_bike + 50) || CRight == false)
                    return false;
            }
        }
    }
    return true;
}

void run_bikes(std::vector<Bike>& bikes, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::vector<Bus>& buses, std::vector<Car>& cars, std::vector<pedestrian>& pedestrians, std::stop_token stop_token) {
    srand(time(NULL));
    bool CTop = true, CBottom = true, CLeft = true, CRight = true;
    while (!stop_token.stop_requested())
    {
        generate_bikes(bikes);

        // on regarde si le spassages piétons sont libres
        CTop = true;
        CBottom = true;
        CLeft = true;
        CRight = true;
        freeCrossing(pedestrians, traffic_lights, crossings, CTop, CBottom, CLeft, CRight);

        for (auto it = bikes.begin(); it != bikes.end();) {
            Bike& bike = *it;

            bool stop_for_red = false;
            bool can_turn = true;

            bike.turn();

            if ((bike.getDestination() == entry::top && bike.getX() >= 515 - safe_distance_bike && bike.getX() <= 515 + safe_distance_bike) ||
                (bike.getDestination() == entry::bottom && bike.getX() >= 485 - safe_distance_bike && bike.getX() <= 485 + safe_distance_bike) ||
                (bike.getDestination() == entry::left && bike.getY() >= 485 - safe_distance_bike && bike.getY() <= 485 + safe_distance_bike) ||
                (bike.getDestination() == entry::right && bike.getY() >= 515 - safe_distance_bike && bike.getY() <= 515 + safe_distance_bike)) {
                if (bike.getCanTurn() == false) {
                    can_turn = bike.canTurn(CTop, CBottom, CLeft, CRight, buses, bikes, cars);
                    bike.setCanTurn(can_turn);
                }
            }

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