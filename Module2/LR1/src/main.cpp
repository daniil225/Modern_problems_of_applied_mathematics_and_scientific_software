#include <boost/math/tools/minima.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <iostream>
#include <cmath>
#include <numeric>
#include <cmath>
#include <matplot/matplot.h>

#include "distribution.hpp"
#include "concepts.hpp"
#include "utils.hpp"

double objective(double x)
{
    return (x - 2.5) * (x - 2.5) + 1.0; // парабола с минимумом в x=2.5
}

template <typename T>
bool is_close(T expect, T got, T tolerance)
{
    using boost::math::fpc::close_at_tolerance;
    using boost::math::fpc::FPC_STRONG;
    using boost::math::fpc::is_small;

    if (is_small<T>(expect, tolerance))
    {
        return is_small<T>(expect, tolerance);
    }

    return close_at_tolerance<T>(tolerance, FPC_STRONG)(expect, got);
}

int main()
{

        constexpr double nu = 0.5;
        

        auto sn_distr = distribution::density::make_distribution_density<distribution::distribution_type::standart_SN_distribution>(nu, 5.0);
        DensityDistributionPlot(sn_distr, 2.0, 8.0, "SN Distribution density with nu = " + std::to_string(nu), "x", "y");
        
        auto SNDistributionGenerator = distribution::modeling::make_distribution_generator<distribution::distribution_type::standart_SN_distribution>(nu, 5.0);

        std::vector<double> distrs = generate_distribution<decltype(SNDistributionGenerator),std::vector<double>>(SNDistributionGenerator, static_cast<std::size_t>(1000000));

        DistributionHist(distrs, "SN Generated", "x", "y");

        std::cout << "Mean: " << sample_mean(distrs) << std::endl;
        std::cout << "Stdev: " << sample_dispersion(distrs) << std::endl;
        std::cout << "Asymmetry: " << sample_asymmetry_coefficient(distrs) << std::endl;
        std::cout << "Kurtosis: " << sample_kurtosis_coefficient(distrs) << std::endl;
    // using namespace boost::math::tools;
    // const boost::uintmax_t maxit = 20;
    // boost::uintmax_t it = maxit;
    // const int double_bits = std::numeric_limits<double>::digits/2;

    // // Ищем минимум на интервале [0, 10] с точностью 1e-8
    // auto result = brent_find_minima(objective, 0.0, 10.0, double_bits, it);

    // std::cout << "Минимум в x = " << result.first << std::endl;
    // std::cout << "Значение функции: " << result.second << std::endl;
    // std::cout << "Итераций:" << it << std::endl;

    return 0;
}