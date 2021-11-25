# CS205 Project 4 Report

11812804 董正

---

[TOC]

---

## 1 Introduction

The documentation of class `matrix` is hosted on [my GitHub](https://xdzhelheim.github.io/CS205_C_CPP_Lab/classmatrix.html).

Alternatively, view this report online [here](https://xdzhelheim.github.io/CS205_C_CPP_Lab/md__r_e_a_d_m_e.html).

### 1.1 Project Description

This project is to design a class for matrices.

1. The class should contain the data of a matrix and related information.
2. The class support different data types.
3. Do not use memory hard copy if a matrix object is assigned to another.
4. Implement some frequently used operators.
5. Implement region of interest (ROI) to avoid memory hard copy.
6. Test the program on X86 and ARM platforms, and describe the differences.

### 1.2 Development Environment

* x86_64

  * Windows 10 Home China x86_64

  * Kernel version `10.0.19042`
  * `Intel i5-9300H (8) @ 2.400GHz`

  * `g++.exe (tdm64-1) 10.3.0`

  * C++ standard: `c++11`

* ARM64

  * `macOS 12.0.1 21A559 arm64`
  * Darwin Kernel Version `21.1.0`
  * Apple M1 Pro (10-cores)
  * `Apple clang version 13.0.0 (clang-1300.0.29.3)`
  * C++ standard: `c++11`

## 2 Design and Implementation



