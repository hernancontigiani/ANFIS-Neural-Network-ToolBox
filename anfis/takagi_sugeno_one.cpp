#include "takagi_sugeno_one.h"
#include "takagi_input_rule_link.h"
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
TakagiSugenoOne::TakagiSugenoOne()
{

}
//--------------------------------------------------------------------------------
TakagiSugenoOne::~TakagiSugenoOne()
{

}
//--------------------------------------------------------------------------------
void TakagiSugenoOne::operator=(const RuleLayer &takagi)
{
    TakagiSugenoZero::operator =(takagi);
}
//--------------------------------------------------------------------------------
void TakagiSugenoOne::process(std::vector<double> input)
{
    int input_index = 0;
    TakagiSugenoZero::process(input);

    // Output TakaSugenoOne is a linar
    // F = Sum(w*input)+k
    for(InputRuleLink* link : rule_input_links_)
    {
        TakagiInputRuleLink* takagi_link = dynamic_cast<TakagiInputRuleLink*>(link);
        rule_result_ += takagi_link->weight() * input.at(input_index);
        input_index++;
    }
}
//--------------------------------------------------------------------------------
void TakagiSugenoOne::train(std::vector<double> input, double output,
                      double err, double err_relative,
                   double total_membership_cross_product_result, double training_rate)
{

    TakagiSugenoZero::train(input,output,err,err_relative,total_membership_cross_product_result, training_rate);
    if(total_membership_cross_product_result == 0)
    {
        // None value could be obtain in this case
        return;
    }

    int input_index = 0;
    for(InputRuleLink* link : rule_input_links_)
    {
        TakagiInputRuleLink* takagi_link = dynamic_cast<TakagiInputRuleLink*>(link);
        double input_value = input.at(input_index);
        // dEdm = Err * (W/SumW)*X
        double dEdm = err * (membership_cross_product_result_/total_membership_cross_product_result)
                        * input_value;
        double m = takagi_link->weight() - training_rate * dEdm;
        takagi_link->set_weight(m);
        input_index++;
    }
}
//--------------------------------------------------------------------------------
void TakagiSugenoOne::save(FILE* file)
{
    fprintf(file,"Out= %f",k_);
    for(InputRuleLink* link : rule_input_links_)
    {
        TakagiInputRuleLink* takagi_link = dynamic_cast<TakagiInputRuleLink*>(link);
        fprintf(file,",%f",takagi_link->weight());
    }
    fprintf (file,"\n");
}
//--------------------------------------------------------------------------------
