---
hide:
  - navigation
  - footer
---

# Scientific Computing for Physics Students

This page serves as a central hub for all homework assignments (tasks) given throughout the course. Each task can be accessed from the **[Task Section](#homework--task-overview)** below, or from the navigation bar at the top-left of the webpage, beneath the course title. Within each task, you will find multiple pages dedicated to different sub-tasks, each providing a step-by-step explanation and the rationale behind every choice.

!!! warning
    This webpage is actively maintained and updated as the course progresses. **Currently, only Task 1 is fully documented.** More tasks will appear in the future.


## Contact Information

- **Teacher:** Prof. Alessandro Renzi
    - **Email:** alessandro.renzi@unipd.it

- **Student Contact (author of this document):**
    - **Name:** Nicolò Lai  
    - **Email:** nicolo.lai@phd.unipd.it


## Syllabus

### Target Skills and Knowledge
Upon successful completion of this course, students will be able to:

1. Effectively handle and combine scientific computing tools.
2. Independently research and gather necessary information for coding projects.
3. Design and develop code tailored to specific research needs.
4. Critically evaluate and efficiently use the most suitable tools for given tasks, balancing ease of use and optimization.
5. Demonstrate a fundamental understanding of computer science concepts, enabling autonomous further learning as needed.
6. Apply programming skills specifically oriented towards scientific applications.
7. Analyze and solve complex scientific problems using computational methods.
8. Articulate the trade-offs between different computational approaches in scientific contexts.


### Examination Methods
An oral examination where students present and defend multiple micro-projects developed throughout the course, demonstrating their understanding and application of key concepts.

### Assessment Criteria
Critically analyze and evaluate coding micro-projects presented during the course, identifying potential pitfalls and proposing effective design strategies to overcome them.

### Course Unit Contents
Coding forms the core of scientific computing, embodying a dual nature: one deeply rooted in established practices, and another driven by constant innovation. Throughout this course, we will strive to strike a balance between these two aspects.

#### Topics

1. **Linux as a Development Environment**  
      - Introducing CloudVeneto  
      - Setting Up the Development Environment (in CloudVeneto)  
      - Managing distros (apt and yum)  
      - Shell scripting in Bash  
      - Remote development (SSH)

2. **Overview of Hardware Architecture**  
      - CPU, memory, and storage  
      - Data representation (binary, hexadecimal)  
      - Processes, threads, and multitasking  
      - Floating-point representations

3. **Introduction to Programming Concepts**  
      - Procedural vs. interpreted languages  
      - Basics of compiling vs. interpreting  
      - Object-Oriented Programming

4. **Data Structures**  
      - Arrays, matrices, lists, stacks, queues, trees, heaps, hashing, graphs  
      - Saving data (txt, binary, HDF5, etc.)

5. **Numerical Methods and Algorithms**  
      - Big-O notation  
      - Matrix inversion (SVD, Cholesky)  
      - Root finding, interpolation, numerical integration, ODE, autodiff  
      - Libraries for numerical computing

6. **Software Engineering for Scientists**  
      - Version control (Git and GitHub)  
      - Debugging and compilation flags  
      - Using AI tools (ChatGPT, GitHub Copilot)  
      - Data visualization (Matplotlib, Plotly, gnuplot)  
      - Documentation and CI principles

7. **Fast Fourier Transforms (FFTs)**  
      - Discrete Fourier Transform  
      - FFT algorithm

8. **Introduction to Monte Carlo Techniques**  
      - Random number generators  
      - Monte Carlo integration and simulation

9. **Introduction to Machine Learning for Physicists**  
      - ML concepts and libraries  
      - Building a simple neural network

10. **Optimization Techniques**  
        - Compiler optimizations, memory layout  
        - Benchmarking and profiling

11. **Parallel and High-Performance Computing**  
        - MPI, OpenMP, and cluster environments (SLURM, HTCondor)  
        - HPC software (Spark, modules)

12. **GPU Programming**  
        - GPU architecture and rationale  
        - Writing your first GPU kernel  
        - Effective use of GPU libraries


### Additional Notes about Suggested Reading
- We will provide reputable resources throughout the course, ranging from foundational materials to current literature.
- You can find recommended books and references for both C/C++ and Python programming, HPC, and AI below.

**Programming with C (including parallel computing)**  
- [Effective C (2nd Edition)](https://nostarch.com/effective-c-2nd-edition)  
- [CompSciForPhys](https://github.com/tgmattso/CompSciForPhys)  
- [ParProgForPhys](https://github.com/tgmattso/ParProgForPhys)  
- [SciCompHPC](https://github.com/tgmattso/SciCompHPC/tree/main)

**Programming with Python**  
- [Learn Python](https://www.learnpython.org)  
- [Introduction to Python for Scientists](https://nostarch.com/python-tools-scientists)

**Miscellaneous**  
- [CloudVeneto UserGuide](https://userguide.cloudveneto.it/en/latest/)  
- [Julia Book: Practical Julia](https://nostarch.com/practical-julia)  
- [Quantum computing (book)](https://nostarch.com/quantum-computingv)  

---

## Homework / Task Overview <a name="homework--task-overview"></a>

Below are links to the tasks (micro-projects) completed throughout the course:

- **Task 1:** 
    - [Docker Installation and AlmaLinux9 setup](task1/install_docker.md)  
    - [AlmaLinux9 setup for Python and C/C++ development](task1/custom_docker.md)  
    - [Testing the AlmaLinux9 container with Python and C++ code](task1/test_py_cpp.md)

Future tasks will be added here as the course progresses. 
