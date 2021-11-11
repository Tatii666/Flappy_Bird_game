#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>

using namespace sf;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "window",
                            sf::Style::Close | sf::Style::Titlebar);

    //отрисовка героя
    Image heroImage;
    heroImage.loadFromFile("00/img/bird2.png");
    heroImage.createMaskFromColor(Color(0x00, 0xFF, 0x00));

    Texture heroTexture;
    heroTexture.loadFromImage(heroImage);

    Sprite heroSprite;
    heroSprite.setTexture(heroTexture);
    heroSprite.setPosition(50, 200);

    //отрисовка фона
    Image background;
    background.loadFromFile("00/img/forest.png");
    Texture forest;
    forest.loadFromImage(background);
    Sprite s_forest;
    s_forest.setTexture(forest);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();

                break;
            }
        }
        window.clear();
        window.draw(s_forest);
        window.draw(heroSprite);

        window.display();
    }

    return EXIT_SUCCESS;
}