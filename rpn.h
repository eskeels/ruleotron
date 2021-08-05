#pragma once

#include <string>
#include <vector>
#include <map>

#include "symbol.h"
#include "rulestriggered.h"

void RPN( const std::string& rule, std::vector<Symbol>& rpn );

bool EvaluateRPN(const RulesTriggered& rt, 
                 std::vector<Symbol>& rpn);

