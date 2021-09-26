import numpy as np

if __name__ == "__main__":
    A=np.loadtxt("./mat-A-2048.txt")
    B=np.loadtxt("./mat-B-2048.txt")

    dims=[64, 128, 512, 1024]

    for dim in dims:
        np.savetxt(f"./mat-A-{dim}.txt", A[:dim, :dim], fmt="%.1f")
        np.savetxt(f"./mat-B-{dim}.txt", A[:dim, :dim], fmt="%.1f")
