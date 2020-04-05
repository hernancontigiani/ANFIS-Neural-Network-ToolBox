#include "dataset.h"
//--------------------------------------------------------------------------------
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
//--------------------------------------------------------------------------------
using namespace ANFIS;
//--------------------------------------------------------------------------------
Dataset::Dataset()
:
rows_(0),
cols_(0),
header_("")
{

}
//--------------------------------------------------------------------------------
Dataset::Dataset(std::string file_path)
:
Dataset()
{
    LoadFromCSV(file_path);
}
//--------------------------------------------------------------------------------
Dataset::Dataset(std::vector<double> row)
:
Dataset()
{
    if(row.size() > 0)
    {
        rows_ = 1;  // Insert one row
        data_.push_back(row);
    }
}
//--------------------------------------------------------------------------------
Dataset::Dataset(const Dataset &d)
{
    data_ = d.data_;
    rows_ = d.rows_;
    cols_ = d.cols_;
    header_ = d.header_;
}
//--------------------------------------------------------------------------------
Dataset::~Dataset()
{

}
//--------------------------------------------------------------------------------
void Dataset::clear()
{
    data_.clear();
    rows_ = 0;
    cols_ = 0;
    header_ = "";
}
//--------------------------------------------------------------------------------
void Dataset::LoadFromCSV(std::string file_path)
{
    std::ifstream file(file_path);

    if(file.is_open() == false)
    {
        return;
    }

    clear();    // Clear current dataset
    std::string line = "";
    getline(file, line);    // Get header
    header_ = line;

    // Read each line and separate value by comma
    std::vector<double> row_data;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        row_data.clear();
        for (double i; ss >> i;)
        {
            row_data.push_back(i);
            // From the first row, count numbers of cols
            if(rows_ == 0)
            {
                cols_++;
            }
            if (ss.peek() == ',')
                ss.ignore();
        }
        data_.push_back(row_data);
        rows_++;
    }
    // Close the File
    file.close();
}
//--------------------------------------------------------------------------------
double Dataset::operator()(int row, int col) const
{
    double res = 0;
    if(row < rows_ && col < cols_)
    {
        res = data_.at(row).at(col);
    }
    return res;
}
//--------------------------------------------------------------------------------
std::vector<double> Dataset::row(int row) const
{
    std::vector<double> row_data;
    if(row < rows_)
    {
        row_data = data_.at(row);
    }
    return row_data;
}
//--------------------------------------------------------------------------------
void Dataset::push_back(std::vector<double> row_data)
{
    if(row_data.size() > 0)
    {
        data_.push_back(row_data);
        rows_++;
    }
}
//--------------------------------------------------------------------------------
void Dataset::push_back(Dataset data)
{
    // if the current dataset is empty or the schema are identical
    // it is allowed to append dataset to dataset
    if(cols_ == 0 || data.cols() == cols_)
    {
        data_.insert(std::end(data_),std::begin(data.data_),std::end(data.data_));
        rows_ += data.rows();
        cols_ = data.cols();
    }
}
//--------------------------------------------------------------------------------
void Dataset::random_shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data_.begin(), data_.end(), g);
}
//--------------------------------------------------------------------------------
std::vector<double> Dataset::pop_back()
{
    std::vector<double> row_data;
    if(rows_ > 0)
    {
        for(double rdata : data_.at(rows_-1))
        {
            row_data.push_back(rdata);
        }
        data_.erase(std::end(data_));
        rows_--;
    }
    return row_data;
}
//--------------------------------------------------------------------------------
void Dataset::train_validation_test_split(Dataset& train, Dataset& validation, double validation_size,
                                          Dataset& test, double test_size) const
{
    if(rows_ == 0)
    {
        // Empty dataset
        return;
    }
    if((test_size + validation_size) > 1.0)
    {
        // test size or validation size error
        return;
    }

    // Copy current dataset into auxiliary and random shuffle
    Dataset random_set = *this;
    random_set.random_shuffle();

    // Calculate each dataset length
    int train_length = random_set.rows() * (1-test_size-validation_size);
    int validation_length = random_set.rows() * (validation_size);
    int test_length = random_set.rows() * (test_size);

    int remain = random_set.rows() - train_length - validation_length - test_length;
    train_length += remain;

    // Complete datasets
    for(int i=0; i<train_length; i++)
    {
        train.push_back(random_set.pop_back());
    }
    for(int i=0; i<validation_length; i++)
    {
        validation.push_back(random_set.pop_back());
    }
    for(int i=0; i<test_length; i++)
    {
        test.push_back(random_set.pop_back());
    }

}
//--------------------------------------------------------------------------------

