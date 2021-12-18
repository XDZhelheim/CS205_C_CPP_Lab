import os
from tqdm import tqdm
import time

if __name__ == "__main__":
    image_count=0
    correct_count=0
    
    start=time.time()
    for folder in tqdm(os.listdir("../data/lfw/")):
        for image in os.listdir(f"../data/lfw/{folder}"):
            res=int(os.popen(f"face_detect -f ../data/lfw/{folder}/{image}").read()) # windows
            # res=int(os.popen(f"./face_detect -f ../data/lfw/{folder}/{image}").read()) # linux
            
            image_count+=1
            correct_count+=res
    
    end=time.time()
    
    print("Time cost:", end-start)
    print("Total # of images:", image_count)
    print("# of correctly classified images:", correct_count)
    print("Accuracy:", correct_count/image_count)
