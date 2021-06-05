#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

#include "symbol.h"
#include "rulestriggered.h"
#include "rpn.h"

#include "tests.h"

int main()
{
    TestRPN();

    TestNew();

    TestAnd();
    TestAndWithNot();
    TestOr();
    TestOrWithAnd();

    return 0;
}
