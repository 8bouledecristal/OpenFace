from fastapi import FastAPI, File, UploadFile, HTTPException
import uvicorn
import os
import subprocess
import csv
import json

app = FastAPI()

# Ensure the /tmp/openface directory exists
os.makedirs("/tmp/openface", exist_ok=True)

@app.post("/upload-photo/")
async def upload_photo(file: UploadFile = File(...)):
    
    # Define the path where the file will be saved
    file_path = os.path.join("/tmp/openface", file.filename.split("/")[-1])
    
    try:
        # Save the file to the specified path
        with open(file_path, "wb") as buffer:
            buffer.write(await file.read())
        
        # Check if the file was saved correctly
        if not(os.path.exists(file_path)):
            # return {"message": f"Photo '{file.filename}' was correctly downloaded and saved to /tmp/openface."}
            raise HTTPException(status_code=500, detail="Failed to save the photo.")
        
        # TODO faire le subprocess, importer pandas et renvoyer la réponse sous forme de json ou csv c'est mpoins lourd
        command = ["FeatureExtraction", "-gaze", "-f", file_path, "-of", "/tmp/openface/output.csv"]
        # command = f"FeatureExtraction -gaze -f {file_path} -of /tmp/openface/output.csv"
        print(command)
        subprocess.check_call(command)
        print("ok")
        
        csv_path = "/tmp/openface/output.csv"

        # Read the CSV file
        with open(csv_path, 'r') as csvfile:
            reader = csv.DictReader(
                csvfile, delimiter=',', skipinitialspace=True)

            # Filter out the rows that have no gaze data
            out = [{"face_id": row["face_id"],
                    "confidence": row["confidence"],
                    "gaze_angle_x": row["gaze_angle_x"],
                    "gaze_angle_y": row["gaze_angle_y"]
                    }
                   for row in reader]
        
            print(out)

            # use gaze_data = list(reader) to get all the rows
            gaze_data = list(out)

        # Convert the gaze data to JSON string
        gaze_data_json = json.loads(json.dumps(gaze_data))
        return gaze_data_json
        
        
        # return f"FeatureExtraction -gaze -f {file_path} -of /tmp/openface/output.csv"
        
    except Exception as e:
        print(e)
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)


# @app.post("/")
# def post_image(image: Image):
#     if (Utils.is_image_url(image.image_url)):
#         image_name = Utils.download_image(image.image_url)

#         # If image could not be downloaded
#         if not image_name:
#             Utils.delete_files(image_name)
#             raise HTTPException(
#                 status_code=status.HTTP_400_BAD_REQUEST,
#                 detail="Unreachable image resource",
#             )

#         # If image was downloaded successfully run faceLandmarkImg
#         if Utils.face_landmark_img(image_name):
#             if Utils.is_face_found(image_name):
#                 json_data = Utils.get_gaze_data(image_name)
#                 # If DEBUG is false, delete the image
                
#                 Utils.delete_files(image_name)
#                 return json_data

#         # If no face was found delete the image and return error
#         Utils.delete_files(image_name)
        
#         raise HTTPException(
#             status_code=status.HTTP_406_NOT_ACCEPTABLE,
#             detail="No face was found",
#         )

#     else:
#         raise HTTPException(
#             status_code=status.HTTP_400_BAD_REQUEST,
#             detail="Invalid image url",
#         )


# import cv2
# import time
# import subprocess
# import pandas as pd
# import numpy as np
# import matplotlib.pyplot as plt

# # ffmpeg -re -i ../test1.mp4 -map 0:v -vf format=yuv420p -f v4l2 /dev/video0
# # ffmpeg -stream_loop 1 -re -i ../pie/test1.mp4 -map 0:v -vf format=yuv420p -f v4l2 /dev/video0

# square_size = 300  # Side length of the square
# center_x = square_size // 2
# center_y = square_size // 2
# max_gaze_angle = 1

# def draw_gaze_square(frame, gaze_angle_x, gaze_angle_y):
#     """
#     Draw a square with a dot corresponding to gaze_angle_x and gaze_angle_y.
#     The gaze direction is normalized to the size of the square.
#     """
#     # Normalize gaze angles to be within the square's dimensions
#     dot_x = int(center_x + (gaze_angle_x / max_gaze_angle) * 2*(square_size))
#     dot_y = int(center_y - (gaze_angle_y / max_gaze_angle) * 2*(square_size))  # Invert y-axis for screen coordinates
    
#     # Draw the square (centered in the frame)
#     cv2.rectangle(frame, (center_x - square_size // 2, center_y - square_size // 2),
#                   (center_x + square_size // 2, center_y + square_size // 2),
#                   (255, 255, 255), 2)  # White square outline
    
#     # Draw the gaze direction as a red dot inside the square
#     cv2.circle(frame, (dot_x, dot_y), 8, (0, 0, 255), -1)  # Red dot for gaze direction
    
#     return frame

# frame_path = "./data/frame.jpg"
# output_path = "./data/tesla/tesla.csv"

# # docker_command = [
# #     "docker", "run", "--rm",
# #     "-v", f"{frame_path}:/input/frame.jpg",  # Map the frame into the container
# #     "-v", f"{output_path}:/output/output.csv",  # Map the output file from the container
# #     "openface-image",  # Name of the OpenFace Docker image
# #     "./FeatureExtraction",  # Command to execute inside the container
# #     "-f", "/input/frame.jpg",  # Input frame path inside the container
# #     "-of", "/output/output.csv"  # Output file path inside the container
# # ]

# W=160
# H=120
# cap = cv2.VideoCapture(0)

# # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M','J','P','G'))
# cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('Y','U','Y','V'))
# # cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('U','H','C','V'))
# cap.set(cv2.CAP_PROP_FRAME_WIDTH, W)
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, H)
# cap.set(cv2.CAP_PROP_FPS, 30)

# while True:
#     ret, frame = cap.read()
#     if not ret:
#         continue
#     frame = cv2.resize(frame, (640, 480))
    
#     cv2.imwrite(frame_path, frame)
#     docker_command = [
#         "docker", "exec", "-itd", "openface",
#         "FaceLandmarkImg", "-gaze",
#         "-f", "/tmp/openface/frame.jpg",
#         "-of", "/tmp/openface/output.csv"
#     ]
#     subprocess.run(docker_command, check=True)
#     # print(f"Output saved to {output_path}")
    
#     data = pd.read_csv("./data/output.csv")
#     thetax = data.iloc[0,8]
#     thetay = data.iloc[0,9]
#     print(thetax, thetay)
#     frame = draw_gaze_square(frame, thetax, thetay)
    
#     cv2.imshow('usb cam test', frame)
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break
    

# cap.release()
# cv2.destroyAllWindows()
