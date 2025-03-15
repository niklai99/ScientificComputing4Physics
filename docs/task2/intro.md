# Introduction to Task 2

This document introduces Task 2, which demonstrates the implementation and testing of two numerical operations: a vector sum and a matrix multiplication. These operations are implemented in both Python and C++.

!!! info "Prerequisites"

    - Docker must be installed and an AlmaLinux9 container set up for Python and C++ development.  
    - Follow the Docker and container setup instructions in [Task 1](../task1/install_docker.md) before proceeding with Task 2.

In Task 2, you will find:

- A **Vector Sum** implementation that computes:  
 
    $$
    d = a \cdot x + y,
    $$

    where \(a\) is a scalar and \(x\) and \(y\) are vectors.

- A **Matrix Multiplication** implementation that computes:  
 
    $$
    C = A \, B, \quad \text{with} \quad C_{ij} = \sum_{k=1}^{N} a_{ik} \, b_{kj},
    $$

    where \(A\), \(B\), and \(C\) are \(N \times N\) matrices.

Each operation comes with separate test suites that measure execution time and validate correctness.  