#ifndef GSL_SUMMATOR_HPP
#define GSL_SUMMATOR_HPP

#include "Summator.hpp"
#include <vector>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

/**
 * @brief GSL-based summation using gsl_vector_sum.
 *
 * This implementation allocates a GSL vector, copies the data from the input
 * std::vector into it, calls gsl_vector_sum() to compute the sum, and then frees
 * the allocated vector.
 */
class GSLSummator : public Summator {
public:
    double sum(const std::vector<double>& vec) const override {
        size_t N = vec.size();
        gsl_vector* v = gsl_vector_alloc(N);
        for (size_t i = 0; i < N; ++i) {
            gsl_vector_set(v, i, vec[i]);
        }
        double s = gsl_vector_sum(v);  // Uses GSL's vector-sum routine.
        gsl_vector_free(v);
        return s;
    }
};

#endif // GSL_SUMMATOR_HPP