#include "random_functions_data.h"
#include <random>
#include <math.h>

RandomFunctionsData::RandomFunctionsData(RandomFunctionsData::Type type, std::vector<double> args):
    type_(type), args_(args)
{
}

double RandomFunctionsData::random_value()
{
    double random_value;
    std::default_random_engine generator;
    switch (this->type()) {
    case CONSTANT:
    {
        random_value = args().at(0);
        break;
    }
    case NORMAL:
    {
        std::normal_distribution<double> distribution(args().at(0), args().at(1));
        random_value = distribution(generator);
        break;
    }
    case UNIFORM:
    {
        std::uniform_real_distribution<double> uniform(args().at(0), args().at(1));
        random_value = uniform(generator);
        break;
    }
    case TRIANGULAR:
    {
        random_value = triangular_distribution(args().at(0),args().at(1), args().at(2));
        break;
    }
    case EXPONENTIAL:
    {
        std::exponential_distribution<double> exponential(args().at(0));
        random_value = exponential(generator);
        break;
    }
    default:
        random_value = 0;
        break;
    }
    return random_value;
}

std::vector<double> RandomFunctionsData::args() const
{
    return args_;
}

RandomFunctionsData::Type RandomFunctionsData::type() const
{
    return type_;
}

double RandomFunctionsData::triangular_distribution(double min, double mode, double max)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> uniform(0,1);
    double U = uniform(generator);
    double Fc = (max - min)/(mode - min);
    if(U > 0 && U < Fc) {
        return min + sqrt(U*(mode - min)*(max-min));
    } else { // U < 1 && Fc <= U
        return mode - sqrt((1-U)*(mode-min)*(mode-max));
    }
}
