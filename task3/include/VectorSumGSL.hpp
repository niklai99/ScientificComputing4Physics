#ifndef VECTOR_SUM_GSL_HPP
#define VECTOR_SUM_GSL_HPP

#include "VectorSumInterface.hpp"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

/**
 * @brief GSL-based vector sum implementation using gsl_vector_axpby.
 *
 * Computes: d = a * x + y.
 */
class VectorSumGSL : public VectorSumInterface {
public:
    virtual void compute_sum(const std::vector<double>& x,
                             const std::vector<double>& y,
                             double a,
                             std::vector<double>& d) override {
        size_t N = x.size();
        d.resize(N);
        gsl_vector* vx = gsl_vector_alloc(N);
        gsl_vector* vy = gsl_vector_alloc(N);
        gsl_vector* vd = gsl_vector_alloc(N);

        // Copy input data to GSL vectors.
        for (size_t i = 0; i < N; ++i) {
            gsl_vector_set(vx, i, x[i]);
            gsl_vector_set(vy, i, y[i]);
        }
        // Initialize result by copying y.
        gsl_vector_memcpy(vd, vy);
        // Compute: vd = a * vx + 1.0 * vd, i.e. d = a*x + y.
        gsl_vector_axpby(a, vx, 1.0, vd);

        // Copy the result back to std::vector.
        for (size_t i = 0; i < N; ++i) {
            d[i] = gsl_vector_get(vd, i);
        }

        gsl_vector_free(vx);
        gsl_vector_free(vy);
        gsl_vector_free(vd);
    }
};

#endif // VECTOR_SUM_GSL_HPP
