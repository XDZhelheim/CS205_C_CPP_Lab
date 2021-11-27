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
    cycles_dict = {}
    
    for dim in dims:
        out = os.popen(
            f"perf stat -e r11 -x, -r 10 ../matmul.out ../data/mat-A-{dim}.txt ../data/mat-B-{dim}.txt ./out/out-{dim}.txt"
        ).read()
        
        cycles_dict[dim]=int(out.split(",")[0])

        print(f"Finished {dim}")

    with open("./rmse_dict.json", "w") as f:
        json.dump(cycles_dict, f)

    print("Finished.")