#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <climits>
#include "util.h"
#include "rnGen.h"
#define INITCNT 5
#define RANGE 100
using namespace std;
class GuessNum
{
  public:
    GuessNum(unsigned range=RANGE, unsigned initcnt=INITCNT);
    void init(unsigned range=RANGE, unsigned initcnt=INITCNT);
    int compare(int);
    bool operator!() { return (_remainNum > 0); }
    const int getTarget() const { return _target; }
    const int getRemainNum() const { return _remainNum; }
    void clear() { _remainNum = 0; }
  private:
    int                 _range;
    int                 _remainNum;
    int                 _target;
    rnGen               _rnGen;
};
