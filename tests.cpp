#include "rpn.h"

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
    ASSERT(expected == EvaluateRPNOLD(rulesTriggered, vec));
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
}

void TestAndWithNot()
{
    std::cout << __func__ << std::endl;
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
    ASSERT(Test("A|B",{{"A",true},{"B",true}},true));
    ASSERT(Test("A|B",{{"A",true},{"B",false}},true));
    ASSERT(Test("A|B",{{"A",false},{"B",true}},true));
    ASSERT(Test("A|B",{{"A",false},{"B",false}},false));
    ASSERT(Test("A|B|C",{{"A",true},{"B",true},{"C",true}},true));
    ASSERT(Test("A|B|C",{{"A",true},{"B",false},{"C",false}},true));
    ASSERT(Test("A|B|C",{{"A",false},{"B",true},{"C",false}},true));
    ASSERT(Test("A|B|C",{{"A",false},{"B",false},{"C",true}},true));
    ASSERT(Test("A|B|C",{{"A",false},{"B",false},{"C",false}},false));
}

void TestOrWithAnd()
{
    std::cout << __func__ << std::endl;
    ASSERT(Test("(A|B)&(A|B)",{{"A",true},{"B",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",true},{"B",true},{"C",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",true},{"B",false},{"C",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",false},{"B",true},{"C",true}},true));
    ASSERT(Test("(A|B)&(C|B)",{{"A",false},{"B",false},{"C",true}},false));

    std::cout << "SIZE IS " << sizeof(Symbol) << std::endl;
}
 
