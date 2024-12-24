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

const int l1 = 400;
const int l2 = 600;

using namespace std::chrono_literals;

bool pedestrian::canCross(Crossing crossing) {
	if (crossing.get_crossing_color() == Crossing_color::green)
		return(true);
	return(false);
};

sf::CircleShape pedestrian::getShape() {
	return shape_;
}

void pedestrian::move(std::map<std::string, Crossing>& crossings) {
	if (direction_ == sf::Vector2f(1, 0) && shape_.getPosition().x >= 320 && shape_.getPosition().x <= l1) {
		if (crossings["bottom"].get_crossing_color() == Crossing_color::red)
			return;
	}
	if (direction_ == sf::Vector2f(-1, 0) && shape_.getPosition().x >= l2 && shape_.getPosition().x <= 670) {
		if (crossings["top"].get_crossing_color() == Crossing_color::red)
			return;
	}
	if (direction_ == sf::Vector2f(0, 1) && shape_.getPosition().y >= 320 && shape_.getPosition().y <= l1) {
		if (crossings["left"].get_crossing_color() == Crossing_color::red)
			return;
	}
	if (direction_ == sf::Vector2f(0, -1) && shape_.getPosition().y >= l2 && shape_.getPosition().y <= 670) {
		if (crossings["right"].get_crossing_color() == Crossing_color::red)
			return;
	}
	
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

	shape_.move(direction_ * pedestrianSpeed);

	return;
}

float pedestrian::getPosition_x() {
	return shape_.getPosition().x;
}

float pedestrian::getPosition_y() {
	return shape_.getPosition().y;
}

void pedestrian::setPosition_x(const float nb) {
	shape_.setPosition(nb, shape_.getPosition().y);
}

void pedestrian::setPosition_y(const float nb) {
	shape_.setPosition(shape_.getPosition().x, nb);
}

float pedestrian::distanceX(pedestrian& p) {
	return abs(shape_.getPosition().x - p.getPosition_x());
}

float pedestrian::distanceY(pedestrian& p) {
	return abs(shape_.getPosition().y - p.getPosition_y());
}

sf::Vector2f pedestrian::getDirection() {
	return direction_;
}

sf::Vector2f pedestrian::getPosition() {
	return shape_.getPosition();
}

void pedestrian::setDirection(const sf::Vector2f direction) {
	direction_ = direction;
}

entry pedestrian::getDestination() {
	return destination_;
}

bool canMove(std::vector<pedestrian>& pedestrians, pedestrian& p) {
	for (int i = 0; i < pedestrians.size(); i++)
	{
		if (p.getPosition() != pedestrians.at(i).getPosition()) {
			if (p.getDirection().y == 1) {
				if (pedestrians.at(i).getPosition_y() > p.getPosition().y && pedestrians.at(i).getPosition_y() <= p.getPosition().y + 30) {
					if (pedestrians.at(i).getPosition_x() > p.getPosition().x - 10 && pedestrians.at(i).getPosition_x() <= p.getPosition().x + 10)
						return false;
				}
			}
			if (p.getDirection().y == -1) {
				if (pedestrians.at(i).getPosition_y() < p.getPosition().y && pedestrians.at(i).getPosition_y() >= p.getPosition().y - 30) {
					if (pedestrians.at(i).getPosition_x() > p.getPosition().x - 10 && pedestrians.at(i).getPosition_x() <= p.getPosition().x + 10)
						return false;
				}
			}
			if (p.getDirection().x == 1) {
				if (pedestrians.at(i).getPosition_x() > p.getPosition().x && pedestrians.at(i).getPosition_x() <= p.getPosition().x + 30) {
					if (pedestrians.at(i).getPosition_y() > p.getPosition().y - 10 && pedestrians.at(i).getPosition_y() <= p.getPosition().y + 10)
						return false;
				}
			}
			if (p.getDirection().x == pedestrians.at(i).getDirection().x && p.getDirection().x == -1) {
				if (pedestrians.at(i).getPosition_x() < p.getPosition().x && pedestrians.at(i).getPosition_x() >= p.getPosition().x - 30) {
					if (pedestrians.at(i).getPosition_y() > p.getPosition().y - 10 && pedestrians.at(i).getPosition_y() <= p.getPosition().y + 10)
						return false;
				}
			}
		}
	}
	return(true);
}

void generatePedestrians(std::vector<pedestrian>& pedestrians) {
	if (pedestrians.size() >= 10)
		return;
	int rdm = rand() % 1000;
	bool freeSpace = true;
	switch (rdm)
	{
	case 0:
		for (int i = 0; i < pedestrians.size(); i++) {
			if (pedestrians.at(i).getPosition_x() == 390 && pedestrians.at(i).getPosition_y() < 50)
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
			pedestrians.push_back(pedestrian(390, 0, dest, sf::Vector2f(0, 1)));
		}
		break;
	case 2:
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_x() == 610 && pedestrians.at(i).getPosition_y() > 950)
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
			pedestrians.push_back(pedestrian(610, 1000, dest, sf::Vector2f(0, -1)));
		}
		break;
	case 4:
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_y() == 610 && pedestrians.at(i).getPosition_x() < 50)
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
			pedestrians.push_back(pedestrian(0, 610, dest, sf::Vector2f(1, 0)));
		}
		break;
	case 6:
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_y() == 3905 && pedestrians.at(i).getPosition_x() > 1850)
				freeSpace = false;
		}
		if (freeSpace == true)
		{
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
			pedestrians.push_back(pedestrian(1900, 390, dest, sf::Vector2f(-1, 0)));
		}
		break;
	case 1:
		for (int i = 0; i < pedestrians.size(); i++) {
			if (pedestrians.at(i).getPosition_x() == 640 && pedestrians.at(i).getPosition_y() < 50)
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
			pedestrians.push_back(pedestrian(640, 0, dest, sf::Vector2f(0, 1)));
		}
		break;
	case 3:
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_x() == 360 && pedestrians.at(i).getPosition_y() > 950)
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
			pedestrians.push_back(pedestrian(360, 1000, dest, sf::Vector2f(0, -1)));
		}
		break;
	case 5:
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_y() == 360 && pedestrians.at(i).getPosition_x() < 50)
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
			pedestrians.push_back(pedestrian(0, 360, dest, sf::Vector2f(1, 0)));
		}
		break;
	case 7:
		for (int i = 0; i < pedestrians.size(); i++)
		{
			if (pedestrians.at(i).getPosition_y() == 640 && pedestrians.at(i).getPosition_x() > 1850)
				freeSpace = false;
		}
		if (freeSpace == true)
		{
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
			pedestrians.push_back(pedestrian(1900, 640, dest, sf::Vector2f(-1, 0)));
		}
		break;
	default:
		break;
	}
	return;
}

void run_pedestrians(std::vector<pedestrian>& pedestrians, std::map<std::string, Crossing>& crossings, std::stop_token stop_token)
{
	srand(time(NULL));
	while (!stop_token.stop_requested())
	{
		generatePedestrians(pedestrians);
		for (int i = 0; i < pedestrians.size(); i++)
		{
			//tourner
			if ((pedestrians.at(i).getPosition_x() == l2 + pedestrianSize || pedestrians.at(i).getPosition_x() == l1 - 3 * pedestrianSize - 10) && pedestrians.at(i).getDestination() == entry::top)
				pedestrians.at(i).setDirection(sf::Vector2f(0, -1)); // vers le haut
			if ((pedestrians.at(i).getPosition_x() == l1 - pedestrianSize || pedestrians.at(i).getPosition_x() == l2 + 3 * pedestrianSize + 10) && pedestrians.at(i).getDestination() == entry::bottom)
				pedestrians.at(i).setDirection(sf::Vector2f(0, 1)); // vers le bas
			if ((pedestrians.at(i).getPosition_y() == l1 - pedestrianSize || pedestrians.at(i).getPosition_y() == l2 + 3 * pedestrianSize + 10) && pedestrians.at(i).getDestination() == entry::left)
				pedestrians.at(i).setDirection(sf::Vector2f(-1, 0));//gauche
			if ((pedestrians.at(i).getPosition_y() == l2 + pedestrianSize || pedestrians.at(i).getPosition_y() == l1 - 3 * pedestrianSize - 10) && pedestrians.at(i).getDestination() == entry::right)
				pedestrians.at(i).setDirection(sf::Vector2f(1, 0));//droite

			if (canMove(pedestrians, pedestrians.at(i)) == true) {
				pedestrians.at(i).move(crossings);
				if (pedestrians.at(i).getPosition_x() >= 1900 || pedestrians.at(i).getPosition_y() >= 1000 || pedestrians.at(i).getPosition_x() <= 0 || pedestrians.at(i).getPosition_y() <= 0)
					pedestrians.erase(pedestrians.begin() + i);
			}
		}
		std::this_thread::sleep_for(speed_delay);
	}
}

void freeCrossing(std::vector<pedestrian>& pedestrians, std::map<std::string, Traffic_light>& lights, std::map<std::string, Crossing>& crossings, bool& CTop, bool& CBottom, bool& CLeft, bool& CRight) {
	for (int i = 0; i < pedestrians.size(); i++)
	{
		// on verifie pour chauqe pieton qu il n est pas proche d un passage qu il va traverser
		if ((pedestrians.at(i).getDestination() == entry::top && pedestrians.at(i).getPosition().y > l1 && pedestrians.at(i).getPosition().y <= l2 + 50) ||
			(pedestrians.at(i).getDestination() == entry::bottom && pedestrians.at(i).getPosition().y < l2 && pedestrians.at(i).getPosition().y >= l1 - 500)) {
			if (pedestrians.at(i).getPosition().x > l1 && pedestrians.at(i).getPosition().x < l2 && crossings["top"].get_crossing_color() == Crossing_color::green) {
				if (pedestrians.at(i).getPosition().x > l1 - 50 && pedestrians.at(i).getPosition().x <= l1)
					CLeft = false;
				if (pedestrians.at(i).getPosition().x >= l2 && pedestrians.at(i).getPosition().x < l2 + 50)
					CRight = false;
			}
			else {
				if ((!(pedestrians.at(i).getPosition().x < l1 - 50 || pedestrians.at(i).getPosition().x > l2 + 50) && lights["vauban"].get_traffic_color() == Traffic_color::green)) {
					if (pedestrians.at(i).getPosition().x > l1 - 50 && pedestrians.at(i).getPosition().x <= l1)
						CLeft = false;
					if (pedestrians.at(i).getPosition().x >= l2 && pedestrians.at(i).getPosition().x < l2 + 50)
						CRight = false;
				}
			}

		}
		if ((pedestrians.at(i).getDestination() == entry::left && pedestrians.at(i).getPosition().y > l1 && pedestrians.at(i).getPosition().y <= l2 + 50) ||
			(pedestrians.at(i).getDestination() == entry::right && pedestrians.at(i).getPosition().y < l2 && pedestrians.at(i).getPosition().y >= l1 - 50)) {
			if (pedestrians.at(i).getPosition().y > l1 && pedestrians.at(i).getPosition().y < l2 && crossings["left"].get_crossing_color() == Crossing_color::green) {
				if (pedestrians.at(i).getPosition().y > l1 - 50 && pedestrians.at(i).getPosition().y <= l1)
					CTop = false;
				if (pedestrians.at(i).getPosition().y >= l2 && pedestrians.at(i).getPosition().y < l2 + 50)
					CBottom = false;
			}
			else {
				if ((!(pedestrians.at(i).getPosition().y < l1 - 50 || pedestrians.at(i).getPosition().y > l2 + 50) && lights["solferino"].get_traffic_color() == Traffic_color::green)) {
					if (pedestrians.at(i).getPosition().y > l1 - 50 && pedestrians.at(i).getPosition().y <= l1)
						CTop = false;
					if (pedestrians.at(i).getPosition().y >= l2 && pedestrians.at(i).getPosition().y < l2 + 50)
						CBottom = false;
				}
			}

		}
	}
	return;
}