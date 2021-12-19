import os
import cv2
from tqdm import tqdm
import time

import torch
import torchvision.transforms as T
# print('PyTorch version: {}.'.format(torch.__version__))

from model import SimpleCLS
# init net
net = SimpleCLS(phase='test')
net = net.eval()
# load weight
state_dict = torch.load('./face_binary_cls.pth')
net.load_state_dict(state_dict)

# test
image_count = 0
correct_count = 0
    
start = time.time()
for folder in tqdm(os.listdir("../data/lfw/")):
    for image in os.listdir(f"../data/lfw/{folder}"):
        img = cv2.imread(f"../data/lfw/{folder}/{image}")
        img = cv2.resize(img, (128, 128))
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        with torch.no_grad():
            out = net(T.ToTensor()(img).unsqueeze(0))
            conf_scores = out.squeeze(0).data.numpy()
            
            image_count+=1
            if conf_scores[1] > conf_scores[0]:
                correct_count+=1
end = time.time()

print("Time cost:", end-start)
print("Total # of images:", image_count)
print("# of correctly classified images:", correct_count)
print("Accuracy:", correct_count / image_count)
