#ifndef DATASET_H
#define DATASET_H
//-----------------------------------------------------------------------------
/**
 * @file dataset.h
 *
 * @brief Dataset class to fill neural network data
 *
 *
 * @author Hernan Contigiani
 * Contact: hernan4790@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
//-----------------------------------------------------------------------------
namespace ANFIS {
//--------------------------------------------------------------------------------
class Dataset
{

// Constructor
public:
    Dataset();
    Dataset(std::string file_path);
    Dataset(std::vector<double> row);
    Dataset(const Dataset &d);
    virtual ~Dataset();


// Bahavior
public:
    void clear();
    void LoadFromCSV(std::string file_path);
    double operator()(int row, int col) const;
    std::vector<double> row(int row) const;

    void push_back(std::vector<double> row_data);
    void push_back(Dataset data);
    std::vector<double> pop_back();

    void random_shuffle();
    void train_validation_test_split(Dataset& train, Dataset& test, double validation_size,
                                              Dataset& validation, double test_size) const;

// Getters
public:
    int rows() { return rows_; }
    int cols() { return cols_; }

private:
    int rows_;
    int cols_;
    std::string header_;
    std::vector<std::vector<double> >data_;

};
//--------------------------------------------------------------------------------
}   // namespace
//--------------------------------------------------------------------------------
#endif // DATASET_H
