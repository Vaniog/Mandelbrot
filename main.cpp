#include <iostream>
#include <SFML/Graphics.hpp>

class MandelbrotFractal
{
public:
    MandelbrotFractal(int width, int height, int windowWidth, int windowHeight, std::string gradientPlace)
    {
        widthInPixels = width, heightInPixels = height;


        zeroCoordsInPixels = {widthInPixels / 2, heightInPixels / 2};
        scale = (long double)4 / widthInPixels;
        windowScale = (long double)windowWidth / width;
        mandImage.create(widthInPixels, heightInPixels, sf::Color::White);
        mandSprite.setPosition(0, 0);

        sf::Image gradientImage;
        gradientImage.loadFromFile(gradientPlace);
        int i;
        for (i = 0; i < 100; i++)
            colors[i] = gradientImage.getPixel(i, 0);
    }


    void fillMandImage(long double startX, long double startY, long double endX, long double endY)
    {
        int pointX, pointY, steps;
        scale = (endX - startX) / widthInPixels;
        zeroCoordsInPixels.first = 0 - startX / scale;
        zeroCoordsInPixels.second = 0 - startY / scale;
        sX = startX, sY = startY, eX = endX, eY = endY;
        mandImage.create(widthInPixels, heightInPixels, sf::Color(255, 255, 255));


        for (pointX = 0; pointX < widthInPixels; pointX++)
            for (pointY = 0; pointY < heightInPixels; pointY++)
            {
                long double c = (pointX - zeroCoordsInPixels.first) * scale;
                long double k = (pointY - zeroCoordsInPixels.second) * scale;
                steps = checkPoint(0, 0, c, k, 0);
                if (steps == -1)
                    mandImage.setPixel(pointX, pointY, sf::Color(0, 0, 0));
                else
                    mandImage.setPixel(pointX, pointY, colors[steps]);


            }
        mandTexture.loadFromImage(mandImage);
        mandSprite.setTexture(mandTexture);
    }

    void display(sf::RenderWindow *window, int x, int y, long double rastrScale)
    {
        mandSprite.setPosition(x, y);
        mandSprite.setScale(windowScale * rastrScale, windowScale * rastrScale);
        window->draw(mandSprite);
    }


private:
    int widthInPixels, heightInPixels;
    std::pair <int, int> zeroCoordsInPixels;
    int step;
    long double scale, windowScale, sX, sY, eX, eY;
    sf::Color colors[100];

    sf::Texture mandTexture;
    sf::Sprite mandSprite;
    sf::Image mandImage;

    int checkPoint(long double zc, long double zk, long double c, long double k, int stepsBefore) // zc * 1 + zk * i + (c * 1 + k * i)^2
    {
        if (zc * zc + zk * zk > 4)
        {
            return stepsBefore;
        }
        if (stepsBefore == 100)
            return -1;
        long double zcNew = zc * zc  - zk * zk + c;
        zk = 2 * zc * zk + k;
        return checkPoint(zcNew, zk, c, k, stepsBefore + 1);
    }
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(1920, 1080), " ", sf::Style::Fullscreen);
    MandelbrotFractal mand(540, 540, 1080, 1080, "resources//MandelbrotGradient5.png");
    mand.fillMandImage(-2, -2, 2, 2);

    int startWidth = 1080, startHeight = 1080;
    long double zoom = 1, rastrZoom;
    long double startX = -2, startY = -2;
    bool update = false;

    sf::Text zoomValue;
    zoomValue.setPosition(20, 20);
    zoomValue.setColor(sf::Color::White);
    sf::Font font;
    font.loadFromFile("resources//mainFont1.ttf");
    zoomValue.setFont(font);
    while(window.isOpen())
    {
        sf::Event event;
        update = false;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                window.close();
        }
        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            update = true, startX -= zoom * 0.2 ;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            update = true, startX += zoom * 0.2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            update = true, startY -= zoom * 0.2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            update = true, startY += zoom * 0.2;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int mouseX = sf::Mouse::getPosition().x;
            int mouseY = sf::Mouse::getPosition().y;

            int changeXInPixels = mouseX - 1920 / 2;
            int changeYInPixels = mouseY - 1080 / 2;
            startX += (long double)changeXInPixels * zoom * 4 / startWidth;
            startY += (long double)changeYInPixels * zoom * 4 / startHeight;
            update = true;
            while(sf::Mouse::isButtonPressed(sf::Mouse::Left));
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (double)1 / zoom < 32000000)
        {
            startX += zoom, startY += zoom;
            update = true, zoom /= 2;
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){};
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            startX -= zoom * 2, startY -= zoom * 2;
            update = true, zoom *= 2;
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){};
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            startX = -2, startY = -2;
            zoom = 1;
            update = true;
        }

        if (update == true)
        {
            mand.fillMandImage(startX, startY, startX + zoom * 4, startY + zoom * 4);
        }
        mand.display(&window, 420, 1, 1);
        zoomValue.setString(std::to_string((int)(1 / zoom)));
        window.draw(zoomValue);
        window.display();
        //window.draw(line);
        //window.draw(line2);
;    }
}
