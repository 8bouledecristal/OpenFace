// the python version for FaceTrackingVidMulti.cpp : Defines the entry point for the multiple face tracking console application.
#include "LandmarkCoreIncludes.h"

#include <fstream>
#include <sstream>

// OpenCV includes
#include <opencv2/videoio/videoio.hpp>  // Video write
#include <opencv2/videoio/videoio_c.h>  // Video write
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// TBB include
#include <tbb/tbb.h>

#define INFO_STREAM( stream ) \
std::cout << stream << std::endl

#define WARN_STREAM( stream ) \
std::cout << "Warning: " << stream << std::endl

#define ERROR_STREAM( stream ) \
std::cout << "Error: " << stream << std::endl

static void printErrorAndAbort( const std::string & error )
{
    std::cout << error << std::endl;
    abort();
}

#define FATAL_STREAM( stream ) \
printErrorAndAbort( std::string( "Fatal error: " ) + stream )

class Tracker{

  public:
	Tracker(std::vector<std::string> &args);
    bool tracking(cv::Mat &video_capture, std::vector<cv::Rect_<double> > &face_rects, std::vector<cv::Mat_<double> > &face_landmarks);

  private:
    std::vector<std::string> get_arguments(int argc, char **argv);
    void NonOverlapingDetections(const std::vector<LandmarkDetector::CLNF>& clnf_models, std::vector<cv::Rect_<float> >& face_detections);

	std::vector<std::string> arguments;

	int frame_count;
	cv::Mat captured_image;
	// Reading the images
	cv::Mat_<double> depth_image;
	cv::Mat_<uchar> grayscale_image;
	cv::Mat disp_image;

	std::vector<cv::Rect_<float> > face_detections;

	// saving the videos
	std::string current_file;
	cv::VideoWriter writerFace;
	// Get the input output file parameters
	bool u;
	std::string output_codec;
	char fpsC[255];
	std::string fpsSt;

	// If multiple video files are tracked, use this to indicate if we are done
	bool done;	
	int f_n;

	// By default try webcam 0
	int device;
	// cx and cy aren't necessarilly in the image center, so need to be able to override it (start with unit vals and init them if none specified)
    float fx, fy, cx, cy;
	// If cx (optical axis centre) is undefined will use the image size/2 as an estimate
	bool cx_undefined;

	// Some initial parameters that can be overriden from command line	
	std::vector<std::string> files, depth_directories, tracked_videos_output, dummy_out;

	// The modules that are being used for tracking
	//LandmarkDetector::FaceModelParameters det_params;
	std::vector<LandmarkDetector::FaceModelParameters> det_parameters;
	std::vector<LandmarkDetector::CLNF> clnf_models;
	std::vector<bool> active_models;
	int num_faces_max;
	bool all_models_active;
	char active_m_C[255];
	std::string active_models_st;
	int num_active_models;

	bool use_depth;
	cv::Mat_<short> depth_image_16_bit;

};
