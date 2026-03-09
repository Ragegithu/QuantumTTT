#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800,800}),"QTTT");

    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({300.f, 200.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}