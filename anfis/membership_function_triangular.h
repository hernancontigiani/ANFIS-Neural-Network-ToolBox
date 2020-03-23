#ifndef MEMBERSHIP_FUNCTION_TRIANGULAR_H
#define MEMBERSHIP_FUNCTION_TRIANGULAR_H
//-----------------------------------------------------------------------------
/**
 * @file dataset.h
 *
 * @brief Triangular membership function, inheritance from base MembershipFunction
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//--------------------------------------------------------------------------------
#include <membership_function.h>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class TriangularMembershipFunction : public MembershipFunction
{

// Constructor
public:
    TriangularMembershipFunction(int membershipfunction_index, int number_of_membership_function,
                                 double min_spread, double max_spread);
    virtual ~TriangularMembershipFunction();

    virtual void operator=(const MembershipFunction &mf);

// Behavior
public:
    void process(double x);
    virtual void train(double dEdW, double input, double training_rate);
    virtual void save(FILE* file);

private:
    double SignFunction(double x, double y);

private:
    double a_;
    double b_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // MEMBERSHIP_FUNCTION_TRIANGULAR_H
