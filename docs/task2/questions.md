# Questions Regarding Task 2: Vector Sum and Matrix Multiplication

Below are the teacher's questions and my corresponding answers:

## Q1: Did you find any problems in running the codes for some N? If so, do you have an idea why?

**Answer:**

No, I did not encounter any problems running the codes for the tested values of N. Here’s why:

- **Python:**  
  NumPy handles large arrays by allocating memory on the heap, not on the stack. This means that even for very large values of N, the system's available RAM is the primary constraint rather than the limited stack size. Additionally, NumPy’s operations are implemented in highly optimized C code, so they run efficiently and blazingly fast even for large arrays.

- **C++:**  
  In the C++ implementations, I used `std::vector` to allocate memory dynamically on the heap. This avoids the stack overflow issues that would occur if large arrays were allocated on the stack. If I had allocated these arrays on the stack (for example, using standard C++ arrays), then for large N the program would fail due to exceeding the available stack size.

## Q2: Were you able to test correctly the sum and product for vector sum and matrix multiplication? If so, how? If not, what was the problem?

**Answer:**

Yes, I was able to test the computations correctly. I verified the results as follows:

- **Tolerance-Based Comparison:**  
  For both vector sum and matrix multiplication, I compared the computed results with the expected values using a tolerance-based approach. In Python, I used `np.allclose` to check that the computed results were within a small tolerance of the expected values. In C++, I used assertions with a tolerance check (using `std::fabs` to compare differences). 

- **Why Not Exact Equality?**  
  Directly testing for exact equality (using `==`) would fail due to the inherent rounding errors in floating-point arithmetic. Floating-point operations can introduce tiny discrepancies due to limited precision. The tolerance method ensures that these minor discrepancies do not cause the tests to fail, confirming that the computations are correct within an acceptable error margin.
