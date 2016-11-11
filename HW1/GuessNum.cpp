#include "GuessNum.h"
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
    if (_remainNum <= 0)
        return REMAIN_ZERO;
    if (input <= 0 || input > _range)
        return OUT_OF_BOUND;
    --_remainNum;
    return input - _target;
}
