#include "membership_function_triangular.h"
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
TriangularMembershipFunction::TriangularMembershipFunction(int membership_function_index,
                                                           int number_of_membership_function,
                                                           double min_spread, double max_spread)
:
MembershipFunction(membership_function_index,number_of_membership_function,min_spread,max_spread),
a_(0),
b_(0)
{
    // In this part it is calculated the position of the membership in the input spread possible values
    double delta_x;
    if(number_of_membership_function > 1)
    {
        delta_x = (max_spread_-min_spread_)/(number_of_membership_function-1);
        a_ = min_spread + membership_function_index * delta_x;
    }
    else
    {
        delta_x = (max_spread_ - min_spread_)/2.0;
        a_ = min_spread + delta_x;
    }
    b_ = delta_x*2;

}
//--------------------------------------------------------------------------------
void TriangularMembershipFunction::operator=(const MembershipFunction &mf)
{
    MembershipFunction::operator =(mf);
    const TriangularMembershipFunction& triangular_mf = dynamic_cast<const TriangularMembershipFunction&>(mf);
    a_ = triangular_mf.a_;
    b_ = triangular_mf.b_;
}
//--------------------------------------------------------------------------------
TriangularMembershipFunction::~TriangularMembershipFunction()
{

}
//--------------------------------------------------------------------------------
void TriangularMembershipFunction::process(double x)
{
    if (x < (a_-(b_/2)))
    {
        degree_of_membership_ =  0;
        return;
    }
    if (x > (a_+(b_/2)))
    {
        degree_of_membership_ =  0;
        return;
    }
    if (x <= a_)
    {
       /* if(b_ != 0)
        {
            degree_of_membership_ =  (1-2*((a_-x)/b_));
        }
        else
        {
            degree_of_membership_ = 0;
        }*/
        degree_of_membership_ =  (1-2*((a_-x)/b_));
        return;
    }
    if (x > a_)
    {
       /* if(b_ != 0)
        {
            degree_of_membership_ = (1-2*((x-a_)/b_));
        }
        else
        {
            degree_of_membership_ = 0;
        }*/
        degree_of_membership_ = (1-2*((x-a_)/b_));
        return;
    }
}
//--------------------------------------------------------------------------------
void TriangularMembershipFunction::train(double dEdW, double input, double training_rate)
{
    double dWdA = 0;
    double dWdB = 0;
    if(degree_of_membership_ != 0)
    {
        // dWdA = (1/A) * (2/b) * sign(X,a)
        dWdA = (1/degree_of_membership_) * (2/b_) * SignFunction(input,a_);
    }
    if(degree_of_membership_ != 0 && b_ != 0)
    {
        // dWdB = (1/A) * ((1-A)/b)
        dWdB = (1/degree_of_membership_) * ((1-degree_of_membership_)/b_);
    }

    double dEdA = dEdW * dWdA;
    a_ -= training_rate * dEdA;

    double dEdB = dEdW * dWdB;
    b_ -= training_rate * dEdB;
}
//--------------------------------------------------------------------------------
void TriangularMembershipFunction::save(FILE* file)
{
    fprintf(file,"MF= [%f %f %f]\n", a_- (b_/2),a_, a_ + (b_/2));
}
//--------------------------------------------------------------------------------
double TriangularMembershipFunction::SignFunction(double x, double y)
{
    int res;
    if ( x > y) res = 1;
    if ( x < y) res = -1;
    if ( x == y) res = 0;
    return res;
}
//--------------------------------------------------------------------------------
