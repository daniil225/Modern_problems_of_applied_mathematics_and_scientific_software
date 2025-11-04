#pragma once

/*
    Вспомогательные типы и функции
*/

#include "concepts.hpp"

#include <vector>
#include <numeric>
#include <cmath>
#include <matplot/matplot.h>




namespace details
{
    template <realization_container realization>
    realization::value_type sample_dispersion_helper(
        const realization &sample,
        typename realization::value_type mean)
    {
        using type = typename realization::value_type;

        return std::accumulate(
            sample.cbegin(), sample.cend(), // iter
            static_cast<type>(0),           // init value
            [mean](type sum, type yi)
            { return sum + std::pow(yi - mean, 2); } // binary operation
        );
    }

    template <realization_container realization>
    realization::value_type sample_asymmetry_coefficient_helper(
        const realization &sample,
        typename realization::value_type mean)
    {
        using type = typename realization::value_type;

        return std::accumulate(
            sample.cbegin(), sample.cend(),
            static_cast<type>(0),
            [mean](type sum, type yi)
            { return sum + std::pow(yi - mean, 3); });
    }

    template <realization_container realization>
    realization::value_type sample_kurtosis_coefficient_helper(
        const realization &sample, 
        typename realization::value_type mean)
    {
        using type = typename realization::value_type;

        return std::accumulate(
            sample.cbegin(), sample.cend(),
            static_cast<type>(0),
            [mean](type sum, type yi)
            { return sum + std::pow(yi - mean, 4); }); 
    }
}

template <realization_container realization>
realization::value_type sample_mean(const realization &sample)
{
    using type = typename realization::value_type;

    type N = static_cast<type>(sample.size());
    type M = std::accumulate(
        sample.cbegin(), sample.cend(), // iter
        static_cast<type>(0)            // init value
    );

    return M / N;
}

template <realization_container realization>
realization::value_type sample_dispersion(const realization &sample)
{
    using type = typename realization::value_type;

    type mean = ::sample_mean(sample);
    type N = static_cast<type>(sample.size());
    return details::sample_dispersion_helper(sample, mean) / N;
}

template <realization_container realization>
realization::value_type sample_dispersion(
    const realization &sample,
    typename realization::value_type mean)
{
    using type = typename realization::value_type;

    type N = static_cast<type>(sample.size());
    return details::sample_dispersion_helper(sample, mean) / N;
}

template <realization_container realization>
realization::value_type sample_asymmetry_coefficient(const realization &sample)
{
    using type = typename realization::value_type;

    type mean = ::sample_mean(sample);
    type dispersion = ::sample_dispersion(sample, mean);
    type N = static_cast<type>(sample.size());

    return details::sample_asymmetry_coefficient_helper(sample, mean) / (std::pow(dispersion, 1.5) * N);
}

template <realization_container realization>
realization::value_type sample_asymmetry_coefficient(
    const realization &sample,
    typename realization::value_type mean,
    typename realization::value_type dispersion)
{
    using type = typename realization::value_type;

    type N = static_cast<type>(sample.size());
    return details::sample_asymmetry_coefficient_helper(sample, mean) / (std::pow(dispersion, 1.5) * N);
}

template<realization_container realization>
realization::value_type sample_kurtosis_coefficient(const realization &sample, typename realization::value_type mean, typename realization::value_type dispersion)
{
    using type = typename realization::value_type;

    type N = static_cast<type>(sample.size());

    return (details::sample_kurtosis_coefficient_helper(sample, mean) / (N * std::pow(dispersion, 2))) - static_cast<type>(3);
}

template<realization_container realization>
realization::value_type sample_kurtosis_coefficient(const realization &sample)
{
    using type = typename realization::value_type;

    type mean = sample_mean(sample);
    type dispersion = sample_dispersion(sample, mean);

    return sample_kurtosis_coefficient(sample, mean, dispersion);
}


template<typename distribution_t, realization_container realization>
realization generate_distribution(const distribution_t& distr, std::size_t N)
{
    realization res;
    auto&& bit = std::back_inserter(res);
    for(std::size_t i = 0; i < N; i++)
        bit = distr();

    return res;
}

/* Ploting functions */

template<typename density_distribution_t, floating_point interval_t>
void DensityDistributionPlot(const density_distribution_t& f,
    interval_t a, interval_t b,
    const std::string& title, const std::string& xl, const std::string& yl,
    std::size_t detalization = 1000)
{
    std::vector<double> x = matplot::linspace(a, b, detalization);
    std::vector<double> y = matplot::transform(x, f);


    matplot::plot(x, y);
    matplot::title(title);
    matplot::xlabel(xl);
    matplot::ylabel(yl);
    matplot::show();
}

template<typename sample_t>
void DistributionHist(const sample_t& samples, 
    const std::string& title, const std::string& xl, const std::string& yl)
{
    matplot::hist(samples);
    matplot::title(title);
    matplot::xlabel(xl);
    matplot::ylabel(yl);
    matplot::show();
}