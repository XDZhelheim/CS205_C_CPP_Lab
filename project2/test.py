import numpy as np
import os
import time

def rmse(predictions, targets):
    return np.sqrt(np.mean((predictions-targets)**2))

if __name__ == "__main__":
    A_path="./mat-A-2048.txt"
    B_path="./mat-B-2048.txt"
    C_path="./out2048.txt"

    A=np.loadtxt(A_path)
    B=np.loadtxt(B_path)

    start=time.time()
    C=np.matmul(A, B)
    end=time.time()
    print("numpy multiplication time: {:.3f}s".format(end-start))

    os.system("g++ matmul.cpp -o matmul")
    os.system(f"matmul {A_path} {B_path} {C_path}")

    cpp_res=np.loadtxt(C_path)

    print()
    print("RMSE =", rmse(cpp_res, C))