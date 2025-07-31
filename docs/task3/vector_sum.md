The vector sum operation  

$$
d = a \cdot x + y,
$$

is implemented via a polymorphic interface in C++. This allows seamless switching between a simple element-wise loop and a GSL-optimized version.

## Common interface

All implementations derive from an abstract base class `VectorSumInterface`, declared in `VectorSumInterface.hpp`:

```cpp linenums="1" title="VectorSumInterface.hpp"
class VectorSumInterface {
public:
    virtual ~VectorSumInterface() {}

    /// Compute d = a*x + y.
    virtual void compute_sum(
        const std::vector<double>& x,
        const std::vector<double>& y,
        double a,
        std::vector<double>& d
    ) = 0;
};
```

This enforces a uniform method signature across all concrete classes.

## Default (element-by-element) implementation

The class `VectorSumDefault` provides the straightforward loop-based approach. Its code is in `VectorSumDefault.hpp`:

```cpp linenums="1" title="VectorSumDefault.hpp"
class VectorSumDefault : public VectorSumInterface {
public:
    void compute_sum(
        const std::vector<double>& x,
        const std::vector<double>& y,
        double a,
        std::vector<double>& d
    ) override {
        size_t N = x.size();
        d.resize(N);
        for (size_t i = 0; i < N; ++i) {
            d[i] = a * x[i] + y[i];
        }
    }
};
```

This version requires only the C++ standard library and works reliably for all vector sizes.

## GSL-based implementation

For larger vectors or performance-sensitive scenarios, `VectorSumGSL` uses the GNU Scientific Library. Its implementation resides in `VectorSumGSL.hpp`:

```cpp linenums="1" title="VectorSumGSL.hpp"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

class VectorSumGSL : public VectorSumInterface {
public:
    void compute_sum(
        const std::vector<double>& x,
        const std::vector<double>& y,
        double a,
        std::vector<double>& d
    ) override {
        size_t N = x.size();
        d.resize(N);

        // Allocate GSL vectors
        gsl_vector* vx = gsl_vector_alloc(N);
        gsl_vector* vy = gsl_vector_alloc(N);
        gsl_vector* vd = gsl_vector_alloc(N);

        // Copy data into GSL vectors
        for (size_t i = 0; i < N; ++i) {
            gsl_vector_set(vx, i, x[i]);
            gsl_vector_set(vy, i, y[i]);
        }

        // Initialize vd = y
        gsl_vector_memcpy(vd, vy);
        // Compute vd = a * vx + 1.0 * vd  ⇒  d = a*x + y
        gsl_vector_axpby(a, vx, 1.0, vd);

        // Copy result back to std::vector
        for (size_t i = 0; i < N; ++i) {
            d[i] = gsl_vector_get(vd, i);
        }

        // Free GSL resources
        gsl_vector_free(vx);
        gsl_vector_free(vy);
        gsl_vector_free(vd);
    }
};
```

This leverages gsl_vector_axpby, which internally can use optimized BLAS routines.

## Runtime selection of implementation

At execution, the implementation is chosen based on the implementation field in the `YAML` config. In `vectorSum.cpp`, a `std::unique_ptr<VectorSumInterface>` is created as either `VectorSumDefault` or `VectorSumGSL`:

```cpp linenums="1" title="vectorSum.cpp"
std::unique_ptr<VectorSumInterface> vs;
if (implementation == "default") {
    vs.reset(new VectorSumDefault());
}
else if (implementation == "gsl") {
    vs.reset(new VectorSumGSL());
}
else {
    std::cerr << "Unsupported implementation: " << implementation << std::endl;
    return 1;
}

// Compute the sum
std::vector<double> d;
vs->compute_sum(x, y, a, d);
```

This pattern ensures that the rest of the codebase treats both cases uniformly.
