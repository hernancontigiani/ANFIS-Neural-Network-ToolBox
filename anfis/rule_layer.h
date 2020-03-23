#ifndef RULE_LAYER_H
#define RULE_LAYER_H
//-----------------------------------------------------------------------------
/**
 * @file rule_layer.h
 *
 * @brief Rule layer of the neural network which have each rule link relationship
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#include <input_rule_link.h>
//--------------------------------------------------------------------------------
#include <share.h>
#include <condition_variable>
#include <vector>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class RuleLayer
{

public:
typedef enum {
    RULE_NOT_DEFINED = 0,
    RULE_SUGENO_ZERO = 1,
    RULE_SUGENO_ONE = 2,
} RuleLayerFunctionType;

// Constructor
public:
    RuleLayer();
    virtual ~RuleLayer();

    virtual void operator=(const RuleLayer& rule_layer);

private:
    virtual void copy(const RuleLayer &rule_layer);

// Behavior
public:
    void add(InputRuleLink* rule_input_link);
    virtual void process(std::vector<double> input);
    virtual void train(std::vector<double> input, double output, double err, double err_relative,
                       double total_membership_cross_product_result, double training_rate);
    virtual void save(FILE* file) =0;

// Getter
public:
    std::vector<InputRuleLink*>& rule_input_links() { return rule_input_links_; }
    double membership_cross_product_result() { return membership_cross_product_result_; }
    double rule_result() { return rule_result_; }

protected:
    std::vector<InputRuleLink*> rule_input_links_;
    double membership_cross_product_result_;
    double rule_result_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // RULE_LAYER_H
