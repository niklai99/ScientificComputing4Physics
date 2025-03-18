import matplotlib.pyplot as plt
import numpy as np
import mplhep as hep

import matplotlib as mpl
from typing import Optional

hep.style.use("CMS")

def restore_minor_ticks_log_plot(
    ax: Optional[plt.Axes] = None, n_subticks=9
) -> None:
    """For axes with a logrithmic scale where the span (max-min) exceeds
    10 orders of magnitude, matplotlib will not set logarithmic minor ticks.
    If you don't like this, call this function to restore minor ticks.

    Args:
        ax:
        n_subticks: Number of Should be either 4 or 9.

    Returns:
        None
    """
    if ax is None:
        ax = plt.gca()

    locmaj = mpl.ticker.LogLocator(base=10, numticks=1000)
    ax.yaxis.set_major_locator(locmaj)
    locmin = mpl.ticker.LogLocator(
        base=10.0, subs=np.linspace(0, 1.0, n_subticks + 2)[1:-1], numticks=1000
    )
    ax.yaxis.set_minor_locator(locmin)
    ax.yaxis.set_minor_formatter(mpl.ticker.NullFormatter())

FONTSIZE = 18
PALETTE = [
    "#5790fc",		
    "#f89c20",		
    "#e42536",		
    "#964a8b",		
    "#9c9ca1",		
    "#7a21dd",
]

# Matrix N x N sizes used in the tests.
N_mat = np.array([10, 100, 10000])

# Lentgh of the vectors for data locality 
N_vec = N_mat**2 # matrices are square

# Python (NumPy) results
py_avg = np.array([1.49e-06, 6.31e-05, 9.293362])
py_rms = np.array([3.03e-06, 3.19e-04, 0.729940])

# C++ (no optimization) results
cpp_no_avg = np.array([3.29334e-06, 0.00281444])
cpp_no_rms = np.array([8.93811e-07, 0.000374047])

# C++ (with optimization) results
cpp_opt_avg = np.array([3.67979e-07, 0.000100926, 216.812])
cpp_opt_rms = np.array([3.14219e-07, 4.28775e-05, 0.961168])


fig, ax = plt.subplots(figsize=(8, 6), constrained_layout=True)

ax.errorbar(N_vec, py_avg,      yerr=py_rms,      marker="o", capsize=5, color=PALETTE[0], label="Python")
ax.errorbar(N_vec[:-1], cpp_no_avg,  yerr=cpp_no_rms,  marker="o", capsize=5, color=PALETTE[1], label="C++ (no opt)")
ax.errorbar(N_vec, cpp_opt_avg, yerr=cpp_opt_rms, marker="o", capsize=5, color=PALETTE[2], label="C++ (with opt)")

ax.set_xscale('log')
ax.set_yscale('log')
ax.tick_params(axis='both', which='major', labelsize=FONTSIZE-2)
ax.set_xlabel("Vector Size (NxN)", fontsize=FONTSIZE)
ax.set_ylabel("Average Time (seconds)", fontsize=FONTSIZE)

ax.legend(loc="upper left", fontsize=FONTSIZE-2)

restore_minor_ticks_log_plot(ax)

# grid on major ticks only on the y-axis
# ax.grid(True, which="major", axis="y", linestyle="-", alpha=0.1, color="black")

plt.savefig("matrix_mult_performance.png", dpi=300, bbox_inches="tight")

# plt.figure(figsize=(10, 6))

# # Plot Python results with symmetrical error bars (RMS).
# plt.errorbar(N, py_avg, yerr=py_rms, fmt='o-', capsize=5, label="Python (NumPy)")

# # Plot C++ (no optimization) results with symmetrical error bars.
# plt.errorbar(N, cpp_no_avg, yerr=cpp_no_rms, fmt='s-', capsize=5, label="C++ (no opt)")

# # Plot C++ (with optimization) results with symmetrical error bars.
# plt.errorbar(N, cpp_opt_avg, yerr=cpp_opt_rms, fmt='d-', capsize=5, label="C++ (with opt)")

# plt.xscale('log')
# plt.yscale('log')
# plt.xlabel("Vector Size (N)")
# plt.ylabel("Average Time (seconds)")
# plt.title("Vector Sum Performance Comparison")
# plt.legend()
# # plt.grid(True, which="both", linestyle="--")
# plt.tight_layout()
# plt.savefig("vector_sum_performance.png")
