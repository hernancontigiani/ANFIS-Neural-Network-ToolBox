#include "input_rule_link.h"
//--------------------------------------------------------------------------------
#include <membership_function.h>
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
InputRuleLink::InputRuleLink(std::shared_ptr<MembershipFunction> mf)
:
mf_(mf)
{

}
//--------------------------------------------------------------------------------
void InputRuleLink::operator=(const InputRuleLink &link)
{
    MembershipFunction* dest = mf_.get();
    MembershipFunction* source = link.mf_.get();
    // Copy content of one shr_ptr to other
    // Me cannont copy shr_ptr because we want the reference to be separeted
    // in orther to modify one without modify the other
    *(dest) = *(source);
}
//--------------------------------------------------------------------------------
InputRuleLink::~InputRuleLink()
{

}
//--------------------------------------------------------------------------------
