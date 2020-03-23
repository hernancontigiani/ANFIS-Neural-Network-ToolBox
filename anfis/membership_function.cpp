#include "membership_function.h"
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
MembershipFunction::MembershipFunction(int membership_function_index, int number_of_membership_function, double min_spread, double max_spread)
:
membership_function_index_(membership_function_index),
number_of_membership_function_(number_of_membership_function),
min_spread_(min_spread),
max_spread_(max_spread),
degree_of_membership_(0)
{

}
//--------------------------------------------------------------------------------
void MembershipFunction::operator=(const MembershipFunction &mf)
{
    membership_function_index_ = mf.membership_function_index_;
    number_of_membership_function_ = mf.number_of_membership_function_;
    min_spread_ = mf.min_spread_;
    max_spread_ = mf.max_spread_;
    degree_of_membership_ = mf.degree_of_membership_;
}
//--------------------------------------------------------------------------------
MembershipFunction::~MembershipFunction()
{

}
//--------------------------------------------------------------------------------
