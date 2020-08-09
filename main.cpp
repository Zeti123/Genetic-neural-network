#include <iostream>
#include "Population.hpp"
#include "Window.hpp"

#include <chrono>
#include <ctime>
#include <climits>
#include <fstream>
#include <optional>

using namespace std;

#define POPULATION_SIZE 48
#define TOPOLOGY 4, 3, 2 // neural network topology
static Population<POPULATION_SIZE, TOPOLOGY> population;
static Window window;

enum class Status
{
    Drawing60fps,
    Drawing120fps,
    DrawingUnlimited,
    NotDrawing,
    Pause
};

Window::Event windowActions(Status status, std::optional<size_t> selected)
{
    if (status != Status::NotDrawing)
    {
        switch(status)
        {
        case Status::Pause:
            window.setFrameRate(30); break;
        case Status::Drawing60fps:
            window.setFrameRate(60); break;
        case Status::Drawing120fps:
            window.setFrameRate(120); break;
        case Status::DrawingUnlimited:
            window.setFrameRate(UINT_MAX); break;
        case Status::NotDrawing: break;
        }
        window.clear();
        const auto& boxes = population.getBoxes();
        for (size_t i = 0; i < boxes.size(); i++)
        {
            int x = static_cast<int>((i%8) * boxes[i].getSize());
            int y = static_cast<int>((i/8) * boxes[i].getSize());
            window.drawBox(boxes[i], std::make_pair(x, y), selected == i);
        }
        window.display();
    }
    return window.pollEvent();
}

void populationActions(size_t steps, size_t nextGenSize)
{
    static size_t generation = 0;
    static size_t step = 0;
    if (step >= steps)
    {
        const auto& boxes = population.getBoxes();
        int bestScore = 0;
        for (const auto& box: boxes)
            bestScore = std::max(bestScore, box.getScore());
        std::cout<<"Generation number: "<<generation++<<" best score: "<<bestScore<<std::endl;
        population.nextGeneration(nextGenSize);
        step = 0;
    }
    population.step();
    step++;
}

std::optional<size_t> findSelectedBox()
{
    const auto& boxes = population.getBoxes();
    for (size_t i = 0; i < boxes.size(); i++)
    {
        int x = static_cast<int>((i%8) * boxes[i].getSize());
        int y = static_cast<int>((i/8) * boxes[i].getSize());
        int size = static_cast<int>(boxes[i].getSize());
        std::pair<int, int> mpos = window.getMousePosition();
        if ((mpos.first > x) && (mpos.second > y) && (mpos.first < x + size) && (mpos.second < y + size))
            return i;
    }
    return std::nullopt;
}

int main()
{
    Status status = Status::Drawing60fps;
    auto timer = std::chrono::system_clock::now();
    while(true)
    {
        if (status != Status::Pause)
            populationActions(1000, 15);
        if(status != Status::NotDrawing || (std::chrono::system_clock::now() - timer).count() > 1.f/30)
        {
            auto selectedBox = (status == Status::NotDrawing || status == Status::DrawingUnlimited)
                                ? std::nullopt : findSelectedBox();
            Window::Event event = windowActions(status, selectedBox);
            switch (event)
            {
            case Window::Event::close:
                window.close();
                return 0;
                break;
            case Window::Event::clickLoadButton:
                if (selectedBox != std::nullopt)
                {
                    std::string fileName;
                    std::cout<<"Load from file: ";
                    std::cin>>fileName;
                    try{
                        population.getNetworks()[selectedBox.value()].loadFromFile(fileName);
                    } catch(std::exception& ex) {
                        std::cout<<ex.what()<<std::endl;
                    }
                }
                break;
            case Window::Event::clickNextButton:
                if (status == Status::Pause)
                    status = Status::Drawing60fps;
                else
                    status = static_cast<Status>((static_cast<int>(status) + 1)%4);
                break;
            case Window::Event::clickSaveButton:
                if (selectedBox != std::nullopt)
                {
                    std::string fileName;
                    std::cout<<"Save to file: ";
                    std::cin>>fileName;
                    try{
                        population.getNetworks()[selectedBox.value()].saveToFile(fileName);
                    } catch(std::exception& ex) {
                        std::cout<<ex.what()<<std::endl;
                    }
                }
                break;
            case Window::Event::clickPauseButton:
                status = (status == Status::Pause) ? Status::Drawing60fps : Status::Pause;
                break;
            case Window::Event::none:
                break;
            }
        }
    }

    return 0;
}
