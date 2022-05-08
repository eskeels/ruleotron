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
        RulesTriggered(const std::map<uint16_t,bool> rulesAndTriggers)
        {
            for (auto& p : rulesAndTriggers) {
                SetTrigger(p.first, p.second);
            }    
        }

        void SetTrigger(uint16_t id, bool v)
        {
            mTriggers.insert({id, v}); 
        }


        void SetTrigger(const std::string& ruleId, bool v)
        {
            uint16_t id = std::stoul(ruleId, nullptr, 16);

            mTriggers.insert({id, v}); 
        }

        void SetTrigger(const std::string& ruleId)
        {
            uint16_t id = std::stoul(ruleId, nullptr, 16);

            mTriggers.insert({id, true});
        }

        void SetNoTrigger(const std::string& ruleId)
        {
            uint16_t id = std::stoul(ruleId, nullptr, 16);

            mTriggers.insert({id, false});
        }

        bool CheckTrigger(const std::string& ruleId) const
        {
            uint16_t id = std::stoul(ruleId, nullptr, 16);

            bool result = false;
            auto it = mTriggers.find(id);
            if (it != mTriggers.end()) { 
                result = it->second;
            }
            return result;
        }

    protected:
        std::unordered_map<uint16_t, bool> mTriggers;
};
