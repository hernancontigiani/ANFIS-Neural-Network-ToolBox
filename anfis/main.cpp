/**
 * @file main.cpp
 *
 * @brief main test file
 *
  *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//--------------------------------------------------------------------------------
#include <QCoreApplication>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <anfis.h>
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Load dataset from file
    Dataset input("exponential.txt");
    Dataset train, test, validation;

    // Split into train, test and validation datasets
    input.train_test_validation_split(train,test,0.15,validation,0.05);

    AnfisNeuralNetwork ann;
    InputLayer inp_a;

    // Create input with 3 membership function type triangular
    inp_a.add(3,MembershipFunction::MF_TRIANGULAR_TYPE,0,10);
    ann.add(inp_a);

    // Create ANFIS
    ann.compile(RuleLayer::RULE_SUGENO_ONE);

    // Fit
    ann.fit(train,test,0.01,1000,0);
    qCritical()<<"Train error "<<ann.mean_squared_error();

    // Evaluate
    double validation_error = ann.test(validation);
    qCritical()<<"Validation Error "<<validation_error;

    // Predict
    std::vector<double> predict_input;
    predict_input.push_back(5);
    double output = ann.predict(predict_input);
    qCritical()<<"Predict output "<<output;

    ann.save("fis.txt");
    qCritical()<<"ANFIS saved";

    return a.exec();
}
//--------------------------------------------------------------------------------
