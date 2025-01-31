import requests
import time
import tqdm

url = "http://127.0.0.1:8000/upload-photo/"
file_path = "../photomoi.jpg"  # Replace with the path to your photo

start = time.time()
for i in tqdm.tqdm(range(100)) :
    with open(file_path, "rb") as file:
        files = {"file": (file_path, file, "image/jpeg")}
        response = requests.post(url, files=files)

    # print(response.json())
print(time.time() - start)
# import os
# print(os.path.join("/tmp/openface/", 'test.txt'))