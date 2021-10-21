import numpy as np
import os
import matplotlib.pyplot as plt
import json


def rmse(predictions, targets):
    return np.sqrt(np.mean((predictions - targets)**2))


def plot_compare(x1,
                 y1,
                 x2,
                 y2,
                 label1,
                 label2,
                 title,
                 xlabel_name,
                 ylabel_name,
                 fig_path,
                 ylimit: tuple = None):
    if ylimit:
        plt.ylim(ylimit)
    plt.plot(x1, y1, "o-", label=label1)
    plt.plot(x2, y2, "o-", label=label2)
    plt.title(title)
    plt.xlabel(xlabel_name)
    plt.ylabel(ylabel_name)
    plt.legend()
    plt.savefig(fig_path)
    plt.clf()


if __name__ == "__main__":
    dims = ["32", "64", "128", "256", "512", "1024", "2048"]
    cases = ["C", "C++", "OpenBLAS", "O1", "O2", "O3"]
    read_time_dict = {}
    time_cost_dict = {}
    rmse_dict = {}

    for case in cases:
        if case == "C++":
            os.system(f"g++ matmul_{case}.cpp -o matmul_{case}")
        elif case == "OpenBLAS":
            os.system(
                "gcc -I D:\\OpenBLAS-0.3.18-x64\\include -L D:\\OpenBLAS-0.3.18-x64\\lib .\\matmul_OpenBLAS.c -lopenblas -o matmul_OpenBLAS"
            )
        elif case.startswith("O"):
            os.system(
                f"gcc -{case} matmul_C.c ../matrix.c ../matrix.h -o matmul_{case}"
            )
        else:
            os.system(
                f"gcc matmul_{case}.c ../matrix.c ../matrix.h -o matmul_{case}"
            )

        read_time_dict[case] = []
        time_cost_dict[case] = []
        rmse_dict[case] = []

        for dim in dims:
            A = np.loadtxt(f"../data/mat-A-{dim}.txt")
            B = np.loadtxt(f"../data/mat-B-{dim}.txt")
            C = np.matmul(A, B)

            cout = os.popen(
                f"matmul_{case} ../data/mat-A-{dim}.txt ../data/mat-B-{dim}.txt ./out-{case}-{dim}.txt"
            ).read()

            read_time = float(cout.split()[3][:-1])
            read_time_dict[case].append(read_time)

            time_cost = float(cout.split()[-1][:-1])
            time_cost_dict[case].append(time_cost)

            out = np.loadtxt(f"./out-{case}-{dim}.txt")
            rmse_dict[case].append(rmse(out, C))

            print(f"Finished {case}-{dim}")

    with open("./read_time.json", "w") as f:
        json.dump(read_time_dict, f)
    with open("./time_cost_dict.json", "w") as f:
        json.dump(time_cost_dict, f)
    with open("./rmse_dict.json", "w") as f:
        json.dump(rmse_dict, f)

    plot_compare(dims, read_time_dict["C"], dims, read_time_dict["C++"],
                 "C fscanf", "C++ ifstream", 
                 "C fscanf vs C++ ifstream: read file time", 
                 "Dimension",
                 "Time/s", 
                 "../images/av_read_time.png")
    plot_compare(dims, time_cost_dict["C"], dims, time_cost_dict["C++"],
                 "C array", "C++ vector", 
                 "C array vs C++ vector: time cost", 
                 "Dimension",
                 "Time/s", 
                 "../images/av_time_cost.png")
    plot_compare(dims, rmse_dict["C"], dims, rmse_dict["C++"], 
                 "C array", "C++ vector", 
                 "C array vs C++ vector: RMSE", 
                 "Dimension", 
                 "RMSE",
                 "../images/av_rmse.png")

    plot_compare(dims, time_cost_dict["C"], dims, time_cost_dict["OpenBLAS"],
                 "Strassen", "OpenBLAS", 
                 "Strassen vs OpenBLAS: time cost", 
                 "Dimension",
                 "Time/s", 
                 "../images/so_time_cost.png")
    plot_compare(dims, rmse_dict["C"], dims, rmse_dict["OpenBLAS"], 
                 "Strassen", "OpenBLAS", 
                 "Strassen vs OpenBLAS: RMSE", 
                 "Dimension", 
                 "RMSE",
                 "../images/so_rmse.png")

    plt.plot(dims, time_cost_dict["OpenBLAS"], "o-", label="OpenBLAS")
    plt.plot(dims, time_cost_dict["C"], "o-", label="gcc O0")
    plt.plot(dims, time_cost_dict["O1"], "o-", label="gcc O1")
    plt.plot(dims, time_cost_dict["O2"], "o-", label="gcc O2")
    plt.plot(dims, time_cost_dict["O3"], "o-", label="gcc O3")
    plt.title("OpenBLAS vs Strassen with gcc optimization: time cost")
    plt.xlabel("Dimension")
    plt.ylabel("Time/s")
    plt.legend()
    plt.savefig("../images/oo_time_cost.png")
    plt.clf()

    plt.plot(dims, rmse_dict["OpenBLAS"], "o-", label="OpenBLAS")
    plt.plot(dims, rmse_dict["C"], "o-", label="gcc O0")
    plt.plot(dims, rmse_dict["O1"], "o-", label="gcc O1")
    plt.plot(dims, rmse_dict["O2"], "o-", label="gcc O2")
    plt.plot(dims, rmse_dict["O3"], "o-", label="gcc O3")
    plt.title("OpenBLAS vs Strassen with gcc optimization: RMSE")
    plt.xlabel("Dimension")
    plt.ylabel("RMSE")
    plt.legend()
    plt.savefig("../images/oo_rmse.png")
    plt.clf()

    print("Finished.")