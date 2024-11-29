// condition_variable::wait (with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "lights.hpp"
#include "car.hpp"
#include <vector>

int main()
{
    std::stop_source stopping;
    std::map<std::string, Traffic_light> traffic_lights;
    std::map<std::string, Crossing> crossings;

    // Initialisation des voitures
   
   // Initialisation des voitures avec une vitesse maximale réaliste
// Initialisation des voitures avec une vitesse maximale réaliste
    std::vector<Car> cars;

    traffic_lights["solferino"] = Traffic_light(Traffic_color::green, 0.0, 0.0);
    traffic_lights["vauban"] = Traffic_light(Traffic_color::red, 0.0, 0.0);
    crossings["top"] = Crossing(Crossing_color::red, 0.0, 0.0);
    crossings["bottom"] = Crossing(Crossing_color::red, 0.0, 0.0);
    crossings["left"] = Crossing(Crossing_color::red, 0.0, 0.0);
    crossings["right"] = Crossing(Crossing_color::red, 0.0, 0.0);
    std::jthread thread_traffic_light_master(run_traffic_light,
        std::ref(traffic_lights), std::ref(crossings), stopping.get_token());

    std::jthread write_traffic_light(print_traffic_light,
        std::ref(traffic_lights), std::ref(crossings), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");

    float l1 = 350, l2 = 430, size = 800, radius = 10;
    sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(0, l1)), sf::Vertex(sf::Vector2f(size, l1)) };
    sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(0, l2)), sf::Vertex(sf::Vector2f(size, l2)) };
    sf::Vertex line3[] = { sf::Vertex(sf::Vector2f(l1, 0)), sf::Vertex(sf::Vector2f(l1, size)) };
    sf::Vertex line4[] = { sf::Vertex(sf::Vector2f(l2, 0)), sf::Vertex(sf::Vector2f(l2, size)) };

    sf::CircleShape circle1(radius);
    circle1.setFillColor(sf::Color::Blue);
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2);
    circle1.setPosition(l1 + radius / 2, l1 - radius);
    sf::CircleShape circle2(radius);
    circle2.setFillColor(sf::Color::Green);
    circle2.setOrigin(circle2.getRadius() / 2, circle2.getRadius() / 2);
    circle2.setPosition(l2 + radius / 2, l1 + radius / 2);
    sf::CircleShape circle3(radius);
    circle3.setFillColor(sf::Color::Blue);
    circle3.setOrigin(circle3.getRadius() / 2, circle3.getRadius() / 2);
    circle3.setPosition(l2 - radius, l2 + radius / 2);
    sf::CircleShape circle4(radius);
    circle4.setFillColor(sf::Color::Green);
    circle4.setOrigin(circle4.getRadius() / 2, circle4.getRadius() / 2);
    circle4.setPosition(l1 - radius, l2 - radius);

    sf::RectangleShape rectangle1(sf::Vector2f(l2 - l1, 25.f));
    rectangle1.setFillColor(sf::Color::Red);
    rectangle1.setPosition(l1, l1 - 100);
    sf::RectangleShape rectangle2(sf::Vector2f(25.f, l2 - l1));
    rectangle2.setFillColor(sf::Color::Red);
    rectangle2.setPosition(l2 + 100, l1);
    sf::RectangleShape rectangle3(sf::Vector2f(l2 - l1, 25.f));
    rectangle3.setFillColor(sf::Color::Red);
    rectangle3.setPosition(l1, l2 + 100);
    sf::RectangleShape rectangle4(sf::Vector2f(25.f, l2 - l1));
    rectangle4.setFillColor(sf::Color::Red);
    rectangle4.setPosition(l1 - 100, l1);

    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                stopping.request_stop();
                window.close();
                return 0;
            }
        }
        window.clear(sf::Color::Black);




       

        const float safe_distance = 150.0f; // Distance minimale entre deux voitures
        const float stop_distance = 120.0f; // Distance pour commencer à s'arrêter au feu rouge

        for (auto it = cars.begin(); it != cars.end();) {
            Car& car = *it;

            bool stop_for_red = false;

            if (car.getDirection().x > 0) { // Voiture se déplaçant horizontalement
                if (car.getX() > l1 - stop_distance && car.getX() < l1 && traffic_lights["solferino"].get_traffic_color() == Traffic_color::red) {
                    stop_for_red = true;
                }
            }
            else if (car.getDirection().y > 0) { // Voiture se déplaçant verticalement
                if (car.getY() > l1 - stop_distance && car.getY() < l1 && traffic_lights["vauban"].get_traffic_color() == Traffic_color::red) {
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
                            (car.getDirection().y > 0 && other_car.getY() > car.getY())) {
                            if (car.getDistanceTo(other_car) < safe_distance) {
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
            if (car.getX() > window.getSize().x || car.getY() > window.getSize().y) {
                it = cars.erase(it);
            }
            else {
                window.draw(car.getShape());
                ++it;
            }
        }
        static sf::Clock clock;
        if (clock.getElapsedTime().asSeconds() > 4) { // Ajouter une voiture toutes les 3 secondes
            cars.emplace_back(100.f, 360.f, 50.f, 30.f, sf::Vector2f(1.f, 0.f), 0.5f);
            cars.emplace_back(360.f, 100.f, 30.f, 50.f, sf::Vector2f(0.f, 1.f), 0.5f);
            clock.restart();
        }

        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
        window.draw(line3, 2, sf::Lines);
        window.draw(line4, 2, sf::Lines);

        circle1.setFillColor(get_SFML_color(traffic_lights["vauban"]));
        circle2.setFillColor(get_SFML_color(traffic_lights["solferino"]));
        circle3.setFillColor(get_SFML_color(traffic_lights["vauban"]));
        circle4.setFillColor(get_SFML_color(traffic_lights["solferino"]));

        window.draw(circle1);
        window.draw(circle2);
        window.draw(circle3);
        window.draw(circle4);

        rectangle1.setFillColor(get_SFML_color(crossings["top"]));
        rectangle2.setFillColor(get_SFML_color(crossings["right"]));
        rectangle3.setFillColor(get_SFML_color(crossings["bottom"]));
        rectangle4.setFillColor(get_SFML_color(crossings["left"]));
        

        window.draw(rectangle1);
        window.draw(rectangle2);
        window.draw(rectangle3);
        window.draw(rectangle4);

        
        window.display();
    }

    return 0;
}