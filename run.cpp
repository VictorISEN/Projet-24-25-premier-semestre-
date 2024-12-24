// condition_variable::wait (with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "lights.hpp"
#include "pedestrian.hpp"
#include "car.hpp"
#include "bus.hpp"
#include "bikes.hpp"


int main()
{
    srand(time(NULL));
    std::stop_source stopping;
    std::map<std::string, Traffic_light> traffic_lights;
    std::map<std::string, Crossing> crossings;
    std::vector<Car> cars;
    std::vector<Bus> buses;
    std::vector<Bike> bikes;

    // Charger la texture pour la carte
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("map.png")) {
        std::cerr << "Erreur : Impossible de charger map_texture.png" << std::endl;
        return -1;
    }

    // Créer un sprite à partir de la texture
    sf::Sprite mapSprite;
    mapSprite.setTexture(mapTexture);



    float l1 = 400, l2 = 600, height = 1000, width = 1920, radius = 25;

    traffic_lights["solferino"] = Traffic_light(Traffic_color::green, (l2 - l1) / 2 + l1, height/2);
    traffic_lights["vauban"] = Traffic_light(Traffic_color::red, (l2 - l1) / 2 + l1, height/2);

    crossings["top"] = Crossing(Crossing_color::red, (l2 - l1)/ 2 + l1 - 10, l1 - 15);
    crossings["bottom"] = Crossing(Crossing_color::red, (l2 - l1) / 2 + l1 - 10, l2 + 5);
    crossings["left"] = Crossing(Crossing_color::red, l1 - 15, height / 2 - 10);
    crossings["right"] = Crossing(Crossing_color::red, l2 + 5, height / 2 - 10);

    std::vector<pedestrian> pedestrians;

    std::jthread thread_traffic_light(run_traffic_light,
        std::ref(traffic_lights), std::ref(crossings), stopping.get_token());

    std::jthread write_traffic_light(print_traffic_light,
        std::ref(traffic_lights), std::ref(crossings), stopping.get_token());

    std::jthread thread_pedestrian(run_pedestrians,
        std::ref(pedestrians), std::ref(crossings), stopping.get_token());

    std::jthread thread_cars(run_cars,
        std::ref(cars), std::ref(crossings), std::ref(traffic_lights), std::ref(buses), std::ref(bikes), std::ref(pedestrians), stopping.get_token());

    std::jthread thread_buses(run_buses,
        std::ref(buses), std::ref(crossings), std::ref(traffic_lights), std::ref(cars), std::ref(bikes), std::ref(pedestrians), stopping.get_token());

    std::jthread thread_bikes(run_bikes,
        std::ref(bikes), std::ref(crossings), std::ref(traffic_lights), std::ref(buses), std::ref(cars), std::ref(pedestrians), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(width, height), "My window");

    sf::Vertex centerLeft[] = { sf::Vertex(sf::Vector2f(0, height / 2)), sf::Vertex(sf::Vector2f(l1, height / 2)) };
    sf::Vertex centerRight[] = { sf::Vertex(sf::Vector2f(l2, height / 2)), sf::Vertex(sf::Vector2f(width, height / 2)) };
    sf::Vertex centerTop[] = { sf::Vertex(sf::Vector2f((l2 - l1) / 2 + l1, 0)), sf::Vertex(sf::Vector2f((l2 - l1) / 2 + l1, l1)) };
    sf::Vertex centerBottom[] = { sf::Vertex(sf::Vector2f((l2 - l1) / 2 + l1, l2)), sf::Vertex(sf::Vector2f((l2 - l1) / 2 + l1, height)) };

    sf::Vertex topOut[] = { sf::Vertex(sf::Vector2f(0, l1)), sf::Vertex(sf::Vector2f(width, l1)) };
    sf::Vertex bottomOut[] = { sf::Vertex(sf::Vector2f(0, l2)), sf::Vertex(sf::Vector2f(width, l2)) };
    sf::Vertex leftOut[] = { sf::Vertex(sf::Vector2f(l1, 0)), sf::Vertex(sf::Vector2f(l1, height)) };
    sf::Vertex rightOut[] = { sf::Vertex(sf::Vector2f(l2, 0)), sf::Vertex(sf::Vector2f(l2, height)) };

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
       
        window.draw(mapSprite);

       

        window.draw(crossings["top"].getShape());
        window.draw(crossings["bottom"].getShape());
        window.draw(crossings["left"].getShape());
        window.draw(crossings["right"].getShape());

        sf::CircleShape tmp;
        tmp.setPosition(sf::Vector2f(l1 - traffic_lights["solferino"].getShape().getRadius(), l2 - 3 * traffic_lights["solferino"].getShape().getRadius()));
        tmp.setRadius(traffic_lights["solferino"].getShape().getRadius());
        tmp.setFillColor(get_SFML_color(traffic_lights["solferino"].get_traffic_color()));
        window.draw(tmp);
        tmp.setPosition(sf::Vector2f(l2 - traffic_lights["solferino"].getShape().getRadius(), l1 + traffic_lights["solferino"].getShape().getRadius()));
        tmp.setRadius(traffic_lights["solferino"].getShape().getRadius());
        tmp.setFillColor(get_SFML_color(traffic_lights["solferino"].get_traffic_color()));
        window.draw(tmp);
        tmp.setPosition(sf::Vector2f(l1 + traffic_lights["vauban"].getShape().getRadius(), l1 - traffic_lights["vauban"].getShape().getRadius()));
        tmp.setRadius(traffic_lights["vauban"].getShape().getRadius());
        tmp.setFillColor(get_SFML_color(traffic_lights["vauban"].get_traffic_color()));
        window.draw(tmp);
        tmp.setPosition(sf::Vector2f(l2 - 3 * traffic_lights["vauban"].getShape().getRadius(), l2 - traffic_lights["vauban"].getShape().getRadius()));
        tmp.setRadius(traffic_lights["vauban"].getShape().getRadius());
        tmp.setFillColor(get_SFML_color(traffic_lights["vauban"].get_traffic_color()));
        window.draw(tmp);
        tmp.setPosition(sf::Vector2f(l2 + 500 + traffic_lights["solferino"].getShape().getRadius(), l2 - 3 * traffic_lights["solferino"].getShape().getRadius()));
        tmp.setRadius(traffic_lights["solferino"].getShape().getRadius());
        tmp.setFillColor(get_SFML_color(traffic_lights["solferino"].get_traffic_color()));
        window.draw(tmp);
        tmp.setPosition(sf::Vector2f(l2 + 600 + traffic_lights["solferino"].getShape().getRadius(), l1 + traffic_lights["solferino"].getShape().getRadius()));
        tmp.setRadius(traffic_lights["solferino"].getShape().getRadius());
        tmp.setFillColor(get_SFML_color(traffic_lights["solferino"].get_traffic_color()));
        window.draw(tmp);

        for (int i = 0; i < pedestrians.size(); i++)
        {
            window.draw(pedestrians.at(i).getShape());
        }
        for (int i = 0; i < cars.size(); i++)
        {
            window.draw(cars.at(i).getShape());
        }
        for (int i = 0; i < buses.size(); i++)
        {
            window.draw(buses.at(i).getShape());
        }
        for (int i = 0; i < bikes.size(); i++)
        {
            window.draw(bikes.at(i).getShape());
        }

        window.display();
    }

    return 0;
}