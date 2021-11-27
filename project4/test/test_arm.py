import os

if __name__ == "__main__":
    dims = ["32", "64", "128", "256", "512", "1024", "2048"]

    for dim in dims:
        os.system(
            f"perf stat -e r11 -x, -r 10 ../matmul.out ../data/mat-A-{dim}.txt ../data/mat-B-{dim}.txt ./out/out-{dim}.txt 2>>res_arm.csv"
        )

        print(f"Finished {dim}")

    print("Finished.")