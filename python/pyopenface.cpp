#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <iostream>
#include <boost/python.hpp>
#include <numpy/arrayobject.h>
#include <opencv2/core.hpp>
// #include "detector.hpp"
#include "featureextraction.hpp"
// #include "tracker.hpp"

namespace bp = boost::python;



// bp::object landmark(Detector *detector, bp::object frame_obj, const bp::list& rect_object)
// {

//   PyArrayObject* frame_arr = reinterpret_cast<PyArrayObject*>(frame_obj.ptr());
//   const int height = PyArray_DIMS(frame_arr)[0];
//   const int width = PyArray_DIMS(frame_arr)[1];
//   cv::Mat frame(cv::Size(width, height), 
// 		  CV_8UC1, 
// 		  PyArray_DATA(frame_arr), 
// 		  cv::Mat::AUTO_STEP);

//   //rect_object[left, top, right, bottom]
//   bp::ssize_t rect_len = bp::len(rect_object);
//   assert(rect_len==4);
//   double rect_x = bp::extract<double>(rect_object[0]);
//   double rect_y = bp::extract<double>(rect_object[1]);
//   double rect_height = bp::extract<double>(rect_object[2]) - bp::extract<double>(rect_object[0]);
//   double rect_width  = bp::extract<double>(rect_object[3]) - bp::extract<double>(rect_object[1]);
//   cv::Rect_<double> face_rect( rect_x, 
// 		  rect_y, 
// 		  rect_height, 
// 		  rect_width);

//   cv::Mat_<double> face_landmarks = detector->Run(frame, face_rect);

//   long int face_landmarks_size[2] = {face_landmarks.rows, face_landmarks.cols};
//   PyObject * face_landmarks_obj = PyArray_SimpleNewFromData( 2, 
// 		  face_landmarks_size, 
// 		  NPY_DOUBLE, 
// 		  face_landmarks.data);
//   bp::handle<> face_landmarks_handle(face_landmarks_obj);
//   bp::numeric::array face_landmarks_arr(face_landmarks_handle);

//   return face_landmarks_arr.copy();

// }

// bp::list detect(Detector *detector, bp::object frame_obj)
// {

//     PyArrayObject *frame_arr = reinterpret_cast<PyArrayObject *>(frame_obj.ptr());
//     const int height = PyArray_DIMS(frame_arr)[0];
//     const int width = PyArray_DIMS(frame_arr)[1];
//     cv::Mat grayscale_frame(cv::Size(width, height), 
// 		    CV_8UC1, 
// 		    PyArray_DATA(frame_arr), 
// 		    cv::Mat::AUTO_STEP);

//     cv::Rect_<double> face_rect = detector->DetectFace(grayscale_frame);

//     bp::list list;
//     list.append(face_rect.x);
//     list.append(face_rect.y);
//     list.append(face_rect.x+face_rect.height);
//     list.append(face_rect.y+face_rect.width);
//     //return list is face_rect[left, top, right, bottom]
//     return list;

// }

// Tracker* init_tracker()
// {

//   std::vector<std::string> args;
//   args.push_back(std::string(" ")); //for low level c++ access args[0]
//   return new Tracker(args);

// }

// bp::object tracking(Tracker *tracker, bp::object frame_obj)
// {

//     PyArrayObject *frame_arr = reinterpret_cast<PyArrayObject *>(frame_obj.ptr());
//     const int height = PyArray_DIMS(frame_arr)[0];
//     const int width = PyArray_DIMS(frame_arr)[1];
//     cv::Mat grayscale_frame(cv::Size(width, height), CV_8UC1, PyArray_DATA(frame_arr), cv::Mat::AUTO_STEP);

//     vector<cv::Rect_<double> > face_rects;
//     vector<cv::Mat_<double> > face_landmarks;
//     bool success = tracker->tracking(grayscale_frame, 
// 		    face_rects, 
// 		    face_landmarks);

//     bp::list face_landmarks_list;
//     for (int i = 0; i < face_landmarks.size(); i++)
//     {
//       long int landmarks_size[2] = {face_landmarks[i].rows, face_landmarks[i].cols};
//       PyObject *landmarks_obj = PyArray_SimpleNewFromData(2,
//                                                           landmarks_size,
//                                                           NPY_DOUBLE,
//                                                           face_landmarks[i].data);
//       bp::handle<> landmarks_handle(landmarks_obj);
//       bp::list landmarks_arr(landmarks_handle);
//       face_landmarks_list.extend(landmarks_arr);
//     }

//     bp::list face_rects_list;
//     for(int i=0; i < face_rects.size(); i++){
//       face_rects_list.append(face_rects[i].x);
//       face_rects_list.append(face_rects[i].y);
//       face_rects_list.append(face_rects[i].x + face_rects[i].height);
//       face_rects_list.append(face_rects[i].y + face_rects[i].width);
//     }

//     return bp::make_tuple(face_rects_list, face_landmarks_list);

// }

int landmark(Featureextraction * featureextracion, bp::object frame_obj) {
  PyArrayObject *frame_arr = reinterpret_cast<PyArrayObject *>(frame_obj.ptr());
  const int height = PyArray_DIMS(frame_arr)[0];
  const int width = PyArray_DIMS(frame_arr)[1];
  cv::Mat grayscale_frame(cv::Size(width, height), CV_8UC1, PyArray_DATA(frame_arr), cv::Mat::AUTO_STEP);
  cv::Mat frame(cv::Size(width, height), CV_8UC3, PyArray_DATA(frame_arr), cv::Mat::AUTO_STEP);
  // std::cout << "nombre de channel dans grayscale_frame" << grayscale_frame.channels() << std::endl;
  // std::cout << "nombre de channel dans frame" << frame.channels() << std::endl;

  bool detection_success = LandmarkDetector::DetectLandmarksInVideo(frame, featureextracion->clnf_model_, featureextracion->det_parameters_, grayscale_frame);
  // if (detection_success) {
  //   std::cout << "La détection est réussie" << std::endl;
  // }else {
  //   std::cout << "La détection est ratée" << std::endl;
  // }
  // std::cout << "detection_success : " << detection_success << std::endl;

  cv::Point3f gazeDirection0(0, 0, 0); cv::Point3f gazeDirection1(0, 0, 0); cv::Vec2d gazeAngle(0, 0);

  if (detection_success && featureextracion->clnf_model_.eye_model)
  {
    GazeAnalysis::EstimateGaze(featureextracion->clnf_model_, gazeDirection0, -1, -1, -1, -1, true);
    GazeAnalysis::EstimateGaze(featureextracion->clnf_model_, gazeDirection1, -1, -1, -1, -1, false);
    gazeAngle = GazeAnalysis::GetGazeAngle(gazeDirection0, gazeDirection1);
  }
  // std::cout << "gaze angle: " << gazeAngle << std::endl;
  featureextracion->clnf_model_.tracking_initialised = false;
  return 0;
}


BOOST_PYTHON_MODULE(pyopenface) {

  // bp::numeric::array::set_module_and_type("numpy", "ndarray");
  // import_array();
  if (_import_array() < 0) {
    PyErr_Print();
    PyErr_SetString(PyExc_ImportError, "numpy.core.multiarray failed to import");
    return;
  }

  bp::class_<Featureextraction>("Featureextraction", bp::no_init)
      .def("__init__", bp::make_constructor(&Featureextraction::Create))
      .def("landmark", &landmark)
      ;
  
  // bp::class_<Detector>("Detector", bp::no_init)
  //     .def("__init__", bp::make_constructor(&Detector::Create))
  //     .def("detect",  &detect)
  //     // .def("landmark", &landmark)
      // ;

  // bp::class_<Tracker>("Tracker", bp::no_init)
  //     .def("__init__", bp::make_constructor(&init_tracker))
  //     .def("tracking",  &tracking)
  //     ;

}
