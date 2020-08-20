#include "Window.hpp"

Window::Window()
    :_window(sf::VideoMode(_windowSize.first, _windowSize.second), "NeuralNetworks")
{
    _window.setFramerateLimit(60);
}

void Window::clear()
{
    _window.clear();
}

void Window::display()
{
    _window.display();
}

void Window::setFrameRate(unsigned int frameRate)
{
    _window.setFramerateLimit(frameRate);
}

void Window::drawBox(const Box& box, std::pair<int, int> position, bool dragged)
{
    sf::RectangleShape boxBorder(sf::Vector2f(box.getSize(), box.getSize()));
    boxBorder.setPosition(sf::Vector2f(position.first, position.second));
    boxBorder.setFillColor(dragged ? sf::Color(36, 37, 38) : sf::Color::Black);
    boxBorder.setOutlineThickness(2);
    boxBorder.setOutlineColor(sf::Color::White);

    sf::CircleShape hero(box.getHeroSize());
    hero.setFillColor(sf::Color::Blue);
    hero.setOrigin(box.getHeroSize(), box.getHeroSize());
    hero.setPosition(box.getHeroPos().first  + position.first, box.getHeroPos().second  + position.second);

    sf::CircleShape food(box.getFoodSize());
    food.setFillColor(sf::Color::Green);
    food.setOrigin(box.getFoodSize(), box.getFoodSize());
    food.setPosition(box.getFoodPos().first  + position.first, box.getFoodPos().second + position.second);

    _window.draw(boxBorder);
    _window.draw(food);
    _window.draw(hero);
}

Window::Event Window::pollEvent()
{
    sf::Event event;
    while (_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            return Event::Close;
        if (event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::P)
                return Event::ClickPauseButton;
            if(event.key.code == sf::Keyboard::N)
                return Event::ClickNextButton;
            if(event.key.code == sf::Keyboard::S)
                return Event::ClickSaveButton;
            if(event.key.code == sf::Keyboard::L)
                return Event::ClickLoadButton;
        }
    }
    return Event::None;
}

std::pair<int, int> Window::getMousePosition()
{
    sf::Mouse mouse;
    return std::make_pair(mouse.getPosition(_window).x, mouse.getPosition(_window).y);
}

void Window::close()
{
    _window.close();
}
