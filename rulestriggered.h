#pragma once

#include <string>
#include <unordered_map>

class RulesTriggered
{
    public:
        RulesTriggered(){};
        RulesTriggered(const std::map<std::string,bool> rulesAndTriggers)
        {
            for (auto& p : rulesAndTriggers) {
                SetTrigger(p.first, p.second);
            }    
        }
        void SetTrigger(const std::string& rule, bool v)
        {
            mTriggers.insert({rule, v}); 
        }

        void SetTrigger(const std::string& rule)
        {
            mTriggers.insert({rule, true});
        }

        void SetNoTrigger(const std::string& rule)
        {
            mTriggers.insert({rule, false});
        }

        bool CheckTrigger(const std::string& rule) const
        {
            bool result = false;
            auto it = mTriggers.find(rule);
            if (it != mTriggers.end()) { 
                result = it->second;
            }
            return result;
        }

    protected:
        std::unordered_map<std::string, bool> mTriggers;
};
