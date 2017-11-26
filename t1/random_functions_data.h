#ifndef RANDOM_FUNCTIONS_DATA_H
#define RANDOM_FUNCTIONS_DATA_H

#include <vector>

class RandomFunctionsData
{
public:
    enum Type{
        CONSTANT = 0,
        NORMAL,
        UNIFORM,
        TRIANGULAR,
        EXPONENTIAL,
    };
    RandomFunctionsData(Type type, std::vector<double> args);
    double random_value();
    std::vector<double> args() const;
    Type type() const;
    double triangular_distribution(double min, double mode, double max);

private:
    Type type_;
    std::vector<double> args_;

};

#endif // RANDOM_FUNCTIONS_DATA_H
