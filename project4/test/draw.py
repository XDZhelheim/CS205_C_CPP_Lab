import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


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
    
dims = ["32", "64", "128", "256", "512", "1024", "2048"]
    
if __name__ == "__main__":
    res_x86=pd.read_csv("./res_x86.csv", names=["val", "1", "2", "3", "4", "5", "6", "7"])
    res_arm=pd.read_csv("./res_arm.csv", names=["val", "1", "2", "3", "4", "5", "6", "7"])
    
    # plot_compare(dims, res_x86["val"], dims, res_arm["val"],
    #              "x86", "Arm", 
    #              "x86 vs Arm: CPU Cycles", 
    #              "Dimension",
    #              "Cycles", 
    #              "../images/cycles.png")
    
    x86_sum=res_x86["val"].sum()
    arm_sum=res_arm["val"].sum()
    
    print(arm_sum/x86_sum)