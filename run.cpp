// condition_variable::wait (with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include "lights.hpp"
#include "pedestrian.hpp"

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    std::stop_source stopping;
    std::map<std::string, Traffic_light> traffic_lights;
    std::map<std::string, Crossing> crossings;

    float l1 = 300, l2 = 500, size = 800, radius = 25;

    traffic_lights["solferino"] = Traffic_light(Traffic_color::green, 325, 275);
    traffic_lights["vauban"] = Traffic_light(Traffic_color::red, 275, 425);

    crossings["top"] = Crossing(Crossing_color::red, 300, 200);
    crossings["bottom"] = Crossing(Crossing_color::red, 300, 600);
    crossings["left"] = Crossing(Crossing_color::red, 600, 300);
    crossings["right"] = Crossing(Crossing_color::red, 200, 300);

    std::vector<pedestrian> pedestrians;

    std::jthread thread_traffic_light(run_traffic_light,
        std::ref(traffic_lights), std::ref(crossings), stopping.get_token());

    std::jthread write_traffic_light(print_traffic_light,
        std::ref(traffic_lights), std::ref(crossings), stopping.get_token());

    std::jthread thread_pedestrian(run_pedestrians,
        std::ref(pedestrians), std::ref(crossings), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");

    sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(0, l1)), sf::Vertex(sf::Vector2f(size, l1)) };
    sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(0, l2)), sf::Vertex(sf::Vector2f(size, l2)) };
    sf::Vertex line3[] = { sf::Vertex(sf::Vector2f(l1, 0)), sf::Vertex(sf::Vector2f(l1, size)) };
    sf::Vertex line4[] = { sf::Vertex(sf::Vector2f(l2, 0)), sf::Vertex(sf::Vector2f(l2, size)) };
    sf::Vertex centerH[] = { sf::Vertex(sf::Vector2f(0, size/2)), sf::Vertex(sf::Vector2f(size, size/2)) };
    sf::Vertex centerV[] = { sf::Vertex(sf::Vector2f(size / 2, 0)), sf::Vertex(sf::Vector2f(size/2, size)) };

    sf::CircleShape lightTop(radius);
    lightTop.setFillColor(sf::Color::Red);
    lightTop.setOrigin(lightTop.getRadius() / 2, lightTop.getRadius() / 2);
    lightTop.setPosition(traffic_lights["solferino"].getPosition_x(), traffic_lights["solferino"].getPosition_y());
    sf::CircleShape lightBottom(radius);
    lightBottom.setFillColor(sf::Color::Red);
    lightBottom.setOrigin(lightBottom.getRadius() / 2, lightBottom.getRadius() / 2);
    lightBottom.setPosition(traffic_lights["solferino"].getPosition_x() + 112, traffic_lights["solferino"].getPosition_y() + 225);
    sf::CircleShape lightLeft(radius);
    lightLeft.setFillColor(sf::Color::Red);
    lightLeft.setOrigin(lightLeft.getRadius() / 2, lightLeft.getRadius() / 2);
    lightLeft.setPosition(traffic_lights["vauban"].getPosition_x(), traffic_lights["vauban"].getPosition_y());
    sf::CircleShape lightRight(radius);
    lightRight.setFillColor(sf::Color::Red);
    lightRight.setOrigin(lightRight.getRadius() / 2, lightRight.getRadius() / 2);
    lightRight.setPosition(traffic_lights["vauban"].getPosition_x() + 225, traffic_lights["vauban"].getPosition_y() - 112);

    sf::RectangleShape CTop(sf::Vector2f(l2 - l1, 50.f));
    CTop.setFillColor(sf::Color::Red);
    CTop.setPosition(l1, l1 - 100);
    sf::RectangleShape CRight(sf::Vector2f(50.f, l2 - l1));
    CRight.setFillColor(sf::Color::Red);
    CRight.setPosition(l2 + 100, l1);
    sf::RectangleShape CBottom(sf::Vector2f(l2 - l1, 50.f));
    CBottom.setFillColor(sf::Color::Red);
    CBottom.setPosition(l1, l2 + 100);
    sf::RectangleShape CLeft(sf::Vector2f(50.f, l2 - l1));
    CLeft.setFillColor(sf::Color::Red);
    CLeft.setPosition(l1 - 100, l1);

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

        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
        window.draw(line3, 2, sf::Lines);
        window.draw(line4, 2, sf::Lines);
        window.draw(centerH, 2, sf::Lines);
        window.draw(centerV, 2, sf::Lines);

        lightTop.setFillColor(get_SFML_color(traffic_lights["vauban"]));
        lightBottom.setFillColor(get_SFML_color(traffic_lights["vauban"]));
        lightLeft.setFillColor(get_SFML_color(traffic_lights["solferino"]));
        lightRight.setFillColor(get_SFML_color(traffic_lights["solferino"]));

        window.draw(lightTop);
        window.draw(lightBottom);
        window.draw(lightLeft);
        window.draw(lightRight);

        CTop.setFillColor(get_SFML_color(crossings["top"]));
        CRight.setFillColor(get_SFML_color(crossings["right"]));
        CBottom.setFillColor(get_SFML_color(crossings["bottom"]));
        CLeft.setFillColor(get_SFML_color(crossings["left"]));

        window.draw(CTop);
        window.draw(CRight);
        window.draw(CBottom);
        window.draw(CLeft);

        for (int i = 0; i < pedestrians.size(); i++)
        {
            sf::RectangleShape rectangle(sf::Vector2f(25, 25));
            rectangle.setFillColor(sf::Color::Magenta);
            rectangle.setPosition(pedestrians.at(i).getPosition_x(), pedestrians.at(i).getPosition_y()); 
            window.draw(rectangle);
        }

        window.display();
    }

    return 0;
}