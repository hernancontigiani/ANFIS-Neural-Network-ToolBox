#include "input_layer.h"
//--------------------------------------------------------------------------------
#include "membership_function_triangular.h"
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
InputLayer::InputLayer()
:
type_(MembershipFunction::MF_NOT_DEFINED),
min_spread_(0),
max_spread_(0)
{

}
//--------------------------------------------------------------------------------
InputLayer::~InputLayer()
{

}
//--------------------------------------------------------------------------------
void InputLayer::operator=(const InputLayer &input_layer)
{
    mfs_.clear();
    add(input_layer.mfs_.size(), input_layer.type_,
        input_layer.min_spread_, input_layer.max_spread_);
}
//--------------------------------------------------------------------------------
void InputLayer::add(int membership_function_count, MembershipFunction::MemberShipFunctionType type,
                     double min_spread, double max_spread)
{
    type_ = type;
    min_spread_ = min_spread;
    max_spread_ = max_spread;
    for(int i=0; i<membership_function_count; i++)
    {
        std::shared_ptr<MembershipFunction> mf;
        switch(type)
        {
            case MembershipFunction::MF_TRIANGULAR_TYPE:
                mf = std::make_shared<TriangularMembershipFunction>(i,membership_function_count,
                                                                    min_spread,max_spread);
            break;
            default:
                mf = std::make_shared<TriangularMembershipFunction>(i,membership_function_count,
                                                                    min_spread,max_spread);
            break;
        }
        mfs_.push_back(mf);
    }
}
//--------------------------------------------------------------------------------
