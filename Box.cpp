#include <random>
#include "Box.hpp"

Box::Box()
    : _heroPos(static_cast<size_t>(rand())%_boxSize, static_cast<size_t>(rand())%_boxSize),
      _foodPos(static_cast<size_t>(rand())%_boxSize, static_cast<size_t>(rand())%_boxSize), _score(0) {}

void Box::moveHero(std::pair<float, float> vec)
{
    _heroPos.first += vec.first * _heroSpeed;
    _heroPos.second += vec.second * _heroSpeed;
    _heroPos.first = std::min(static_cast<float>(_boxSize), std::max(_heroPos.first, 0.f));
    _heroPos.second = std::min(static_cast<float>(_boxSize), std::max(_heroPos.second, 0.f));

    if(((_rHero + _rFood)*(_rHero + _rFood)) >= std::abs((_heroPos.first - _foodPos.first) *
                                                         (_heroPos.first - _foodPos.first) +
                                                         (_heroPos.second - _foodPos.second) *
                                                         (_heroPos.second - _foodPos.second)))
    {
        _score++;
        _foodPos = std::make_pair(static_cast<size_t>(rand())%_boxSize, static_cast<size_t>(rand())%_boxSize);
    }
}

void Box::reset()
{
    _score = 0;
    _heroPos = std::make_pair(static_cast<size_t>(rand())%_boxSize, static_cast<size_t>(rand())%_boxSize);
    _foodPos = std::make_pair(static_cast<size_t>(rand())%_boxSize, static_cast<size_t>(rand())%_boxSize);
}
