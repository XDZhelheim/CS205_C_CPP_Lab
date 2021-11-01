import numpy as np

if __name__ == "__main__":
    np.random.seed(666)
    mat1=np.random.rand(10000, 1000).astype(np.float32)*100
    np.random.seed(888)
    mat2=np.random.rand(10000, 1000).astype(np.float32)*100

    np.savetxt("./mat1.txt", mat1, fmt="%f")
    np.savetxt("./mat2.txt", mat2, fmt="%f")