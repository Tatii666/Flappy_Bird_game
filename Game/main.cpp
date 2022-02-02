#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

bool isSpacePressed = false;
bool isCollision = false;
bool isFinish = false;
int score = 0;
int level = 1;
int limLvl1 = 10;
int limLvl2 = 20;
int limLvl3 = 30;
int limLvl4 = 400;
int pipeMovementSpeed = 250;

double vy_heroSprite = 0;
double fallingAcceleration = 500; //ускорение свободного падения
sf::Vector2f targetSize(70.0f, 60.0f); //целевой размер
sf::Vector2f targetSizePipe(100.0f, 500.0f); //целевой размер

int Rand(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

void initBird(sf::Sprite &heroSprite, sf::Texture &heroTexture)
{
    sf::Image heroImage;
    heroImage.loadFromFile("img/bird23.png");
    heroImage.createMaskFromColor(sf::Color(0x00, 0xFF, 0x00));

    heroTexture.loadFromImage(heroImage);
    heroTexture.setSmooth(true);

    heroSprite.setTexture(heroTexture);
    heroSprite.setPosition(50, 250);

    heroSprite.setOrigin(
            heroSprite.getLocalBounds().width / 2,
            heroSprite.getLocalBounds().height / 2);

    heroSprite.setScale(
            targetSize.x / heroSprite.getLocalBounds().width,
            targetSize.y / heroSprite.getLocalBounds().height);
}

void initPipePositions(std::vector<sf::Sprite*> pipesArray)
{
    for(int i = 0; i < pipesArray.size(); i++)
    {
        if(i % 2 == 0)
        {
            float randomY = Rand(WINDOW_HEIGHT - 400, WINDOW_HEIGHT - 150);
            pipesArray[i]->setPosition(800 + 200 * (i / 2), randomY);
        }
        else
        {
            pipesArray[i]->setPosition(800 + 200 * (i / 2), -1000);
        }
    }
}
void initPipe(std::vector<sf::Sprite*> pipesArray, sf::Texture &bottomPipeTexture, sf::Texture &topPipeTexture)
{
    sf::Image bottomPipeImage;
    sf::Image topPipeImage;

    bottomPipeImage.loadFromFile("img/pipe3.png");
    topPipeImage.loadFromFile("img/pipe4.png");

    bottomPipeTexture.loadFromImage(bottomPipeImage);
    topPipeTexture.loadFromImage(topPipeImage);

    for(int i = 0; i < pipesArray.size(); i++)
    {
        if(i % 2 == 0)
        {
            pipesArray[i]->setTexture(bottomPipeTexture);
            pipesArray[i]->setScale(
                    targetSizePipe.x / pipesArray[i]->getLocalBounds().width,
                    targetSizePipe.y / pipesArray[i]->getLocalBounds().height);
        }
        else
        {
            pipesArray[i]->setTexture(topPipeTexture);
            pipesArray[i]->setScale(
                    targetSizePipe.x / pipesArray[i]->getLocalBounds().width,
                    targetSizePipe.y / pipesArray[i]->getLocalBounds().height);
        }
    }
    initPipePositions(pipesArray);
}

double getDeltaTime(sf::Clock &clock)
{
    return clock.restart().asSeconds();
}

void onKeyDown(const sf::Event &event, sf::Sprite &heroSprite, sf::Sound &wings, std::vector<sf::Sprite*> &pipesArray, sf::Clock &clock)
{
    if ((event.key.code == sf::Keyboard::Space) && !isSpacePressed && !isCollision)
    {
        vy_heroSprite = -200;
        isSpacePressed = true;
        wings.play();
        wings.setVolume(20);
    }

    if((event.key.code == sf::Keyboard::Space) && isCollision)
    {
        isCollision = false;
        //обнуление переменных
        heroSprite.setPosition(50, 250);
        heroSprite.setRotation(-20.f);
        vy_heroSprite = 0;
        initPipePositions(pipesArray);
        getDeltaTime(clock);
        level = 1;
        score = 0;
    }
}
void onKeyUp(const sf::Event &event, sf::Sprite &heroSprite)
{
    if (event.key.code == sf::Keyboard::Space)
    {
        isSpacePressed = false;
    }
}

bool windowCollision(sf::Sprite &heroSprite)
{
    bool isWindowCollision = false;
    sf::Vector2f position = heroSprite.getPosition();
    if ((position.y - (targetSize.y / 2) <= 0) || (position.y + (targetSize.y / 2) >= WINDOW_HEIGHT))
    {
        isWindowCollision = true;
    }
    return isWindowCollision;
}

bool pipeCollision(sf::Sprite &heroSprite, sf::Sprite &pipeSprite)
{
    bool isPipeCollision = false;

    sf::Vector2f positionPipe = pipeSprite.getPosition();
    sf::Vector2f positionHero = heroSprite.getPosition();
    if ((positionHero.x + (targetSize.x / 2) >= positionPipe.x) && //правая грань птички и левая трубы
       (positionHero.y + (targetSize.y / 2) >= positionPipe.y) && //нижняя грань птички и верхняя трубы
        (positionHero.x - (targetSize.x / 2) <= positionPipe.x + targetSizePipe.x) && //левая грань прички и правая трубы
        (positionHero.y - (targetSize.y / 2) <= positionPipe.y + targetSizePipe.y)) //верхняя грань птички и нижняя трубы
    {
        isPipeCollision = true;
    }

    return isPipeCollision;
}

void gameOverScreen(sf::RenderWindow &window)
{
    sf::Texture gameOverTexture, backgroundTexture;
    gameOverTexture.loadFromFile("img/texture1.jpg");
    backgroundTexture.loadFromFile("img/backgroundMenu.jpg");
    sf::Sprite gameOver(gameOverTexture), background(backgroundTexture);

    sf::RectangleShape scoreWindow(sf::Vector2f(400, 50));
    scoreWindow.setPosition(WINDOW_WIDTH / 2 - 190, 350);
    scoreWindow.setFillColor(sf::Color(0x2A, 0xB1, 0xEC));

    sf::Font font;
    font.loadFromFile("PermanentMarker-Regular.ttf");
    sf::Text scoreResult;
    scoreResult.setFont(font);
    scoreResult.setString("Your score is: "+ std::to_string(score));
    scoreResult.setCharacterSize(40); // in pixels, not points!
    scoreResult.setFillColor(sf::Color(0x1A, 0x1A, 0x1A));
    scoreResult.setStyle(sf::Text::Bold);
    scoreResult.move({WINDOW_WIDTH / 2 - 180, 350});

    gameOver.setPosition(100, 100);
    background.setPosition(0, 0);
    background.setScale(WINDOW_WIDTH, WINDOW_HEIGHT);

    window.draw(background);
    window.draw(gameOver);
    window.draw(scoreWindow);
    window.draw(scoreResult);
}

void winGameScreen(sf::RenderWindow &window)
{
    sf::Texture winnerGameTexture, backgroundTexture;
    winnerGameTexture.loadFromFile("img/winner.jpg");
    backgroundTexture.loadFromFile("img/backgroundWin.jpg");
    sf::Sprite winGame(winnerGameTexture), background(backgroundTexture);

    winGame.setPosition(80, 0);
    background.setPosition(0, 0);
    background.setScale(WINDOW_WIDTH, WINDOW_HEIGHT);

    window.draw(background);
    window.draw(winGame);
}

void textScore(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("PermanentMarker-Regular.ttf");
    sf::Text scoreText("Score: "+ std::to_string(score), font);
    scoreText.setPosition(WINDOW_WIDTH/2 - scoreText.getLocalBounds().width/2, 5);
    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color(0x0, 0x0, 0x0));
    scoreText.setStyle(sf::Text::Bold);

    window.draw(scoreText);
}

void currentLevel(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("PermanentMarker-Regular.ttf");
    sf::Text levelText("Level: "+ std::to_string(level), font);
    levelText.setPosition(WINDOW_WIDTH - 150, 5);
    levelText.setFont(font);
    levelText.setCharacterSize(32);
    levelText.setFillColor(sf::Color(0xb6, 0xea, 0xfd));
    levelText.setStyle(sf::Text::Bold);

    window.draw(levelText);
}


void update(sf::Sprite &heroSprite, double clock)
{
    heroSprite.setRotation((vy_heroSprite / 7.f) - 20.f);

    double distanceHeroSprite = (clock * vy_heroSprite) + (fallingAcceleration * clock * clock) / 2;  //(g*t^2)/2
    heroSprite.move(0, distanceHeroSprite);
    vy_heroSprite = vy_heroSprite + fallingAcceleration * clock;

    if (windowCollision(heroSprite))
    {
        isCollision = true;
    }
}

void updatePipe(std::vector <sf::Sprite*> pipesArray, sf::Sprite &heroSprite, double clock)
{
    double movement = pipeMovementSpeed * clock;

    for(int i = 0; i < pipesArray.size(); i++)
    {
        pipesArray[i]->move(-movement, 0);
        if (pipeCollision(heroSprite, *pipesArray[i]))
        {
            isCollision = true;
        }
    }

    for(int i = 0; i < pipesArray.size(); i++)
    {
        if(pipesArray[i]->getPosition().x + 100 <= 0)
        {
            if(pipesArray[i]->getPosition().y != 1000 && pipesArray[i]->getPosition().y != -1000)
            {
                score++;
            }
            if(score >= limLvl1)
            {
                level = 2;
            }
            if(score >= limLvl2)
            {
                level = 3;
            }
            if(score >= limLvl3)
            {
                level = 4;
            }
            if(score >= limLvl4)
            {
                level = 5;
            }
            float randomY = Rand(WINDOW_HEIGHT - 400, WINDOW_HEIGHT - 150);

            switch (level)
            {
                case 1:
                    if(i % 2 == 0)
                    {
                        pipesArray[i]->setPosition(800, randomY);
                    }
                    else
                    {
                        pipesArray[i]->setPosition(800, -1000);
                    }

                    break;
                case 2:
                    if(i % 2 == 0)
                    {
                        pipesArray[i]->setPosition(800, 1000);
                    }
                    else
                    {
                        pipesArray[i]->setPosition(800, randomY - 100 - targetSizePipe.y);
                    }
                    break;
                case 3:
                    if(i % 2 == 0)
                    {
                        pipesArray[i]->setPosition(800, randomY);
                        pipesArray[i+1]->setPosition(800, randomY - 300 - targetSizePipe.y);
                        i++;
                    }
                    else
                    {
                        pipesArray[i]->setPosition(800, randomY - 300 - targetSizePipe.y);
                        pipesArray[i-1]->setPosition(800, randomY);
                    }
                    break;
                case 4:
                    if(i % 2 == 0)
                    {
                        pipesArray[i]->setPosition(800, randomY);
                        pipesArray[i+1]->setPosition(800, randomY - 200 - targetSizePipe.y);
                        i++;
                    }
                    else
                    {
                        pipesArray[i]->setPosition(800, randomY - 200 - targetSizePipe.y);
                        pipesArray[i-1]->setPosition(800, randomY);
                    }
                    break;
                case 5:
                    isFinish = true;
                default:
                    if(i % 2 == 0)
                    {
                        pipesArray[i]->setPosition(800, randomY);
                    }
                    else
                    {
                        pipesArray[i]->setPosition(800, -1000);
                    }
            }
        }
    }
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEvents(sf::RenderWindow &window, sf::Sprite &heroSprite, sf::Sound &wings, std::vector<sf::Sprite*> &pipesArray, sf::Clock &clock)
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
                onKeyDown(event, heroSprite, wings, pipesArray, clock);
                break;
            case sf::Event::KeyReleased:
                onKeyUp(event, heroSprite);
                break;
        }
    }
}
// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, sf::Sprite &heroSprite, sf::Sprite &s_forest, std::vector<sf::Sprite*> &pipesArray)
{
    window.clear();
    window.draw(s_forest);
    window.draw(heroSprite);
    textScore(window);
    currentLevel(window);
    for(int i = 0; i < pipesArray.size(); i++)
    {
        sf::Sprite pipeSprite = *pipesArray[i];
        window.draw(pipeSprite);
    }
    if (isCollision)
    {
        gameOverScreen(window);
    }
    if(isFinish)
    {
        winGameScreen(window);
    }
    window.display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "window",
                            sf::Style::Close | sf::Style::Titlebar);

    sf::Clock clock;

    sf::SoundBuffer wingsBuffer;//создаём буфер для звука
    wingsBuffer.loadFromFile("sound/wings.ogg");//загружаем в него звук
    sf::Sound wings(wingsBuffer);//создаем звук и загружаем в него звук из буфера

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

    sf::Sprite bottomPipeSprite1, bottomPipeSprite2, bottomPipeSprite3, bottomPipeSprite4, topPipeSprite1, topPipeSprite2, topPipeSprite3, topPipeSprite4;
    sf::Texture bottomPipeTexture;
    sf::Texture topPipeTexture;
    std::vector <sf::Sprite*> pipesArray = {&bottomPipeSprite1, &topPipeSprite1, &bottomPipeSprite2, &topPipeSprite2, &bottomPipeSprite3, &topPipeSprite3, &bottomPipeSprite4, &topPipeSprite4};

    initPipe(pipesArray, bottomPipeTexture, topPipeTexture);

    while (window.isOpen())
    {
        pollEvents(window, heroSprite, wings, pipesArray, clock);
        if(!isCollision)
        {
            double deltaTime = getDeltaTime(clock);
            update(heroSprite, deltaTime);
            updatePipe(pipesArray, heroSprite, deltaTime);
        }
        redrawFrame(window, heroSprite, s_forest, pipesArray);
    }
    return 0;
}
