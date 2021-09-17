# CS205 Project 1 Report

11812804 董正

---

## 1 Introduction

### 1.1 Project Description

This project is to implement a calculator that can multiply any two integers.

Mainly, it has two functions:

1. Input validation
2. Big integer multiplication

### 1.2 Development Environment

* `Ubuntu 20.04.2 LTS x86_64` with `Linux 5.8.0-50-generic`
* `g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0`
* C++ standard: `c++11`

---

## 2 Design and Implementation

### 2.1 Input Validation

Macros and helper functions used in this section:

```c++
#define LF 10     // ASCII code for \n
#define SP 32     // ASCII code for space

bool check_valid(string input_str);
void read_input(string &multiplicand, string &multiplier);
```

* Check # of arguments by `argc`

  * `argc=3`

    Read multiplicand and multiplier from `argv[1], argv[2]`

  * `argc=1`

    Ask user to input two integers.

  * Others

    Print error message and ask user to re-input.

  **Implementation:**

  ```c++
  if (argc == 3) {
      multiplicand = argv[1];
      multiplier = argv[2];
  } else {
      if (argc != 1)
          cout << "Wrong number of arguments, please input two integers:" << endl;
      else
          cout << "Please input two integers:" << endl;
  
      read_input(multiplicand, multiplier);
  }
  ```

  **Test cases:**

  ![](./images/2-1-1.png)

* Check if input string is a number

  Use a regular expression to match numbers, including one leading `+` or `-`.

  Multiple leading zeros are allowed.

  **Implementation:**

  ```c++
  #include <regex>
  
  regex number_regexp("(\\+?|-)\\d+");
  
  bool check_valid(string input_str) {
      return regex_match(input_str, number_regexp);
  }
  
  int main(int argc, char const *argv[]) {
      //...
          
  	while (!(check_valid(multiplicand) && check_valid(multiplier))) {
          cout << "Invalid input, please input two integers:" << endl;
          read_input(multiplicand, multiplier);
      }
      
      //...
  }
  ```

  **Test cases:**

  ![](./images/2-1-2.png)

* Check # of input integers

  If more than two integers, ask user to re-input.

  Used when the program need to get input from `stdin`.

  **Implementation:**

  ```c++
  void read_input(string &multiplicand, string &multiplier) {
      cin >> multiplicand >> multiplier;
  
      char nextchar;
      while ((nextchar = getchar()) != LF) {
          if (nextchar != SP) {
              fflush(stdin);
              cout << "Wrong number of input, please input two integers:" << endl;
              cin >> multiplicand >> multiplier;
          }
      }
  }
  ```

  **Test cases:**

  

