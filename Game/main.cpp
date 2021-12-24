#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void initBird(sf::Sprite &heroSprite, sf::Texture &heroTexture)
{
    sf::Image heroImage;
    heroImage.loadFromFile("img/bird2.png");
    heroImage.createMaskFromColor(sf::Color(0x00, 0xFF, 0x00));

    heroTexture.loadFromImage(heroImage);
    heroTexture.setSmooth(true);

    heroSprite.setTexture(heroTexture);
    heroSprite.setPosition(50, 10);
}

float getDeltaTime(sf::Clock &clock)
{
    return clock.restart().asSeconds();
}

void onKeyDown(const sf::Event &event, sf::Sprite &heroSprite)
{
    // std::cout << "mouse x=" << event.x << ", y=" << event.y << std::endl;
    if (event.key.code == sf::Keyboard::Space)
    {
        heroSprite.move(0, -40);
    }
}

void update(sf::Sprite &heroSprite, float clock)
{
    heroSprite.move(0, clock * 100);
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEvents(sf::RenderWindow &window, sf::Sprite &heroSprite)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                onKeyDown(event, heroSprite);
                break;
        }
    }
}
// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, sf::Sprite &heroSprite, sf::Sprite &s_forest)
{
    window.clear();
    window.draw(s_forest);
    window.draw(heroSprite);
    window.display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "window",
                            sf::Style::Close | sf::Style::Titlebar);

    sf::Clock clock;
    // Скорость определим в переменой
    float speed = 100.f;
    // А в этой переменной мы определяем направление
    float way = 1.f;

    //отрисовка фона
    sf::Image background;
    background.loadFromFile("img/forest.png");
    sf::Texture forest;
    forest.loadFromImage(background);
    sf::Sprite s_forest;
    s_forest.setTexture(forest);

    sf::Sprite heroSprite;
    sf::Texture heroTexture;
    initBird(heroSprite, heroTexture);

    while (window.isOpen())
    {
        pollEvents(window, heroSprite);
        update(heroSprite, getDeltaTime(clock));
        redrawFrame(window, heroSprite, s_forest);
    }
    return EXIT_SUCCESS;
}