#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "ForLoopSummator.hpp"
#include "GSLSummator.hpp"
#include "PairwiseSummator.hpp"
#include "KahanSummator.hpp"
#include "NeumaierSummator.hpp"


// Define the vector: [1.0, 1.0e16, -1.0e16, -0.5]
const std::vector<double> vec = {1.0, 1.0e16, -1.0e16, -0.5};
// The analytical sum is 0.5.
const double analytic = 0.5;

int main() {

    // Create instances of the summators.
    ForLoopSummator forLoopSum;
    GSLSummator gslSum;
    PairwiseSummator pairwiseSum;
    KahanSummator kahanSum;
    NeumaierSummator neumaierSum;

    // Compute the sums using the different algorithms.
    double sumForLoop = forLoopSum.sum(vec);
    double sumGSL = gslSum.sum(vec);
    double sumPairwise = pairwiseSum.sum(vec);
    double sumKahan = kahanSum.sum(vec);
    double sumNeumaier = neumaierSum.sum(vec);

    // Print the results.
    // Analytic solution
    std::cout << "Analytic solution: " << analytic << "\n\n";

    // Print the computed sums
    std::cout << "For loop summation:\n";
    std::cout << "Computed sum: " << sumForLoop << "\n\n";

    std::cout << "GSL summation:\n";
    std::cout << "Computed sum: " << sumGSL << "\n\n";

    std::cout << "Pairwise summation:\n";
    std::cout << "Computed sum: " << sumPairwise << "\n\n";

    std::cout << "Kahan summation:\n";
    std::cout << "Computed sum: " << sumKahan << "\n\n";

    std::cout << "Neumaier summation:\n";
    std::cout << "Computed sum: " << sumNeumaier << "\n\n";

    // Explanation of results
    std::cout <<
        "\nExplanation of results:\n"
        "- Forloop and GSL: Suffer from catastrophic cancellation due to the overwhelming effect of large numbers over small ones, resulting in a sum of -0.5.\n"
        "- Pairwise: Groups similar-magnitude numbers together, but in this case, it separates the contributions in a way that cancels out the large terms without preserving the small net offset, returning 0.\n"
        "- Kahan: Introduces a compensation variable, yet still fails to recover the lost precision when faced with catastrophic cancellation, also yielding -0.5.\n"
        "- Neumaier: Adjusts the compensation strategy to handle cases where the incoming term is larger than the current sum, successfully recovering the lost digits and producing the correct result, 0.5.\n";
    std::cout << std::endl;

    return 0;
}
