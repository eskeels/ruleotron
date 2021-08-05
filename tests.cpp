#include "rpn.h"
#include "tests.h"

void Assert(bool b, const char* file, int line)
{
    if (!b) {
        std::cout << "FAILED : " << file << " - " << line << std::endl;
        throw(1);
    }
    std::cout << "PASS" << std::endl;
}

#define ASSERT(b) Assert(b,__FILE__,__LINE__)

bool Test( std::string rule,
           std::map<std::string,bool> rulesTriggered,
           bool expected )
{
    std::vector<std::string> vec;
    std::vector<Symbol> rpn;
    RPN(rule, vec, rpn);
    RulesTriggered rulesT(rulesTriggered);
    return(expected == EvaluateRPN(rulesT, rpn));
}

bool TestRPN( std::string rule, std::vector<std::string> expected )
{
    std::vector<Symbol> rpn;
    RPN(rule, rpn);
    for (int i = 0; i < rpn.size(); ++i)
    {
        if(rpn[i].dump() != expected[i]){
            std::cout << rpn[i].dump() << "!=" << expected[i] << std::endl;
            return false;
        }
    }
    return rpn.size() == expected.size();
}

void TestRPN()
{
    ASSERT(TestRPN("AA&!BB", { "Operand AA",
                                   "Not Operand BB",
                                   "And" } ) );

    ASSERT(TestRPN("AA|BB", { "Operand AA",
                                  "Operand BB",
                                  "Or" } ) );

    ASSERT(TestRPN("(AA|BB)&(AA|CC)",
                                { "Operand AA",
                                  "Operand BB",
                                  "Or",
                                  "Operand AA",
                                  "Operand CC",
                                  "Or",
                                  "And"
                                 } ) );
}

void TestNew()
{
    std::vector<std::string> vec;
    std::vector<Symbol> rpn;
    ASSERT(Test("A&B",{{"A",true},{"B",true}},true));
}

void TestAnd()
{
    std::cout << __func__ << std::endl;
    ASSERT(Test("A&B",{{"A",true},{"B",true}},true));
    ASSERT(Test("A&B",{{"A",true},{"B",false}},false));
    ASSERT(Test("A&B",{{"A",false},{"B",true}},false));
    ASSERT(Test("A&B",{{"A",false},{"B",false}},false));
    ASSERT(Test("A&B&C&D&E",{{"A",false},
                             {"B",false},
                             {"C",false},
                             {"D",false},
                             {"E",false} },false));

    ASSERT(Test("A&B&C&D&E",{{"A",true},
                             {"B",true},
                             {"C",true},
                             {"D",true},
                             {"E",true} },true));

    ASSERT(Test("A&B&C&D&E",{{"A",false},
                             {"B",true},
                             {"C",true},
                             {"D",true},
                             {"E",true} },false));

    ASSERT(Test("A&B&C&D&E",{{"A",true},
                             {"B",true},
                             {"C",true},
                             {"D",true},
                             {"E",false} },false));

    ASSERT(Test("10&11",{{"10",true},{"11",false}}, false));
    ASSERT(Test("10&11",{{"10",true},{"11",true}}, true));

    ASSERT(Test("10&11&101&110&111",{{"10",true},
                                     {"11",true},
                                     {"101",true},
                                     {"110",true},
                                     {"111",true} }, true));

    ASSERT(Test("10&11&101&110&111",{{"10",true},
                                     {"11",true},
                                     {"101",false},
                                     {"110",true},
                                     {"111",true} }, false));
}

void TestAndWithOperandNot()
{
    std::cout << __func__ << std::endl;
    // Operand NOT
    ASSERT(Test("A&!B",{{"A",true},{"B",true}},false));
    ASSERT(Test("A&!B",{{"A",true},{"B",false}},true));
    ASSERT(Test("A&!B",{{"A",false},{"B",true}},false));
    ASSERT(Test("A&!B",{{"A",false},{"B",false}},false));

    ASSERT(Test("!A&B",{{"A",true},{"B",true}},false));
    ASSERT(Test("!A&B",{{"A",true},{"B",false}},false));
    ASSERT(Test("!A&B",{{"A",false},{"B",true}},true));
    ASSERT(Test("!A&B",{{"A",false},{"B",false}},false));

    ASSERT(Test("!A&!B",{{"A",true},{"B",true}},false));
    ASSERT(Test("!A&!B",{{"A",true},{"B",false}},false));
    ASSERT(Test("!A&!B",{{"A",false},{"B",true}},false));
    ASSERT(Test("!A&!B",{{"A",false},{"B",false}},true));
}

void TestOr()
{
    std::cout << __func__ << std::endl;
    ASSERT(Test("A|A",{{"A",true}},true));
    ASSERT(Test("A|B",{{"A",true},{"B",true}},true));
    ASSERT(Test("A|B",{{"A",true},{"B",false}},true));
    ASSERT(Test("A|B",{{"A",false},{"B",true}},true));
    ASSERT(Test("A|B",{{"A",false},{"B",false}},false));
    ASSERT(Test("A|B|C",{{"A",true},{"B",true},{"C",true}},true));
    ASSERT(Test("A|B|C",{{"A",true},{"B",false},{"C",false}},true));
    ASSERT(Test("A|B|C",{{"A",false},{"B",true},{"C",false}},true));
    ASSERT(Test("A|B|C",{{"A",false},{"B",false},{"C",true}},true));
    ASSERT(Test("A|B|C",{{"A",false},{"B",false},{"C",false}},false));
    ASSERT(Test("A|B|C|D|E|F",{{"A",false},
                               {"B",false},
                               {"C",false},
                               {"D",false},
                               {"E",false},
                               {"F",false}},false));

    ASSERT(Test("A|B|C|D|E|F",{{"A",true},
                               {"B",false},
                               {"C",false},
                               {"D",false},
                               {"E",false},
                               {"F",false}},true));

    ASSERT(Test("A|B|C|D|E|F",{{"A",false},
                               {"B",false},
                               {"C",false},
                               {"D",false},
                               {"E",false},
                               {"F",true}},true));

    ASSERT(Test("A|B|C|D|E|F",{{"A",true},
                               {"B",true},
                               {"C",true},
                               {"D",true},
                               {"E",true},
                               {"F",true}},true));
}

void TestOrWithOperandNot()
{
    std::cout << __func__ << std::endl;
    ASSERT(Test("A|!A",{{"A",true}},true));
    ASSERT(Test("A|!B",{{"A",true},{"B",true}},true));
    ASSERT(Test("A|!B",{{"A",true},{"B",false}},true));
    ASSERT(Test("!A|B",{{"A",false},{"B",true}},true));
    ASSERT(Test("!A|B",{{"A",false},{"B",false}},true));
    ASSERT(Test("!A|B|C",{{"A",true},{"B",true},{"C",true}},true));
    ASSERT(Test("A|!B|C",{{"A",true},{"B",false},{"C",false}},true));
    ASSERT(Test("A|B|!C",{{"A",false},{"B",true},{"C",false}},true));
    ASSERT(Test("!A|!B|!C",{{"A",false},{"B",false},{"C",true}},true));
    ASSERT(Test("!A|!B|!C",{{"A",true},{"B",true},{"C",true}},false));
    ASSERT(Test("A|B|!C",{{"A",false},{"B",false},{"C",false}},true));
    ASSERT(Test("!A|B|C|D|E|F",{{"A",false},
                               {"B",false},
                               {"C",false},
                               {"D",false},
                               {"E",false},
                               {"F",false}},true));

    ASSERT(Test("A|B|C|D|E|!F",{{"A",false},
                               {"B",false},
                               {"C",false},
                               {"D",false},
                               {"E",false},
                               {"F",false}},true));

    ASSERT(Test("!A|!B|!C|!D|!E|!F",{{"A",false},
                               {"B",false},
                               {"C",false},
                               {"D",false},
                               {"E",false},
                               {"F",false}},true));

    ASSERT(Test("!A|!B|!C|!D|!E|!F",{{"A",true},
                               {"B",true},
                               {"C",true},
                               {"D",true},
                               {"E",true},
                               {"F",true}},false));
}

void TestOrWithAnd()
{
    std::cout << __func__ << std::endl;
    ASSERT(Test("(A|B)&(A|B)",{{"A",true},{"B",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",true},{"B",true},{"C",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",true},{"B",false},{"C",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",false},{"B",true},{"C",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",false},{"B",false},{"C",true}},false));
}

void TestUnaryNot()
{
    ASSERT(Test("~(A&B)",{{"A",true},{"B",true}},false));
    ASSERT(Test("~(A&B)",{{"A",false},{"B",true}},true));
    ASSERT(Test("~(A&B)",{{"A",true},{"B",false}},true));
    ASSERT(Test("~(A&B)",{{"A",false},{"B",false}},true));

    ASSERT(Test("~(A|B)",{{"A",true},{"B",true}},false));
    ASSERT(Test("~(A|B)",{{"A",false},{"B",true}},false));
    ASSERT(Test("~(A|B)",{{"A",true},{"B",false}},false));
    ASSERT(Test("~(A|B)",{{"A",false},{"B",false}},true));

    ASSERT(Test("~((A|B)|C)",{{"A",true},{"B",true},{"C",true}},false));
    ASSERT(Test("~((A|B)&C)",{{"A",true},{"B",true},{"C",true}},false));
    ASSERT(Test("~((A|B)&C)",{{"A",true},{"B",true},{"C",false}},true));
    // No operator precedense so () needed after ~
    ASSERT(Test("(~(A&B))|(~(C&D))",{{"A",true},
                                     {"B",true},
                                     {"C",false},
                                     {"D",false}},
                                     true));
    // Not really the intended use but works
    ASSERT(Test("~A|~B",{{"A",true},{"B",true}},false));
}
