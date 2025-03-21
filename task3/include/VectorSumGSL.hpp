#ifndef VECTOR_SUM_GSL_HPP
#define VECTOR_SUM_GSL_HPP

#include "VectorSumInterface.hpp"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

/**
 * @brief GSL-based vector sum implementation using gsl_vector_axpby.
 *
 * This class implements the VectorSumInterface using the GNU Scientific Library (GSL)
 * to compute the vector sum.
 *
 * Steps:
 * 1. Allocate three GSL vectors: 
 *    - vx for input vector x, 
 *    - vy for input vector y, and 
 *    - vd for the output vector d.
 * 2. Copy the data from the std::vector inputs into vx and vy.
 * 3. Initialize the result vector (vd) by copying vy (so that vd initially equals y).
 * 4. Call gsl_vector_axpby() with coefficients a and 1.0, which computes:
 *       vd = a * vx + 1.0 * vd
 *    Because vd initially contains y, the final result is d = a*x + y.
 * 5. Copy the contents of vd back into the std::vector output d.
 * 6. Free the allocated GSL vectors.
 *
 * This implementation leverages the optimized BLAS routines provided by GSL,
 * which may provide better performance for large vectors.
 * The interface remains uniform with the default implementation, allowing
 * the selection of the method at runtime.
 */
class VectorSumGSL : public VectorSumInterface {
public:
    virtual void compute_sum(const std::vector<double>& x,
                             const std::vector<double>& y,
                             double a,
                             std::vector<double>& d) override {
        // Determine the size of the input vector.
        size_t N = x.size();
        // Resize the output vector.
        d.resize(N);
        
        // Allocate GSL vectors for x, y, and the result d.
        gsl_vector* vx = gsl_vector_alloc(N);
        gsl_vector* vy = gsl_vector_alloc(N);
        gsl_vector* vd = gsl_vector_alloc(N);

        // Copy input data from the std::vector into the GSL vectors.
        for (size_t i = 0; i < N; ++i) {
            gsl_vector_set(vx, i, x[i]);
            gsl_vector_set(vy, i, y[i]);
        }
        // Initialize the result vector by copying vy (i.e., d = y initially).
        gsl_vector_memcpy(vd, vy);
        // Use GSL function to compute: vd = a * vx + 1.0 * vd.
        // With vd initially equal to y, this computes: d = a*x + y.
        gsl_vector_axpby(a, vx, 1.0, vd);

        // Copy the computed result from the GSL vector back into the std::vector.
        for (size_t i = 0; i < N; ++i) {
            d[i] = gsl_vector_get(vd, i);
        }

        // Free the allocated GSL vectors.
        gsl_vector_free(vx);
        gsl_vector_free(vy);
        gsl_vector_free(vd);
    }
};

#endif // VECTOR_SUM_GSL_HPP
