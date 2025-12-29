import pyopenface
import cv2
import os
# import time
# import pandas as pd

base_path = os.path.realpath(__file__)
base_path = base_path[:base_path.find('OpenFace')]

debug = True

def main():
    # print(base_path)
    # featureextraction = pyopenface.Featureextraction('/root/openface/lib/local/LandmarkDetector/model/main_clnf_general.txt')
    # img = cv2.imread("/root/openface/samples/sample3.jpg")
    # start = time.time()
    # for i in range(100) : 
    #     result = featureextraction.landmark(img)
    # print(time.time() - start)
    # detector = pyopenface.Detector(base_path + "OpenFace/lib/local/LandmarkDetector/model/main_clnf_general.txt")
    print("------------- DETECTOR init -----------------")
    detector = pyopenface.Detector("/usr/local/etc/OpenFace/model/main_ceclm_general.txt", 
                                   "/usr/local/etc/OpenFace/classifiers/haarcascade_frontalface_alt2.xml",
                                   "/usr/local/etc/OpenFace/model/mtcnn_detector/MTCNN_detector.txt")
    
    # gaze_angle_x = []
    # gaze_angle_y = []
    # cap = cv2.VideoCapture("../samples/test_gouget_cut.mp4")
    # while cap.isOpened() :
    #     ret, frame = cap.read()
    #     if not ret :
    #         print("error frame")
    #         break
    #     re = detector.landmarkinvideo(frame)
    #     gaze = detector.getgaze(frame)
    #     gaze_angle_x.append(gaze["gaze_angle_x"])
    #     gaze_angle_y.append(gaze["gaze_angle_y"])
    
    # df = pd.DataFrame(data={"gaze_angle_x" : gaze_angle_x, "gaze_angle_y" : gaze_angle_y})
    # df.to_csv("../analyse/test_gouget_cut_python.csv")
    
    print("------------- Load Image -----------------")
    img = cv2.imread("../samples/tesla.jpg")
    # print(img.dtype)
    # print("------------- DETECT -----------------")
    # face_rects_max = detector.detect(img)
    # print(face_rects_max)

    # print("------------- DETECT Landmark-----------------")
    
    # # times = []
    # # for i in range(100) :
    # #     start = time.time()
    re = detector.landmarkinvideo(img)
    face_keypoints = [(int(kp[0]), int(kp[1])) for kp in zip(re[0], re[1])]
    print(face_keypoints)
    
    img_landmark = img.copy()
    print("---- print landmark -----")
    for face_keypoint in face_keypoints : 
        img_landmark = cv2.circle(img_landmark, face_keypoint, radius=3, color=(0,0,255), thickness=-1)
    cv2.imwrite("../samples/processed/tesla_python.jpg", img_landmark)
        
        # print("------------- GAZE --------------")
    # gaze = detector.getgaze(img)
    #     print(gaze)
    #     times.append(time.time() - start)
    # print(sorted(times))


if __name__ == "__main__":
    main()
