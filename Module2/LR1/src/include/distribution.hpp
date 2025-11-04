#pragma once

#include "concepts.hpp"
#include <functional>
#include <stdexcept>
#include <cmath>
#include <numbers>
#include <random>

namespace distribution
{
    enum class distribution_type
    {
        standart_normal,
        standart_SN_distribution,
    };

    namespace details
    {
        template <floating_point ValueType, distribution_type DistributionType>
        struct standart_density_distribution
        {
        };

        template <floating_point ValueType, distribution_type DistribytionType>
        struct distribution_constraint
        {
        };

        template<floating_point ValueType, distribution_type DistribytionType>
        struct standart_distribution_generator{};

        /* distribution */
        template <floating_point ValueType>
        struct standart_density_distribution<ValueType, distribution_type::standart_normal>
        {
            ValueType operator()(ValueType x)
            {
                ValueType inv_sqrt2 = 1.0 / std::numbers::sqrt2;
                return inv_sqrt2 * std::numbers::inv_sqrtpi * static_cast<ValueType>(std::exp(-std::pow(x, 2) / 2.0));
            }
        };

        template <floating_point ValueType>
        struct standart_density_distribution<ValueType, distribution_type::standart_SN_distribution>
        {
            ValueType operator()(ValueType x, ValueType nu)
            {
                ValueType coef = 2.0 / nu;
                return coef * standart_density_distribution<ValueType, distribution_type::standart_normal>()(coef * std::sinh(x)) * std::cosh(x);
            }
        };

        /* constraint */
        template <floating_point ValueType>
        struct distribution_constraint<ValueType, distribution_type::standart_SN_distribution>
        {
            bool operator()(ValueType nu)
            {
                if (nu > 0 && nu < 2)
                    return true;
                return false;
            }
        };

        /* distribution modeling */
        template<floating_point ValueType>
        struct standart_distribution_generator<ValueType, distribution_type::standart_SN_distribution>
        {
            ValueType operator()(ValueType nu)
            {
                static std::random_device rd{};
                static std::mt19937 gen{rd()};
                static std::normal_distribution normal_gen{0.0, 1.0};
                
                ValueType z = normal_gen(gen);
                return std::asinh(nu*z/2.0);
            }
        };
    }

    namespace modeling
    {
        template <floating_point ValueType>
        using distribution_generator_t = std::function<ValueType(void)>;

        template<distribution_type DistributionType, floating_point ValueType>
        distribution_generator_t<ValueType> make_standart_distribution_generator(ValueType nu)
        {
            if(!details::distribution_constraint<ValueType,DistributionType>()(nu))
            {
                throw std::invalid_argument("nu must be sutisfie the distribution constraint");
            }

            return [nu](void) -> ValueType
            {
                return details::standart_distribution_generator<ValueType, DistributionType>()(nu);
            };
        }

        template<distribution_type DistributionType, floating_point ValueType>
        distribution_generator_t<ValueType> make_distribution_generator(ValueType nu, ValueType theta = 0.0, ValueType lambda = 1.0)
        {
            distribution_generator_t<ValueType> generator = make_standart_distribution_generator<DistributionType, ValueType>(nu);

            return [generator = std::move(generator),theta, lambda](void) -> ValueType
            {
                ValueType x = generator();
                return theta + lambda*x;
            };
        }
    }

    namespace density
    {

        template <floating_point ValueType>
        using distribution_density_function_t = std::function<ValueType(ValueType)>;  

        template<distribution_type DistributionType, floating_point ValueType>
        distribution_density_function_t<ValueType> make_standart_distribution_density(ValueType nu)
        {
            if(!details::distribution_constraint<ValueType, DistributionType>()(nu))
            {
                throw std::invalid_argument("nu must be sutisfie the distribution constraint");
            }

            return [nu](ValueType x) -> ValueType
            {
                return details::standart_density_distribution<ValueType, DistributionType>()(x, nu);
            };
        }

        template<distribution_type DistributionType, floating_point ValueType>
        distribution_density_function_t<ValueType> make_distribution_density(ValueType nu, ValueType theta = 0.0, ValueType lambda = 1.0)
        {
            distribution_density_function_t<ValueType> distribution_density = make_standart_distribution_density<DistributionType, ValueType>(nu);

            return [distribution_density = std::move(distribution_density), theta, lambda](ValueType x) -> ValueType 
            {
                ValueType arg = (x - theta)/lambda;
                return distribution_density(arg)/lambda;
            };
        }
    }
}
