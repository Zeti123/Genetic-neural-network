#ifndef BOX_HPP
#define BOX_HPP

#include <utility>
#include <stddef.h>

class Box
{
public:
    Box();
    void moveHero(std::pair<float, float> vec);
    void reset();

    inline std::pair<float, float> getHeroPos() const { return _heroPos; }
    inline std::pair<float, float> getFoodPos() const { return _foodPos; }
    inline int getScore() const { return _score; }
    inline size_t getSize() const { return _boxSize; }
    inline float getHeroSize() const { return _rHero; }
    inline float getFoodSize() const { return _rFood; }

private:
    static constexpr size_t _boxSize = 100;
    static constexpr float _rHero = 2.5;
    static constexpr float _rFood = 2.5;
    static constexpr float _heroSpeed = 5;

    std::pair<float, float> _heroPos;
    std::pair<float, float> _foodPos;
    int _score;
};

#endif // BOX_HPP
