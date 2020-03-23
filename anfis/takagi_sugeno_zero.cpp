#include "takagi_sugeno_zero.h"
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
TakagiSugenoZero::TakagiSugenoZero()
:
k_(0)
{

}
//--------------------------------------------------------------------------------
void TakagiSugenoZero::operator=(const RuleLayer &takagi)
{
    RuleLayer::operator =(takagi);
    const TakagiSugenoZero& takagi_zero = dynamic_cast<const TakagiSugenoZero&>(takagi);
    k_ = takagi_zero.k_;
}
//--------------------------------------------------------------------------------
TakagiSugenoZero::~TakagiSugenoZero()
{

}
//--------------------------------------------------------------------------------
void TakagiSugenoZero::process(std::vector<double> input)
{
    RuleLayer::process(input);
    // Output TakaSugenoZero is a cte
    // F = k
    rule_result_ = k_;
}
//--------------------------------------------------------------------------------
void TakagiSugenoZero::train(std::vector<double> input, double output,
                      double err, double err_relative,
                   double total_membership_cross_product_result, double training_rate)
{

    RuleLayer::train(input,output,err,err_relative,total_membership_cross_product_result, training_rate);
    if(total_membership_cross_product_result == 0)
    {
        // None value could be obtain in this case
        return;
    }

    // dEdK = Err * (W/SumW)
    double dEdK = err * (membership_cross_product_result_/total_membership_cross_product_result);
    k_ -= training_rate * dEdK;
}
//--------------------------------------------------------------------------------
void TakagiSugenoZero::save(FILE* file)
{
    fprintf(file,"Out= %f\n",k_);
}
//--------------------------------------------------------------------------------
