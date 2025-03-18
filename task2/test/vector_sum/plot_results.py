import matplotlib.pyplot as plt
import numpy as np
import mplhep as hep

hep.style.use("CMS")

FONTSIZE = 18

# Vector sizes (N) used in the tests.
N = np.array([10, 1e6, 1e8])

# Python (NumPy) results
py_avg = np.array([0.000002, 0.000627, 0.114004])
py_rms = np.array([0.000003, 0.000085, 0.096845])

# C++ (no optimization) results
cpp_no_avg = np.array([9.207e-08, 0.00464462, 0.533384])
cpp_no_rms = np.array([4.34307e-08, 0.000816384, 0.177033])

# C++ (with optimization) results
cpp_opt_avg = np.array([4.919e-08, 0.000363343, 0.0341018])
cpp_opt_rms = np.array([6.65792e-08, 0.000176492, 0.00540988])


fig, ax = plt.subplots(figsize=(8, 6), constrained_layout=True)

ax.errorbar(N, py_avg,      yerr=py_rms,      marker="o", capsize=5, label="Python")
ax.errorbar(N, cpp_no_avg,  yerr=cpp_no_rms,  marker="o", capsize=5, label="C++ (no opt)")
ax.errorbar(N, cpp_opt_avg, yerr=cpp_opt_rms, marker="o", capsize=5, label="C++ (with opt)")

ax.set_xscale('log')
ax.set_yscale('log')
ax.tick_params(axis='both', which='major', labelsize=FONTSIZE-2)
ax.set_xlabel("Vector Size (N)", fontsize=FONTSIZE)
ax.set_ylabel("Average Time (seconds)", fontsize=FONTSIZE)

ax.legend(loc="upper left", fontsize=FONTSIZE-2)

plt.savefig("vector_sum_performance.png", dpi=300, bbox_inches="tight")

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
