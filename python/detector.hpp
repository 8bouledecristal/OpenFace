#pragma once

#include <memory>
#include <string>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <LandmarkCoreIncludes.h>
#include <GazeEstimation.h>
#include <tuple>


using std::unique_ptr;

class Detector {

public:
  static Detector * Create(const char *binary_path);
  cv::Mat_<double> Run(cv::Mat &grayscale_frame, const cv::Mat &rgb_frame, const cv::Rect_<double> face_rect);
  cv::Mat_<double> RunInVideo(cv::Mat &grayscale_frame, const cv::Mat &rgb_frame);
  cv::Rect_<double> DetectFace(const cv::Mat &grayscale_frame, const cv::Mat &rgb_frame);
  std::tuple<float, float> GetGaze(const cv::Mat &rgb_frame);

  // cv::Mat_<uchar> grayscale_frame_;

private:
  Detector(LandmarkDetector::FaceModelParameters &det_parameters,
           LandmarkDetector::CLNF &clnf_model,
           cv::CascadeClassifier &classifier,
           dlib::frontal_face_detector &face_detector_hog,
          LandmarkDetector::FaceDetectorMTCNN &face_detector_mtcnn);

  // cv::Mat_<int> GetVisibilities();

  LandmarkDetector::FaceModelParameters det_parameters_;
  LandmarkDetector::CLNF clnf_model_;
  cv::CascadeClassifier classifier_;
  dlib::frontal_face_detector face_detector_hog_;
  LandmarkDetector::FaceDetectorMTCNN face_detector_mtcnn_;
};
