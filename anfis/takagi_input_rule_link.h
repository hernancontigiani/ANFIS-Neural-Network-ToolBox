#ifndef TAKAGI_INPUT_RULE_LINK_H
#define TAKAGI_INPUT_RULE_LINK_H
//-----------------------------------------------------------------------------
/**
 * @file takagi_input_rule_link.h
 *
 * @brief Takagi-Sugeno rule input link type for Takagi-Sugeno rules's function
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#include <input_rule_link.h>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class TakagiInputRuleLink : public InputRuleLink
{

// Constructor
public:
    TakagiInputRuleLink(std::shared_ptr<MembershipFunction> mf);
    virtual ~TakagiInputRuleLink();

    void operator=(const InputRuleLink &link);

// Getters
public:
    double& weight() { return weight_; }

// Setters
public:
    void set_weight(double weight) { weight_ = weight; }

protected:
    double weight_;
};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // TAKAGI_INPUT_RULE_LINK_H
