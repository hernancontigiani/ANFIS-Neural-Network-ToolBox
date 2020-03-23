#ifndef TAKAGI_SUGENO_ZERO_H
#define TAKAGI_SUGENO_ZERO_H
//-----------------------------------------------------------------------------
/**
 * @file takagi_sugeno_zero.h
 *
 * @brief Takagi-Sugeno rule function, order zero (constant)
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//--------------------------------------------------------------------------------
#include <rule_layer.h>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class TakagiSugenoZero : public RuleLayer
{

// Constructor
public:
    TakagiSugenoZero();
    virtual ~TakagiSugenoZero();

    virtual void operator=(const RuleLayer &takagi);

// Behavior
public:
    virtual void process(std::vector<double> input);
    virtual void train(std::vector<double> input, double output,
                          double err, double err_relative,
                       double total_membership_cross_product_result, double training_rate);
    virtual void save(FILE* file);

// Attributes
protected:
    double k_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // TAKAGI_SUGENO_ZERO_H
