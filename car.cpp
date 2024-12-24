#pragma once
// condition_variable::wait(with predicate)

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

bool Car::canTurn(bool CTop, bool CBottom, bool CLeft, bool CRight, std::vector<Bus>& buses, std::vector<Bike>& bikes, std::vector<Car>& cars) {
    if (can_turn_ == true) // si a un moment il avait une possibilité de passer, il l as prise donc il fini de toute facon son virage
        return true;

    // on ignore si il continur=e totu droit
    if ((destination_ == entry::top && direction_ == sf::Vector2f(0, -1)) ||
        (destination_ == entry::bottom && direction_ == sf::Vector2f(0, 1)) ||
        (destination_ == entry::left && direction_ == sf::Vector2f(-1, 0)) ||
        (destination_ == entry::right && direction_ == sf::Vector2f(1, 0)))
        return true;

    // on verifife que les bus n'empechent pas de tourner
    for (int i = 0; i < buses.size(); i++)
    {
        // a chauqes test, on ne prends pas en compte si le bus tourne dans la meme direction que la voiture (leurs trajectoire ne se croisent pas)
        // si il tourne vers le haut
        if (this->getDestination() == entry::top && this->getDirection().x != 0) {
            // si un bus arrive de la droite on verifie qu il soit a une distance minimum de l intersection
            // si ce n est pas le cas (donc que la condition est vraie), la voiture ne peut pas tourner
            // 515 et 485 est le point ou il tourne et 150 ou 40 sont des distance de securité a peut etre recalibrer
            if ((buses.at(i).getDirection().x == -1 && buses.at(i).getX() >= 515 - 40 && buses.at(i).getX() <= 515 + safe_distance_bus + 150 && buses.at(i).getDestination() != entry::top && buses.at(i).getDestination() != entry::bottom) || CTop == false)
                return false;
        }
        // si il tourne vers le bas
        if (this->getDestination() == entry::bottom && this->getDirection().x != 0) {
            // si un bus arrive de la gauche on verifie qu il soit a une distance minimum de l intersection
            if ((buses.at(i).getDirection().x == 1 && buses.at(i).getX() >= 485 - safe_distance_bus - 150 && buses.at(i).getX() <= 458 + 40 && buses.at(i).getDestination() != entry::bottom && buses.at(i).getDestination() != entry::top) || CBottom == false)
                return false;
        }
        // si il tourne vers la gauche
        if (this->getDestination() == entry::left && this->getDirection().y != 0) {
            // si un bus arrive du haut on verifie qu il soit a une distance minimum de l intersection
            if ((buses.at(i).getDirection().y == 1 && buses.at(i).getY() >= 485 - safe_distance_bus - 150 && buses.at(i).getY() <= 458 + 40 && buses.at(i).getDestination() != entry::left && buses.at(i).getDestination() != entry::right) || CLeft == false)
                return false;
        }

        // si il tourne vers la droite
        if (this->getDestination() == entry::right && this->getDirection().y != 0) {
            // si un bus arrive du bas on verifie qu il soit a une distance minimum de l intersection
            if ((buses.at(i).getDirection().y == -1 && buses.at(i).getY() >= 515 - 40 && buses.at(i).getY() <= 515 + safe_distance_bus + 150 && buses.at(i).getDestination() != entry::right && buses.at(i).getDestination() != entry::left) || CRight == false)
                return false;
        }
    }
    
    //on verifie que les velos n empechent pas de tourner, fonctionne comme pour les bus
    for (int i = 0; i < bikes.size(); i++)
    {
        // si il tourne vers le haut
        if (this->getDestination() == entry::top && this->getDirection().x != 0) {
            if ((bikes.at(i).getDirection().x == -1 && bikes.at(i).getDestination() != entry::top && bikes.at(i).getX() >= 515 - 40 && bikes.at(i).getX() <= 515 + safe_distance_bike + 100) || CTop == false)
                return false;
        }
        // si il tourne vers le bas
        if (this->getDestination() == entry::bottom && this->getDirection().x != 0) {
            if ((bikes.at(i).getDirection().x == 1 && bikes.at(i).getDestination() != entry::bottom && bikes.at(i).getX() >= 485 - safe_distance_bike - 100 && bikes.at(i).getX() <= 458 + 40) || CBottom == false)
                return false;
        }
        // si il tourne vers la gauche
        if (this->getDestination() == entry::left && this->getDirection().y != 0) {
            if ((bikes.at(i).getDirection().y == 1 && bikes.at(i).getDestination() != entry::left && bikes.at(i).getY() >= 485 - safe_distance_bike - 100 && bikes.at(i).getY() <= 458 + 40) || CLeft == false)
                return false;
        }
        // si il tourne vers la droite
        if (this->getDestination() == entry::right && this->getDirection().y != 0) {
            if ((bikes.at(i).getDirection().y == -1 && bikes.at(i).getDestination() != entry::right && bikes.at(i).getY() >= 515 - 40 && bikes.at(i).getY() <= 515 + safe_distance_bike + 100) || CRight == false)
                return false;
        }
    }

    // presque comme pour les autre mais on verifie uniquement si la voiture actuelle veut tourner vers sa gauche
    // et on a un systeme de priorite pour eviter que 2 voitures ne se retrouvent bloquées face a face car les 2 doivent laisser passer l autre
    for (int i = 0; i < cars.size(); i++)
    {
        if (cars.at(i).getShape().getPosition() != shape_.getPosition()) {
            if (this->getDestination() == entry::top && this->getDirection().x == 1) {
                if ((cars.at(i).getDirection().x == -1 && cars.at(i).getDestination() != entry::top && cars.at(i).getDestination() != entry::bottom && cars.at(i).getX() >= 515 - 40 && cars.at(i).getX() <= 515 + safe_distance_car + 100) || CTop == false)
                    return false;
            }
            // si il tourne vers le bas
            if (this->getDestination() == entry::bottom && this->getDirection().x == -1) {
                if ((cars.at(i).getDirection().x == 1 && cars.at(i).getDestination() != entry::bottom && cars.at(i).getX() >= 485 - safe_distance_car - 100 && cars.at(i).getX() <= 458 + 40) || CBottom == false)
                    return false;
            }
            // si il tourne vers la gauche
            if (this->getDestination() == entry::left && this->getDirection().y == 1) {
                if ((cars.at(i).getDirection().y == 1 && cars.at(i).getDestination() != entry::left && cars.at(i).getDestination() != entry::right && cars.at(i).getY() >= 485 - safe_distance_car - 100 && cars.at(i).getY() <= 458 + 40) || CLeft == false)
                    return false;
            }
            // si il tourne vers la droite
            if (this->getDestination() == entry::right && this->getDirection().y == -1) {
                if ((cars.at(i).getDirection().y == -1 && cars.at(i).getDestination() != entry::right && cars.at(i).getY() >= 515 - 40 && cars.at(i).getY() <= 515 + safe_distance_car + 100) || CRight == false)
                    return false;
            }
        }
    }
    return true;
}

void run_cars(std::vector<Car>& cars, std::map<std::string, Crossing>& crossings, std::map<std::string, Traffic_light>& traffic_lights, std::vector<Bus>& buses, std::vector<Bike>& bikes, std::vector<pedestrian>& pedestrians, std::stop_token stop_token) {
    srand(time(NULL));
    bool CTop = true, CBottom = true, CLeft = true, CRight = true;
    while (!stop_token.stop_requested())
    {
        generate_cars(cars);

        // on regarde si le spassages piétons sont libres
        CTop = true;
        CBottom = true;
        CLeft = true;
        CRight = true;

        freeCrossing(pedestrians, traffic_lights, crossings, CTop, CBottom, CLeft, CRight);
        std::cout << "T" << "B" << "L" << "R" << std::endl;
        std::cout << CTop << CBottom << CLeft << CRight << std::endl << std::endl;
        
        for (auto it = cars.begin(); it != cars.end();) {
            Car& car = *it;

            bool stop_for_red = false;
            bool can_turn = true;

            car.turn();

            // on ne regarde si il peut tourner que si il est assez proche de la ou il tourne
            if ((car.getDestination() == entry::top && car.getX() >= 515 - safe_distance_car && car.getX() <= 515 + safe_distance_car) ||
                (car.getDestination() == entry::bottom && car.getX() >= 485 - safe_distance_car && car.getX() <= 485 + safe_distance_car) ||
                (car.getDestination() == entry::left && car.getY() >= 485 - safe_distance_car && car.getY() <= 485 + safe_distance_car) ||
                (car.getDestination() == entry::right && car.getY() >= 515 - safe_distance_car && car.getY() <= 515 + safe_distance_car)) {
                if (car.getCanTurn() == false) {
                    // canTurn(...); verifie si il peut tourner, on fait les test a chaque deplacement
                    // jusqu a ce que il ait le droit (tant que c est pas le cas il freine)
                    can_turn = car.canTurn(CTop, CBottom, CLeft, CRight, buses, bikes, cars);
                    // le vehicule posede une propriete can_turn_
                    car.setCanTurn(can_turn);
                }
            }

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
                if (!too_close && can_turn) {
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