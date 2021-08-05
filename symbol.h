#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

class Symbol
{
    public:
        enum STYPE {
            Operand, NotOperand, NotOperator, TrueValue, FalseValue, Or, And, Xor
        };

        void dump(std::stringstream& ss) const
        {
            switch (mType)
            {
                case Operand:
                    ss << "Operand " << GetRuleId();
                    break;
                case NotOperand:
                    ss << "Not Operand " << GetRuleId();
                    break;
                case NotOperator:
                    ss << "Not";
                    break;
                case TrueValue:
                    ss << "True";
                    break;
                case FalseValue:
                    ss << "False";
                    break;
                case Or:
                    ss << "Or";
                    break;
                case And:
                    ss << "And";
                    break;
                case Xor:
                    ss << "Xor";
                    break;
            }
        }
    
        std::string dump() const
        {
            std::stringstream ss;
            dump(ss);
            return ss.str();
        }

        Symbol(bool b)
        {
            mType = (b ? STYPE::TrueValue : STYPE::FalseValue);
        }

        Symbol(std::string val)
        {
            if (val == "|") {
                mType = STYPE::Or;
            } else if (val == "&") {
                mType = STYPE::And;
            } else if (val == "^") {
                mType = STYPE::Xor;
            } else if (val == "~") {
                mType = STYPE::NotOperator;
            } else if (val[0] == '!') {
                mType = STYPE::NotOperand;
                val.erase(0,1);
                SetRuleId(val);
            } else if (val.size() == 1 && val[0] == '1') {
                mType = STYPE::TrueValue;
            } else if (val.size() == 1 && val[0] == '0') {
                mType = STYPE::FalseValue;
            } else {
                mType = STYPE::Operand;
                SetRuleId(val);
            }
        }

        Symbol Evaluate(bool bOp1, bool bOp2)
        {
            switch(mType)
            {
                case STYPE::Or:
                    return Symbol(bOp1 || bOp2);
                case STYPE::And:
                    return Symbol(bOp1 && bOp2);
                case STYPE::Xor:
                    return Symbol(bOp1 ^ bOp2);
            }
            return Symbol(false);
        }

        Symbol Evaluate(bool bOp1)
        {
            switch(mType)
            {
                case STYPE::NotOperator:
                    return Symbol(!bOp1);
            }
            return Symbol(false);
        }

        bool IsOperator() const
        {
            if (mType == Or ||
                mType == And ||
                mType == Xor ||
                mType == NotOperator)
            {
                return true;
            }
            return false;
        }
        bool IsUnaryOperator() const
        {
            if (IsOperator() && mType == NotOperator)
            {
                return true;
            }
            return false;
        }
        bool IsOperand() const
        {
            if (mType == Operand ||
                mType == NotOperand)
            {
                return true;
            }
            return false;
        }

        bool IsNotOperand() const
        {
            return mType == STYPE::NotOperand;
        }

        bool IsBoolValue() const
        {
            if (mType == STYPE::TrueValue ||
                mType == STYPE::FalseValue)
            {
                return true;
            }
            return false;
        }

        bool GetBoolValue() const
        {
            return mType == STYPE::TrueValue ? true : false;
        }

        std::string GetRuleId() const
        {
            std::stringstream stream;
            stream << std::hex << mUVal;
            std::string str( stream.str() );
            std::transform(str.begin(), str.end(),str.begin(), ::toupper);

            return str;
        }

        void SetRuleId(const std::string& v)
        {
            mUVal = std::stoul(v, nullptr, 16);
        }

    STYPE mType;
    uint16_t mUVal;
};

