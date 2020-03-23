#ifndef MEMBERSHIP_FUNCTION_H
#define MEMBERSHIP_FUNCTION_H
//-----------------------------------------------------------------------------
/**
 * @file membership_function.h
 *
 * @brief Base virtual class for any membership function for the system
 *        For example: triangular function, sigma function, trapezoid
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#include <sstream>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class MembershipFunction
{

public:
typedef enum {
    MF_NOT_DEFINED = 0,
    MF_TRIANGULAR_TYPE = 1,
} MemberShipFunctionType;

// Constructor
public:
    MembershipFunction(int index, int number_of_membership_function, double min_spread, double max_spread);
    virtual ~MembershipFunction();

    virtual void operator=(const MembershipFunction &mf);

// Behavior
public:
    virtual void process(double x) =0;
    virtual void train(double dEdW, double input, double training_rate) =0;
    virtual void save(FILE* file) =0;

    double degree_of_membership() { return degree_of_membership_; }

protected:
    int membership_function_index_;
    int number_of_membership_function_;
    double min_spread_;
    double max_spread_;
    double degree_of_membership_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // MEMBERSHIP_FUNCTION_H
