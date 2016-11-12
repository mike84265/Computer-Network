#include "GuessNum.h"
#include <stdexcept>
GuessNum::GuessNum(unsigned range, unsigned initcnt)
    :_range(range), _remainNum(0), _target(-1), _rnGen()
{}

void GuessNum::init(unsigned range, unsigned initcnt)
{
    _range = range;
    _remainNum = initcnt;
    _target = _rnGen(_range);
}

int GuessNum::compare(int input)
{
    if (input <= 0 || input > _range) {
        string str = "Input should be lie in 1 to " + to_string(_range);
        throw out_of_range(str);
        return INT_MAX;
    }
    --_remainNum;
    return input - _target;
}
