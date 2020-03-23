#ifndef TAKAGI_SUGENO_ONE_H
#define TAKAGI_SUGENO_ONE_H
//-----------------------------------------------------------------------------
/**
 * @file takagi_sugeno_one.h
 *
 * @brief Takagi-Sugeno rule function, order one (linear)
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//--------------------------------------------------------------------------------
#include <takagi_sugeno_zero.h>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class TakagiSugenoOne : public TakagiSugenoZero
{

// Constructor
public:
    TakagiSugenoOne();
    virtual ~TakagiSugenoOne();

    virtual void operator=(const RuleLayer &takagi);

// Bahavior
public:
    virtual void process(std::vector<double> input);
    virtual void train(std::vector<double> input, double output,
                          double err, double err_relative,
                       double total_membership_cross_product_result, double training_rate);
    virtual void save(FILE* file);

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // TAKAGI_SUGENO_ONE_H
