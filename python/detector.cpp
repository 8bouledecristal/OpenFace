#include "detector.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <LandmarkCoreIncludes.h>
#include <dlib/image_processing/frontal_face_detector.h>

using std::cout;
using std::string;
using std::vector;

Detector * Detector::Create(const char *binary_path) {

  vector<string> arguments;
  arguments.push_back(string("  ")); // if CPP this is the application name
  arguments.push_back(string("-mloc"));
  arguments.push_back(string(binary_path));

  LandmarkDetector::FaceModelParameters det_parameters(arguments);
  std::cout << "model location " << det_parameters.model_location << std::endl;
  // det_parameters.track_gaze = false;
  // // No need to validate detections, as we're not doing tracking.
  // det_parameters.validate_detections = false;

  // // Grab camera parameters, if they are not defined 
  // // (approximate values will be used).
  // float fx = 0, fy = 0, cx = 0, cy = 0;
  // int device = -1;
  // // Get camera parameters
  // LandmarkDetector::get_camera_params(device, fx, fy, cx, cy, arguments);
  // // If cx (optical axis centre) is undefined will use the image size/2 as 
  // // an estimate.
  // bool cx_undefined = false;
  // bool fx_undefined = false;
  // if (cx == 0 || cy == 0) {
  //   cx_undefined = true;
  // }
  // if (fx == 0 || fy == 0) {
  //   fx_undefined = true;
  // }

  // The modules that are being used for tracking.
  LandmarkDetector::CLNF clnf_model(det_parameters.model_location);

  if (!clnf_model.loaded_successfully)
	{
		std::cout << "ERROR: Could not load the landmark detector" << std::endl;
    throw ;
	}

  

  det_parameters.haar_face_detector_location = "/root/openface/lib/3rdParty/OpenCV/classifiers/haarcascade_frontalface_default.xml";
  cv::CascadeClassifier classifier(det_parameters.haar_face_detector_location);
  
  dlib::frontal_face_detector face_detector_hog = dlib::get_frontal_face_detector();
  std::cout << "ouin" << std::endl ; 
  LandmarkDetector::FaceDetectorMTCNN face_detector_mtcnn(det_parameters.mtcnn_face_detector_location);
  return new Detector(det_parameters, clnf_model, classifier, face_detector_hog, face_detector_mtcnn);
}

Detector::Detector(LandmarkDetector::FaceModelParameters &det_parameters,
                   LandmarkDetector::CLNF &clnf_model,
                   cv::CascadeClassifier &classifier,
                   dlib::frontal_face_detector &face_detector_hog,
                  LandmarkDetector::FaceDetectorMTCNN &face_detector_mtcnn) : det_parameters_(std::move(det_parameters)), clnf_model_(std::move(clnf_model)),
                                                                     classifier_(std::move(classifier)),
                                                                     face_detector_hog_(std::move(face_detector_hog)),
                                                                     face_detector_mtcnn_(std::move(face_detector_mtcnn)) {}

bool CompareRect(cv::Rect_<double> r1, cv::Rect_<double> r2) {

  return r1.height < r2.height;

}

cv::Rect_<double> Detector::DetectFace(const cv::Mat &grayscale_frame, const cv::Mat &rgb_frame) {

  vector<cv::Rect_<float> > face_detections;
  vector<float> confidences;

  if (det_parameters_.curr_face_detector == LandmarkDetector::FaceModelParameters::HOG_SVM_DETECTOR)
  {
    std::vector<float> confidences;
    LandmarkDetector::DetectFacesHOG(face_detections, grayscale_frame, face_detector_hog_, confidences);
  }
  else if (det_parameters_.curr_face_detector == LandmarkDetector::FaceModelParameters::HAAR_DETECTOR)
  {
    LandmarkDetector::DetectFaces(face_detections, grayscale_frame, classifier_);
  }
  else
  {
    std::vector<float> confidences;
    LandmarkDetector::DetectFacesMTCNN(face_detections, rgb_frame, face_detector_mtcnn_, confidences);
  }
  
  // Finding the biggest face among the detected ones.
  // cout << "  Find biggest face" << std::endl;
  if (face_detections.empty()) {
      std::cout<< " No faces detected " << std::endl;
    //throw std::invalid_argument("No faces detected");
  }
  cv::Rect_<double> face_l = *max_element( face_detections.begin(), face_detections.end(), CompareRect);

  // Return the biggest face.
  return face_l;

}

// cv::Mat_<int> Detector::GetVisibilities() {
//   int idx = clnf_model_.patch_experts.GetViewIdx(clnf_model_.params_global, 0);
//   return clnf_model_.patch_experts.visibilities[0][idx];
// }

cv::Mat_<double> Detector::Run(cv::Mat &grayscale_frame, const cv::Mat &rgb_frame, const cv::Rect_<double> face_rect){

  cv::Mat_<float> depth_image;
  bool success = LandmarkDetector::DetectLandmarksInImage(rgb_frame, face_rect, clnf_model_, det_parameters_, grayscale_frame);


  if (!success) {
    throw std::runtime_error("Unable to detect landmarks");
  }

  // Gaze tracking, absolute gaze direction
  cv::Point3f gaze_direction0(0, 0, -1);
  cv::Point3f gaze_direction1(0, 0, -1);
  cv::Vec2f gaze_angle(0, 0);

  if (clnf_model_.eye_model)
  {
    GazeAnalysis::EstimateGaze(clnf_model_, gaze_direction0, 618.359f, 618.359f, 279.5f, 384.0f, true);
    GazeAnalysis::EstimateGaze(clnf_model_, gaze_direction1, 618.359f, 618.359f, 279.5f, 384.0f, false);
    gaze_angle = GazeAnalysis::GetGazeAngle(gaze_direction0, gaze_direction1);
  }
  std::cout << "gaze angle 0 : " << gaze_angle[0] << std::endl;
  std::cout << "gaze angle 1 : " << gaze_angle[1] << std::endl;

  std::cout << "success : " << success << std::endl;
  cv::Mat_<double> landmarks_2d = clnf_model_.detected_landmarks;
  landmarks_2d = landmarks_2d.reshape(1, 2);

  return landmarks_2d;

}
