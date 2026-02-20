# Pricing Rainbow Options via Monte Carlo & Variance Reduction

This project was developed as part of the **Computational and Quantitative Methods in C++** course of the **M.Sc. in FinTech** (Financial Technology) at the **University of Piraeus**.

This repository contains C++ implementations for pricing **European Rainbow Options (Call on min)** using Monte Carlo simulation. It explores the efficiency and accuracy of different **Variance Reduction Techniques** compared to the baseline Crude Monte Carlo method.

## Project Overview
Rainbow options are exotic derivatives whose payoff depends on the performance of multiple underlying assets. This project specifically models a **Call on the Minimum** of two uncorrelated assets ($S_1, S_2$) following Geometric Brownian Motion (GBM).

### Payoff Structure
The payoff at maturity $T$ is defined as:
$$C = \max(\min(S_{1,T}, S_{2,T}) - K, 0)$$

## Methods Implemented
1. **Crude Monte Carlo**: The baseline numerical estimation.
2. **Antithetic Variates (AV)**: Reduces variance by pairing each random draw with its negative counterpart, exploiting the symmetry of the normal distribution.
3. **Control Variates (CV)**: Enhances precision by using the underlying asset prices as "controls," since their expected values are known analytically. A **pilot simulation** is used to estimate the optimal control coefficients ($c^*$).

## Performance Comparison
Based on $4 \times 10^6$ simulations with parameters $S_1=S_2=100, K=100, r=0.05, \sigma=0.2, T=1$:

| Method | Price Estimate | Relative Error (95%) | Runtime (s) |
| :--- | :--- | :--- | :--- |
| **Crude MC** | 3.2973 | 0.002081 | ~3.15 |
| **Antithetic Variates** | 3.2970 | 0.001299 | ~2.40 |
| **Control Variates** | 3.2956 | 0.001530 | ~1.55 |

*Note: Antithetic Variates provided the highest precision due to the monotonic nature of the payoff.*

## Structure
* `agianoglou simple monte carlo method.cpp`: Baseline implementation.
* `agianoglou antithetic variates method.cpp`: Implementation with AV.
* `agianoglou control variates method.cpp`: Implementation with CV and Pilot Run.
* `Report for Monte Carlo.pdf`: Detailed theoretical analysis and documentation.
* `Presentation for Monte Carlo.pdf`: Project presentation slides.

## Authors
* **Agianoglou Nikolaos-Vasileios**
* **DelRosso Vasileios**
* **Lathiotakis Efthymios**
* **Pattas Panagiotis**

*Instructor: Englezos Nikolaos - University of Piraeus (M.Sc. in FinTech)*
