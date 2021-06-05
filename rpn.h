#pragma once

#include <string>
#include <vector>
#include <map>

#include "symbol.h"
#include "rulestriggered.h"

void RPN( const std::string& rule, std::vector<Symbol>& rpn );

void RPN(const std::string& rule,
         std::vector<std::string>& vec,
         std::vector<Symbol>& rpn);

bool EvaluateRPNOLD(std::map<std::string,bool> values,
                    std::vector<std::string>& vec);

bool EvaluateRPN(const RulesTriggered& rt, 
                 std::vector<Symbol>& rpn);


