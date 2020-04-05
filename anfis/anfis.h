#ifndef ANFIS_H
#define ANFIS_H
//--------------------------------------------------------------------------------
#include <input_layer.h>
#include <rule_layer.h>
#include <dataset.h>
//--------------------------------------------------------------------------------
namespace ANFIS{
//--------------------------------------------------------------------------------
class AnfisNeuralNetwork
{

// Constructor
public:
    AnfisNeuralNetwork();
    AnfisNeuralNetwork(const AnfisNeuralNetwork &anfis);
    ~AnfisNeuralNetwork();

    void operator=(const AnfisNeuralNetwork &anfis );

private:
    void copy(const AnfisNeuralNetwork& anfis);

// Behavior
public:
    void add(InputLayer input_layer);
    void add(RuleLayer *rule_layer);
    void compile(RuleLayer::RuleLayerFunctionType type);
    void fit(Dataset trainset, Dataset validationset, double training_rate, long long epochs,
             double max_error, bool keep_best_candidate =true);
    double predict(std::vector<double> input);
    double test(Dataset testset);

    void save(std::string file_path);

    void reset();

private:
    double train(std::vector<double> dataset, double training_rate);


// Getters
public:
    double epoch() { return epoch_; }
    double mean_squared_error() { return mean_squared_error_; }

// Attributes
private:
    std::vector<InputLayer> input_layers_;
    std::vector<RuleLayer*> rule_layers_;
    RuleLayer::RuleLayerFunctionType type_;

    double total_membership_cross_product_result_;

    double epoch_;
    double mean_squared_error_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // ANFIS_H
