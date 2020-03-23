#include "rule_layer.h"
//--------------------------------------------------------------------------------
#include <input_rule_link.h>
#include <membership_function.h>
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
RuleLayer::RuleLayer()
:
membership_cross_product_result_(0),
rule_result_(0)
{

}
//--------------------------------------------------------------------------------
void RuleLayer::operator=(const RuleLayer& rule_layer)
{
    copy(rule_layer);
}
//--------------------------------------------------------------------------------
RuleLayer::~RuleLayer()
{

}
//--------------------------------------------------------------------------------
void RuleLayer::add(InputRuleLink *rule_input_link)
{
    rule_input_links_.push_back(rule_input_link);
}
//--------------------------------------------------------------------------------
void RuleLayer::process(std::vector<double> input)
{
    int input_index = 0;
    membership_cross_product_result_ = 1;

    // For each membership function (MF) affected by this rule
    // the membership deegre is calculated related to the input value
    // and the cross product result is adquired for the total of inputs
    for(InputRuleLink* link : rule_input_links_)
    {
        link->mf()->process(input.at(input_index));
        membership_cross_product_result_ *= link->mf()->degree_of_membership();
        input_index++;
    }
}
//--------------------------------------------------------------------------------
void RuleLayer::train(std::vector<double> input, double output,
                      double err, double err_relative,
                   double total_membership_cross_product_result, double training_rate)
{

    if(err_relative < training_rate)
    {
        // if error is minor to traning rate, it is not necesary to adapt
        // the shape from membership function
        return;
    }
    if(total_membership_cross_product_result == 0)
    {
        // None value could be obtain in this case
        return;
    }

    int input_index = 0;
    // dEdW = Err * ((F-Y)/SumW)*W
    double dEdW = err*
            (((rule_result_-output)*membership_cross_product_result_)
             /total_membership_cross_product_result);

    for(InputRuleLink* link : rule_input_links_)
    {
        double input_value = input.at(input_index);
        link->mf()->train(dEdW,input_value,training_rate);
        input_index++;
    }
}
//--------------------------------------------------------------------------------
void RuleLayer::copy(const RuleLayer& rule_layer)
{
    if(rule_input_links_.size() != rule_layer.rule_input_links_.size())
    {
        return;
    }

    membership_cross_product_result_ = rule_layer.membership_cross_product_result_;
    rule_result_ = rule_layer.rule_result_;

    for(int i=0; i<rule_input_links_.size(); i++)
    {
        *(rule_input_links_.at(i)) = *(rule_layer.rule_input_links_.at(i));
    }
}
//--------------------------------------------------------------------------------
