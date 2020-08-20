#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include "Box.hpp"

class Window
{
public:
    enum class Event
    {
        None,
        Close,
        ClickPauseButton,
        ClickNextButton,
        ClickSaveButton,
        ClickLoadButton
    };

public:
    Window();
    void clear();
    void display();
    void setFrameRate(unsigned int frameRate);
    void drawBox(const Box& box, std::pair<int, int> position, bool dragged);
    Event pollEvent();
    std::pair<int, int> getMousePosition();
    void close();

private:
    static constexpr std::pair<size_t, size_t> _windowSize = {800, 600};

    sf::RenderWindow _window;
};

#endif // WINDOW_HPP
