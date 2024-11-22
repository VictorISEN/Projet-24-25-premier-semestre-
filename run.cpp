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

    std::vector<Car> cars;
    cars.emplace_back(100.f, 360.f, 50.f, 30.f, sf::Vector2f(1.f, 0.f), 0.5f); // Voiture qui se déplace horizontalement
    cars.emplace_back(360.f, 100.f, 30.f, 50.f, sf::Vector2f(0.f, 1.f), 0.5f); // Voiture qui se déplace verticalement

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

        for (auto it = cars.begin(); it != cars.end();) {
            Car& car = *it;

            // Vérification si la voiture est proche ou dans le croisement
            bool near_crossing = (car.getX() > l1 - 50 && car.getX() < l2 + 50) ||
                (car.getY() > l1 - 50 && car.getY() < l2 + 50);

            bool in_crossing = (car.getX() > l1 && car.getX() < l2) ||
                (car.getY() > l1 && car.getY() < l2);

            if (!car.isInCrossing()) {
                // Si la voiture n'est pas encore dans le croisement
                if (near_crossing && !in_crossing) {
                    // Respecter les feux avant d'entrer dans le croisement
                    if ((car.getX() > l1 - 50 && car.getX() < l2 + 50 && traffic_lights["solferino"].get_traffic_color() == Traffic_color::red) ||
                        (car.getY() > l1 - 50 && car.getY() < l2 + 50 && traffic_lights["vauban"].get_traffic_color() == Traffic_color::red)) {
                        car.stop(); // Arrêt au feu rouge
                    }
                    else {
                        car.resume(0.5f); // Reprendre le mouvement si feu vert
                    }
                }
                else if (in_crossing) {
                    // Une fois que la voiture entre dans le croisement, elle continue
                    car.setInCrossing(true);
                    car.resume(0.5f);
                }
            }
            else {
                // Si la voiture est déjà dans le croisement, elle continue jusqu'à sortir complètement
                car.resume(0.5f);
            }

            car.move(); // Déplacer la voiture

            // Supprimer la voiture si elle sort de l'écran
            if (car.getX() > window.getSize().x || car.getY() > window.getSize().y) {
                it = cars.erase(it); // Supprime la voiture et passe à la suivante
            }
            else {
                window.draw(car.getShape()); // Dessiner la voiture dans la fenêtre
                ++it;
            }
        }

        static sf::Clock clock;
        if (clock.getElapsedTime().asSeconds() > 3) { // Ajouter une voiture toutes les 3 secondes
            cars.emplace_back(100.f, 360.f, 50.f, 30.f, sf::Vector2f(1.f, 0.f), 0.5f);
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