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

Detector * Detector::Create(const char *binary_path, const char *haar_face_detector_location, const char *mtcnn_face_detector_location) {

  vector<string> arguments;
  arguments.push_back(string("  ")); // if CPP this is the application name
  arguments.push_back(string("-mloc"));
  arguments.push_back(string(binary_path));
  arguments.push_back(string("  "));
  arguments.push_back(string("-fdloc"));
  arguments.push_back(string(haar_face_detector_location));
  arguments.push_back(string("  "));
  arguments.push_back(string("-mtcnn_face_detector_location"));
  arguments.push_back(string(mtcnn_face_detector_location));

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

  

  // det_parameters.haar_face_detector_location = "/root/openface/lib/3rdParty/OpenCV/classifiers/haarcascade_frontalface_default.xml";
  // det_parameters.haar_face_detector_location = haar_face_detector_location;
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

cv::Mat_<double> Detector::RunInVideo(cv::Mat &grayscale_frame, const cv::Mat &rgb_frame){

  cv::Mat_<float> depth_image;
  bool success = LandmarkDetector::DetectLandmarksInVideo(rgb_frame, clnf_model_, det_parameters_, grayscale_frame);


  if (!success) {
    throw std::runtime_error("Unable to detect landmarks");
  }

  // std::cout << "success : " << success << std::endl;
  cv::Mat_<double> landmarks_2d = clnf_model_.detected_landmarks;
  landmarks_2d = landmarks_2d.reshape(1, 2);

  return landmarks_2d;
}

cv::Mat_<double> Detector::Run(cv::Mat &grayscale_frame, const cv::Mat &rgb_frame, const cv::Rect_<double> face_rect){

  cv::Mat_<float> depth_image;
  bool success = LandmarkDetector::DetectLandmarksInImage(rgb_frame, face_rect, clnf_model_, det_parameters_, grayscale_frame);


  if (!success) {
    throw std::runtime_error("Unable to detect landmarks");
  }

  std::cout << "success : " << success << std::endl;
  cv::Mat_<double> landmarks_2d = clnf_model_.detected_landmarks;
  landmarks_2d = landmarks_2d.reshape(1, 2);

  return landmarks_2d;
}

std::tuple<float, float> Detector::GetGaze(const cv::Mat &rgb_frame) {
  // // Gaze tracking, absolute gaze direction
  cv::Point3f gaze_direction_left(0, 0, -1);
  cv::Point3f gaze_direction_right(0, 0, -1);
  cv::Vec2f gaze_angle(0, 0);

  int image_width = rgb_frame.size().width;
  int image_height = rgb_frame.size().height;
  float cx = image_width / 2.0f;
  float cy = image_height / 2.0f;
  float fx = 500.0f * (image_width / 640.0f);
  float fy = 500.0f * (image_height / 480.0f);
  fx = (fx + fy) / 2.0f;
  fy = fx;
  // std::cout << "image width : " << image_width << std::endl;
	// std::cout << "image height : " << image_height << std::endl;
  // std::cout << "fx" << fx << std::endl;
	// std::cout << "fy" << fy << std::endl;
	// std::cout << "cx" << cx << std::endl;
	// std::cout << "cy" << cy << std::endl;
  if (clnf_model_.eye_model)
  {
    GazeAnalysis::EstimateGaze(clnf_model_, gaze_direction_left, fx, fy, cx, cy, true);
    GazeAnalysis::EstimateGaze(clnf_model_, gaze_direction_right, fx, fy, cx, cy, false);
    gaze_angle = GazeAnalysis::GetGazeAngle(gaze_direction_left, gaze_direction_right);
  }

  return std::tuple<float, float>(gaze_angle[0], gaze_angle[1]);
}
