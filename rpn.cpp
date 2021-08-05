#include <queue>
#include <stack>

#include "rpn.h"

using namespace std;

bool isOperator(string c)
{
    if (c == "&" || 
        c == "|" ||
        c == "^" ||
        c == "~" )
    {
        return true;
    }
    return false;
}

bool isRuleID(string c)
{
    return !isOperator(c) && c != "(" && c != ")";
}

void checkForNot(string& operand, bool& isNot)
{
    if (operand[0] == '!') {
        isNot = true;
        operand.erase(0,1);
    }
}

bool getOperand(map<string,bool>& operandValues, 
                string operand,
                bool isNot)
{
    bool result;

    if (operand == "1") {
        result = true;
    } else if (operand == "0") {
        result = false;
    } else {
        auto it = operandValues.find(operand);
        result = it->second;
    }

    if (isNot) {
        result = !result;
    }
cout << "operand " << operand << " is " << result << endl;
    return result;
}

void getOperands(map<string,bool>& operandValues, 
                 string operand1,
                 string operand2,
                 bool& op1,
                 bool& op2)
{
    bool op1Not = false;
    bool op2Not = false;

    checkForNot(operand1, op1Not);
    checkForNot(operand2, op2Not);
    
    op1 = getOperand(operandValues, operand1, op1Not);
    op2 = getOperand(operandValues, operand2, op2Not);
}

string evaluate( map<string,bool>& operandValues,
                 string operand1, 
                 string operand2,
                 string operat)
{
    bool op1;
    bool op2;
    bool res;

    getOperands(operandValues, operand1, operand2, op1, op2);
    cout << operand1 << " is " << op1 << " " << operand2 << " is " << op2 << endl;
    if (operat == "&") {
       res = op1 & op2;
    } else if (operat == "|") {
       res = op1 | op2; 
    } else if (operat == "^") {
       res = op1 ^ op2; 
    }

    return res ? "1" : "0";
}

bool getOperandValue(const RulesTriggered& rt, Symbol op)
{
    bool bOp;
    
    if (op.IsBoolValue()) {
        cout << "Bool value is " << op.GetBoolValue();
        bOp = op.GetBoolValue();
    } else {
        string v = op.GetRuleId();
        cout << "Rule id " << v << endl;

        bOp = rt.CheckTrigger(v);

        if (op.IsNotOperand()) {
            bOp = !bOp;
        }     
    }
    return bOp;
}

Symbol evaluate(const RulesTriggered& rt, Symbol op1, Symbol op2, Symbol oper)
{
    bool bOp1, bOp2;
    
    bOp1 = getOperandValue(rt, op1);
    bOp2 = getOperandValue(rt, op2);

    return oper.Evaluate(bOp1, bOp2);
}

Symbol evaluate(const RulesTriggered& rt, Symbol op1, Symbol oper)
{
    bool bOp1;
    
    bOp1 = getOperandValue(rt, op1);

    return oper.Evaluate(bOp1);
}

bool isRuleChar(char c)
{
    bool ret = false;

    if ((c == '!')
        ||
        (c >= '0' && c <='9')
        ||
        (c >= 'A' && c <='F')) {
        ret = true;
    }

    return ret;
}

void RPN( const string& rule,
          vector<Symbol>& rpn )
{
    std::queue<string> oq;
    std::stack<string> os;

    for (int i = 0; i < rule.size(); ++i) {
        string tok(1,rule[i]);
        if (isOperator(tok)) {
            os.push(tok);
        } else if (tok == "(") {
            os.push(tok); 
        } else if (tok == ")") {
            while( os.top() != "(" && !os.empty()) {
                oq.push(os.top());
                os.pop();
            }
            // pop ( bracket
            os.pop();
        } else {
            tok = ""; 
            while(i < rule.size() && isRuleChar(rule[i])) {
                tok.append(1, rule[i]);
                ++i;
            }
            --i;
            oq.push(tok); 
        }
    }

    while(!os.empty()) {
        oq.push(os.top());
        os.pop();
    }

    queue<string> copyQ = oq;

    while(!copyQ.empty()) {
        std::cout << copyQ.front() << " ";
        Symbol s = Symbol(copyQ.front());
        rpn.push_back(s);
        copyQ.pop();
    }
}

bool EvaluateRPN(const RulesTriggered& rt, 
                 vector<Symbol>& rpn)
{
    stack<Symbol> exec;
    // now using rpn
    for (auto x : rpn) {
        if (x.IsOperand()) {
            exec.push(x);
        } else if (x.IsUnaryOperator()) {
            Symbol op1 = exec.top();
            exec.pop();
            Symbol sres = evaluate(rt, op1, x);
            exec.push(sres);
        } else if (x.IsOperator()) {
            Symbol op1 = exec.top();
            exec.pop();
            Symbol op2 = exec.top();
            exec.pop();
            Symbol sres = evaluate(rt, op1, op2, x);
            exec.push(sres);
        }
    }
    cout << endl << "exec front (ie result) " << exec.top().GetBoolValue() << std::endl;
    return exec.top().GetBoolValue();
}
