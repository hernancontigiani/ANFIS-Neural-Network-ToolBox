#ifndef INPUT_LAYER_H
#define INPUT_LAYER_H
//--------------------------------------------------------------------------------
/**
 * @file input_layer.h
 *
 * @brief Input layer from the neural network. This layer contains the
 *        membership function of the system
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//--------------------------------------------------------------------------------
#include <membership_function.h>
//--------------------------------------------------------------------------------
#include <share.h>
#include <condition_variable>
#include <vector>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class InputLayer
{

// Constructor
public:
    InputLayer();
    virtual ~InputLayer();

    virtual void operator=(const InputLayer &rule_layer);

// Bahavior
public:
    void add(int membership_function_count, MembershipFunction::MemberShipFunctionType type, double min_spread, double max_spread);

// Getters
public:
    std::shared_ptr<MembershipFunction> mf(int i) { return mfs_.at(i); }
    std::vector<std::shared_ptr<MembershipFunction> >& mfs() { return mfs_; }
    MembershipFunction::MemberShipFunctionType type() { return type_; }
    double min_spread() { return min_spread_; }
    double max_spread() { return max_spread_; }

private:

    std::vector<std::shared_ptr<MembershipFunction> > mfs_;
    MembershipFunction::MemberShipFunctionType type_;
    double min_spread_;
    double max_spread_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // INPUT_LAYER_H
