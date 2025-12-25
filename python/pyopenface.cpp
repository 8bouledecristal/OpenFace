#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
// #include 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <opencv2/core.hpp>

#include <iostream>
// #include <numpy/arrayobject.h>
#include "detector.hpp"
// #include "tracker.hpp"

namespace py = pybind11;

cv::Mat numpy_uint8_to_mat(py::array_t<unsigned char>& input, bool togray) {

    py::buffer_info buf = input.request();

    // TODO extend this a bunch
    int dtype = CV_8UC1;
    if (input.ndim() == 3)
        dtype = (buf.shape[2] == 3) ? CV_8UC3 : CV_8UC4;

    cv::Mat mat(buf.shape[0], buf.shape[1], dtype, (unsigned char*)buf.ptr);

    // if input not grayscale, convert to grayscale before returning
    if (togray && dtype != CV_8UC1) {
        cv::Mat gray;
        int convtype = (dtype == CV_8UC3) ? cv::COLOR_BGR2GRAY : cv::COLOR_BGRA2GRAY;
        cv::cvtColor(mat, gray, convtype);
        return gray;
    }

    return mat;
}

py::list detect(Detector *detector, py::array_t<unsigned char>& input)
{
    cv::Mat grayscale_frame = numpy_uint8_to_mat(input, true);
    cv::Mat rgb_frame = numpy_uint8_to_mat(input, false);

    cv::Rect_<double> face_rect = detector->DetectFace(grayscale_frame, rgb_frame);

    py::list list;
    list.append(face_rect.x);
    list.append(face_rect.y);
    list.append(face_rect.x+face_rect.height);
    list.append(face_rect.y+face_rect.width);
    //return list is face_rect[left, top, right, bottom]
    return list;
}

py::array_t<double> landmark(Detector *detector, py::array_t<unsigned char>& input, const py::list& rect_object)
{

  cv::Mat grayscale_frame = numpy_uint8_to_mat(input, true);
  cv::Mat rgb_frame = numpy_uint8_to_mat(input, false);

  //rect_object[left, top, right, bottom]
  py::ssize_t rect_len = py::len(rect_object);
  assert(rect_len==4);
  double rect_x = rect_object[0].cast<double>();
  double rect_y = rect_object[1].cast<double>();
  double rect_height = rect_object[2].cast<double>() - rect_x;
  double rect_width  = rect_object[3].cast<double>() - rect_y;
  cv::Rect_<double> face_rect( rect_x, 
		  rect_y, 
		  rect_height, 
		  rect_width);

    // Run detector
  cv::Mat_<double> face_landmarks = detector->Run(grayscale_frame, rgb_frame, face_rect);

  // Make a heap-allocated copy
  cv::Mat* cloned_landmarks = new cv::Mat(face_landmarks.clone());

  py::buffer_info buf_info(
      cloned_landmarks->data,                   // data pointer
      sizeof(double),                           // size of one element
      py::format_descriptor<double>::format(),  // Python struct-style format descriptor
      2,                                        // number of dimensions
      {cloned_landmarks->rows, cloned_landmarks->cols},          // shape
      {cloned_landmarks->step[0], cloned_landmarks->step[1]}     // strides
  );

  // Create the NumPy array and attach a capsule that deletes the cv::Mat
  py::array face_landmarks_arr(buf_info,
      py::capsule(cloned_landmarks, [](void *p) {
          delete static_cast<cv::Mat*>(p);
      })
  );

  return face_landmarks_arr;
}

PYBIND11_MODULE(pyopenface, m) {
  
  py::class_<Detector>(m, "Detector")
    .def(py::init(&Detector::Create))
    .def("detect", &detect)
    .def("landmark", &landmark)
    ;
}
