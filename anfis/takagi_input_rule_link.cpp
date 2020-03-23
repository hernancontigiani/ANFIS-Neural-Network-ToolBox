#include "takagi_input_rule_link.h"
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
TakagiInputRuleLink::TakagiInputRuleLink(std::shared_ptr<MembershipFunction> mf)
:
InputRuleLink(mf),
weight_(0)
{

}
//--------------------------------------------------------------------------------
void TakagiInputRuleLink::operator=(const InputRuleLink &link)
{
    InputRuleLink::operator =(link);
    const TakagiInputRuleLink& takagi_link = dynamic_cast<const TakagiInputRuleLink&>(link);
    weight_ = takagi_link.weight_;
}
//--------------------------------------------------------------------------------
TakagiInputRuleLink::~TakagiInputRuleLink()
{

}
//--------------------------------------------------------------------------------
