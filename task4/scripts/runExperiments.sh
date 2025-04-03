#!/bin/bash
# run_experiments.sh
# This script runs both the C++ and Python integration codes for a grid of N values,
# and aggregates only the last output row (the CSV row) from each run into a master CSV file.

# Master results file.
RESULTS_FILE="./data/integrals_results.csv"

# Write CSV header.
echo "Language,N_AB,Trapz,Simpson,Romberg,Analytic" > ${RESULTS_FILE}

# Define a grid of N values (the number of points for data generation)
for N_exp in $(seq 3 24); do

    N=$((2**N_exp + 1))
    echo "Running experiments for N = ${N} ..."

    # Run the C++ executable.
    # The C++ code prints a CSV row as its last line.
    CPP_OUTPUT=$(run computeIntegral ${N} 0 3.141592653589793 | tail -n 1)

    # Run the Python script.
    # The Python code prints a CSV row as its last line.
    PY_OUTPUT=$(run compute_integral --N ${N} --precision 16 | tail -n 1)

    # Append only the last output row for each run to the master CSV file.
    echo "${CPP_OUTPUT}" >> ${RESULTS_FILE}
    echo "${PY_OUTPUT}" >> ${RESULTS_FILE}
done

echo "All results saved in ${RESULTS_FILE}"
