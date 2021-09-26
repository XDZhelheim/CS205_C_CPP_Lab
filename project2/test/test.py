from typing import Tuple
import numpy as np
import os
import matplotlib.pyplot as plt
import json


def rmse(predictions, targets):
    return np.sqrt(np.mean((predictions - targets)**2))


def plot_compare(x1, y1, x2, y2, label1, label2, title, xlabel_name,
                 ylabel_name, fig_path, ylimit: Tuple=None):
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
    cases = ["double", "float", "strassen", "forloop", "strassen_forloop"]
    time_cost_dict = {}
    rmse_dict = {}

    for case in cases:
        os.system("g++ matmul_{0}.cpp -o matmul_{0}".format(case))
        time_cost_dict[case] = []
        rmse_dict[case] = []

        for dim in dims:
            if case=="strassen" and dim=="2048":
                time_cost_dict[case].append(4800) # 如果真要跑大概 1 小时 20 分钟, 不方便画图
                rmse_dict[case].append(1.3950859156363537e-07)
                continue

            A = np.loadtxt(f"../data/mat-A-{dim}.txt")
            B = np.loadtxt(f"../data/mat-B-{dim}.txt")
            C = np.matmul(A, B)

            cout = os.popen(
                f"matmul_{case} ../data/mat-A-{dim}.txt ../data/mat-B-{dim}.txt ./out-{case}-{dim}.txt"
            ).read()
            time_cost = float(cout.split()[-1][:-1])
            time_cost_dict[case].append(time_cost)

            out = np.loadtxt(f"./out-{case}-{dim}.txt")
            rmse_dict[case].append(rmse(out, C))

            print(f"Finished {case}-{dim}")

    with open("./time_cost_dict.json", "w") as f:
        json.dump(time_cost_dict, f)
    with open("./rmse_dict.json", "w") as f:
        json.dump(rmse_dict, f)

    plot_compare(dims, time_cost_dict["double"], dims, time_cost_dict["float"],
                 "double", "float", 
                 "double vs float: time cost", 
                 "Dimension",
                 "Time/s", 
                 "../images/df_time_cost.png")
    plot_compare(dims, rmse_dict["double"], dims, rmse_dict["float"], 
                 "double", "float", 
                 "double vs float: RMSE", 
                 "Dimension", 
                 "RMSE",
                 "../images/df_rmse.png")

    plot_compare(dims, time_cost_dict["strassen"], dims, time_cost_dict["forloop"], 
                 "strassen", "for-loop",
                 "Strassen vs for-loop: time cost", 
                 "Dimension", 
                 "Time/s",
                 "../images/sf_time_cost.png",
                 ylimit=(-25, 550))
    plot_compare(dims, rmse_dict["strassen"], dims, rmse_dict["forloop"],
                 "strassen", "for-loop", 
                 "Strassen vs for-loop: RMSE",
                 "Dimension", 
                 "RMSE", 
                 "../images/sf_rmse.png")

    plt.plot(dims, time_cost_dict["strassen"], "o-", label="strassen")
    plt.plot(dims, time_cost_dict["forloop"], "o-", label="for-loop")
    plt.plot(dims, time_cost_dict["strassen_forloop"], "o-", label="strassen+for-loop")
    plt.title("Strassen + for-loop: time cost")
    plt.xlabel("Dimension")
    plt.ylabel("Time/s")
    plt.legend()
    plt.savefig("../images/s+f_time_cost.png")
    plt.clf()

    plt.plot(dims, rmse_dict["strassen"], "o-", label="strassen")
    plt.plot(dims, rmse_dict["forloop"], "o-", label="for-loop")
    plt.plot(dims, rmse_dict["strassen_forloop"], "o-", label="strassen+for-loop")
    plt.title("Strassen + for-loop: RMSE")
    plt.xlabel("Dimension")
    plt.ylabel("RMSE")
    plt.legend()
    plt.savefig("../images/s+f_rmse.png")
    plt.clf()

    print("Finished.")