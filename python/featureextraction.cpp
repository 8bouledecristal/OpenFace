#include "featureextraction.hpp"
#include <LandmarkCoreIncludes.h>
#include <Face_utils.h>
#include <FaceAnalyser.h>
#include <GazeEstimation.h>
#include <RecorderOpenFace.h>
#include <RecorderOpenFaceParameters.h>
#include <SequenceCapture.h>
#include <Visualizer.h>
#include <VisualizationUtils.h>

using std::cout;
using std::string;
using std::vector;

Featureextraction* Featureextraction::Create(const char* binary_path) {
    std::vector<std::string> arguments;
    arguments.emplace_back(" ");
    arguments.emplace_back("-mloc");
    arguments.emplace_back(binary_path);

    LandmarkDetector::FaceModelParameters det_parameters(arguments);
    det_parameters.haar_face_detector_location =
        "/root/openface/lib/3rdParty/OpenCV/classifiers/haarcascade_frontalface_alt2.xml";

    LandmarkDetector::CLNF face_model(det_parameters.model_location);

    if (!face_model.loaded_successfully) {
        throw std::runtime_error("Could not load landmark detector");
    }

    FaceAnalysis::FaceAnalyserParameters face_analysis_params(arguments);
    FaceAnalysis::FaceAnalyser face_analyser(face_analysis_params);

    return new Featureextraction(
        std::move(det_parameters),
        std::move(face_model),
        std::move(face_analysis_params),
        std::move(face_analyser)
    );
}

Featureextraction::Featureextraction(
    LandmarkDetector::FaceModelParameters det_parameters,
    LandmarkDetector::CLNF clnf_model,
    FaceAnalysis::FaceAnalyserParameters face_analysis_params,
    FaceAnalysis::FaceAnalyser face_analyser)
    : det_parameters_(std::move(det_parameters)),
      clnf_model_(std::move(clnf_model)),
      face_analysis_params_(std::move(face_analysis_params)),
      face_analyser_(std::move(face_analyser)) {}
