import requests
import time
import tqdm
import cv2

url = "http://10.70.1.211:8000/upload-photo/"
file_path = "../photomoi.jpg"  # Replace with the path to your photo
video_path = "path/to/your/video.mp4"
# start = time.time()
# for i in tqdm.tqdm(range(100)) :
with open(file_path, "rb") as file:
    files = {"file": (file_path, file, "image/jpeg")}
    response = requests.post(url, files=files)

    print(response)
# print(time.time() - start)
# import os
# print(os.path.join("/tmp/openface/", 'test.txt'))