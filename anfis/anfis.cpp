#include "anfis.h"
//--------------------------------------------------------------------------------
#include <input_rule_link.h>
//--------------------------------------------------------------------------------
#include <takagi_sugeno_one.h>
#include <takagi_sugeno_zero.h>
#include <takagi_input_rule_link.h>
//--------------------------------------------------------------------------------
#include <cmath>
//--------------------------------------------------------------------------------
#include <QDebug>
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
AnfisNeuralNetwork::AnfisNeuralNetwork()
:
total_membership_cross_product_result_(0),
epoch_(0),
mean_squared_error_(0),
type_(RuleLayer::RULE_NOT_DEFINED)
{

}
//--------------------------------------------------------------------------------
AnfisNeuralNetwork::AnfisNeuralNetwork(const AnfisNeuralNetwork& anfis)
:
AnfisNeuralNetwork()
{
    copy(anfis);
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::operator=(const AnfisNeuralNetwork &anfis)
{
    copy(anfis);
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::reset()
{
    total_membership_cross_product_result_ = 0;
    epoch_ = 0;
    mean_squared_error_ = 0;
    input_layers_.clear();
    rule_layers_.clear();
}
//--------------------------------------------------------------------------------
AnfisNeuralNetwork::~AnfisNeuralNetwork()
{

}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::add(InputLayer input_layer)
{
    input_layers_.push_back(input_layer);
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::add(RuleLayer* rule_layer)
{
    rule_layers_.push_back(rule_layer);
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::compile(RuleLayer::RuleLayerFunctionType type)
{
    int total_rules = 1;

    // Generate a membership index vector, which will
    // indicate at each rule the input membership function associated with it
    std::vector<int> mf_idx;
    mf_idx.resize(input_layers_.size());
    std::fill(mf_idx.begin(), mf_idx.end(), 0);

    type_ = type;

    // The total number of rules will be the combinational number
    // of membership functions
    for(InputLayer& input_layer : input_layers_)
    {
        total_rules *= input_layer.mfs().size();
    }
    for(int i=0; i<total_rules; i++)
    {
        RuleLayer* rule;
        switch(type)
        {
            case RuleLayer::RULE_SUGENO_ZERO: rule = new TakagiSugenoZero(); break;
            case RuleLayer::RULE_SUGENO_ONE: rule = new TakagiSugenoOne(); break;
            default: rule = new TakagiSugenoOne(); break;
        }

        // At each layer add the input MF associated
        for(int j=0; j<input_layers_.size(); j++)
        {
            InputRuleLink* link;
            std::shared_ptr<MembershipFunction> mf = input_layers_.at(j).mf(mf_idx.at(j));
            switch(type)
            {
                case RuleLayer::RULE_SUGENO_ZERO: link = new InputRuleLink(mf); break;
                case RuleLayer::RULE_SUGENO_ONE: link = new TakagiInputRuleLink(mf); break;
                default: link = new TakagiInputRuleLink(mf); break;
            }
            rule->add(link);
        }

        // Then increment index of associated MF for next rule
        int increment_next_idx = 1;
        for(int j=0; j<input_layers_.size(); j++)
        {
            mf_idx.at(j) += increment_next_idx;
            // If the index reach the max MF at that input layer:
            // true: reset index and increment next
            // false: no needed to continue increment index
            if(mf_idx.at(j) == input_layers_.at(j).mfs().size())
            {
                mf_idx.at(j) = 0;
                increment_next_idx = 1;
            }
            else
            {
                increment_next_idx = 0;
                break;
            }
        }
        //rule->process(input);
        add(rule);
    }
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::fit(Dataset trainset, Dataset validationset,
                             double training_rate, long long int epochs, double max_error,
                             bool keep_best_candidate)
{
    epoch_ = 0;
    mean_squared_error_ = std::numeric_limits<double>::max();
    int train_index = 0;

    AnfisNeuralNetwork best_candidate;

    best_candidate = *this;

    while (epoch_ < epochs && mean_squared_error_ > max_error)
    {
        if (train_index == trainset.rows())
        {
            // The training data was its over, generate new from sorting
            // current traning data with test data. Generate new datasets
            double validation_size = validationset.rows() / ((double)(trainset.rows()+validationset.rows()));
            Dataset train_test;
            train_test.push_back(trainset);
            train_test.push_back(validationset);
            Dataset test;
            train_test.train_validation_test_split(trainset,validationset,validation_size,test,0);
            train_index = 0;
        }

        // Train the system
        double train_error = train(trainset.row(train_index),training_rate);
        // Test the system
        double validation_error = test(validationset);
        if(train_error == std::numeric_limits<float>::infinity() || validation_error == std::numeric_limits<float>::infinity())
        {
            // System diverge, finish process
            break;
        }

        if(validationset.rows() > 0)
        {
            // If we are using a validation set, the error is related to the validation process
            mean_squared_error_ = validation_error;
        }
        else
        {
            // In the other hand we use the train error
            mean_squared_error_ = train_error;
        }

        // Evaluate training evolution
        if(keep_best_candidate == true && trainset.rows() > 1 && epochs > 1)
        {
            // If the system will make more than one epoch and the need is to
            // keep the best candidate, in each iteration we compare the MSE of new
            // and best candidate
            if(best_candidate.mean_squared_error_ > mean_squared_error_)
            {
                best_candidate = *this;
            }

        }
        else
        {
            // Each new iteration we keep the actual system
            best_candidate = *this;
        }

        epoch_++;
        train_index++;
    }

    // Copy best candidate to actual network
    *this = best_candidate;
}
//--------------------------------------------------------------------------------
double AnfisNeuralNetwork::predict(std::vector<double> input)
{
    total_membership_cross_product_result_ = 0;
    double output = 0;

    // Propagate input for each node
    for(RuleLayer* rule : rule_layers_)
    {
        rule->process(input);
        output += rule->rule_result() * rule->membership_cross_product_result();
        total_membership_cross_product_result_ += rule->membership_cross_product_result();
    }
    if(total_membership_cross_product_result_ > 0)
    {
        output /= total_membership_cross_product_result_;
    }
    else
    {
        output = 0;
    }
    return output;
}
//--------------------------------------------------------------------------------
double AnfisNeuralNetwork::train(std::vector<double> dataset, double training_rate)
{
    // Split input and desired output from dataset
    std::vector<double> input(dataset.begin(), dataset.end()-1);
    double yd = dataset.at(dataset.size()-1);

    // Predict output with the current network
    double y = predict(input);

    double err, err_relative, mean_squared_error;

    // Calculate error
    err = y - yd;
    err_relative = std::abs((err / yd)*100);
    mean_squared_error = 0.5 * std::pow(err,2);

    // Propagate error
    for(RuleLayer* rule : rule_layers_)
    {
        rule->train(input,y,err,err_relative,total_membership_cross_product_result_,training_rate);
    }
    return mean_squared_error;
}
//--------------------------------------------------------------------------------
double AnfisNeuralNetwork::test(Dataset testset)
{
    double test_mean_squared_error = 0;

    // For each test data, propagate and calculate error
    for(int i=0; i<testset.rows(); i++)
    {
        std::vector<double> dataset = testset.row(i);
        std::vector<double> input(dataset.begin(), dataset.end()-1);
        double yd = dataset.at(dataset.size()-1);
        double y = predict(input);
        test_mean_squared_error += std::pow(yd- y,2);
    }
    test_mean_squared_error /= testset.rows();
    test_mean_squared_error *= (1/2.0);

    // mean squared error MSE = (1/2) * sum( (yd-y)^2 )
    return test_mean_squared_error;
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::copy(const AnfisNeuralNetwork &anfis)
{
    // Create blank input layers
    reset();
    for(const InputLayer& input_layer : anfis.input_layers_)
    {
        InputLayer inp;
        inp = input_layer;
        add(inp);
    }
    // Create blank rule layers
    compile(anfis.type_);

    // Something goes wrong! The networks doesnt match
    if(anfis.rule_layers_.size() != rule_layers_.size())
    {
        reset();
        return;
    }

    total_membership_cross_product_result_ = anfis.total_membership_cross_product_result_;
    epoch_ = anfis.epoch_;
    mean_squared_error_ = anfis.mean_squared_error_;
    type_ = anfis.type_;

    // Copy that from each rule, complete weigths and membership function data
    for(int i=0; i<rule_layers_.size(); i++)
    {
        *(rule_layers_.at(i)) = *(anfis.rule_layers_.at(i));
    }
}
//--------------------------------------------------------------------------------
void AnfisNeuralNetwork::save(std::string file_path)
{
    FILE *file;

    file = fopen(file_path.data(), "w");
    if (!file)
    {
        return;
    }

    // System details
    switch(type_)
    {
        case RuleLayer::RULE_SUGENO_ZERO: fprintf(file,"Type= constant\n"); break;
        case RuleLayer::RULE_SUGENO_ONE: fprintf(file,"Type= linear\n"); break;
        default: fprintf(file,"Type= linear\n"); break;
    }

    fprintf(file,"NumInputs= %d\n",input_layers_.size());
    fprintf(file,"NumOutputs= 1\n");
    fprintf(file,"NumRules= %d\n",rule_layers_.size());

    for(int i = 0; i < input_layers_.size(); i++)
    {
        fprintf(file,"NumMF= %d\n",input_layers_.at(i).mfs().size());
    }
    fprintf (file,"\n");

    // Inputs
    for(int i = 0; i < input_layers_.size(); i++)
    {
        fprintf(file,"[Input%d]\n",i+1);
        fprintf(file,"Name= input%d\n",i+1);
        fprintf(file,"Range= [%f %f]\n",input_layers_.at(i).min_spread(),input_layers_.at(i).max_spread());
        for(std::shared_ptr<MembershipFunction>& mf : input_layers_.at(i).mfs())
        {
            mf->save(file);
        }
        fprintf (file,"\n");
    }

    // Salidas
    fprintf(file,"[Output]\n");
    fprintf(file,"Name= output\n");
    for(RuleLayer* rule : rule_layers_)
    {
        rule->save(file);
    }
    fclose(file);
}
//--------------------------------------------------------------------------------
