#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <array>
#include <iostream>
#include <algorithm>
#include <alloca.h>
#include "NeuralNetwork.hpp"
#include "Box.hpp"

#include <chrono>
#include <ctime>

template<size_t N, size_t... args>
class Population
{
public:
    Population()
    {
        for (auto& network: _population)
        {
            network.mutate(1.f, std::make_pair(-0.2f, 0.2f));
        }
    }
    void step()
    {
        for (size_t i = 0; i < N; i++)
        {
            Matrix<float, 4, 1> a = {{_boxes[i].getHeroPos().first, _boxes[i].getHeroPos().second,
                                     _boxes[i].getFoodPos().first, _boxes[i].getFoodPos().second}};
            auto mat = _population[i].feedForward(a);
            _boxes[i].moveHero(std::make_pair(mat[0][0], mat[0][1]));
        }
    }
    void nextGeneration(size_t n)
    {
        std::array<std::pair<size_t, size_t>, N> scores;
        for (size_t i = 0; i < N; i++)
        {
            scores[i] = std::make_pair(_boxes[i].getScore(), i);
            _boxes[i].reset();
        }
        std::sort(scores.rbegin(), scores.rend());

        size_t to = std::min(n, N);
        NeuralNetwork<args...>* win = static_cast<NeuralNetwork<args...>*>(alloca(to * sizeof(NeuralNetwork<args...>)));
        for (size_t i = 0; i < to; i++)
        {
            size_t index = scores[i].second;
            win[i] = _population[index];
        }
        for (size_t i = 0; i < N; i++)
        {
            _population[i] = win[i%to];
            _population[i].mutate(0.05f, std::make_pair(-0.1f, 0.1f));
        }
    }
    inline size_t getPopulationSize() const
    {
        return N;
    }
    inline std::array<NeuralNetwork<args...>, N>& getNetworks()
    {
        return _population;
    }
    inline std::array<Box, N>& getBoxes()
    {
        return _boxes;
    }

private:
    std::array<NeuralNetwork<args...>, N> _population;
    std::array<Box, N> _boxes;
};

#endif // POPULATION_HPP
