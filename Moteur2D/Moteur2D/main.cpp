#include <iostream>

#include <XkLogs.hpp>

#include "SFML/Graphics.hpp"
#include "Engine/AssetManager.hpp"

int main(int argc, char* argv[])
{

    XK_LOG_INIT();
    XK_INFO("TEST Info");
    XK_ERROR("TEST Error");
    XK_CRITICAL("TEST Critical");
    XK_WARN("TEST Warn");
    XK_SUCCESS("TEST Success");
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "PlaceHolder", sf::Style::Default, sf::State::Windowed, settings);

    xk::AssetManager<sf::Texture>::Load("ERROR", "");
    sf::RectangleShape rectangle;
    rectangle.setPosition({ 100, 100 });
    rectangle.setSize({ 200, 200 });
    rectangle.setTexture(&xk::AssetManager<sf::Texture>::Get("nimporte_quoi"));
    rectangle.setTextureRect(sf::IntRect({ 0,0 }, { 200,200 }));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        window.clear(sf::Color::White);
        window.draw(rectangle);
        window.display();
    }

    xk::AssetManager<sf::Texture>::unloadALL();
    XK_SUCCESS("Moteur arrete proprement.");
	return 0;
}