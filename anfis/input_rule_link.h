#ifndef INPUT_RULE_LINK_H
#define INPUT_RULE_LINK_H
//-----------------------------------------------------------------------------
/**
 * @file input_rule_link.h
 *
 * @brief Rule link is the connection between each differents membership function
 *        and each rules. Depends of the function of the rule the behavior of the
 *        class
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#include <share.h>
#include <condition_variable>
#include <vector>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class MembershipFunction;
//--------------------------------------------------------------------------------
class InputRuleLink
{

// Constructor
public:
    InputRuleLink(std::shared_ptr<MembershipFunction> mf);
    virtual ~InputRuleLink();

    virtual void operator=(const InputRuleLink &link);

// Getter
public:
    std::shared_ptr<MembershipFunction>& mf() { return mf_; }

protected:
    std::shared_ptr<MembershipFunction> mf_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // INPUT_RULE_LINK_H
