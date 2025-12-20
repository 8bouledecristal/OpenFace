#pragma once

#include <LandmarkCoreIncludes.h>
#include <string>
#include <vector>
#include "LandmarkCoreIncludes.h"
#include <Face_utils.h>
#include <FaceAnalyser.h>
#include <GazeEstimation.h>
#include <RecorderOpenFace.h>
#include <RecorderOpenFaceParameters.h>
#include <SequenceCapture.h>
#include <Visualizer.h>
#include <VisualizationUtils.h>
#include "ImageManipulationHelpers.h"



class Featureextraction {
public:
    static Featureextraction* Create(const char* binary_path);
    LandmarkDetector::FaceModelParameters det_parameters_;
    LandmarkDetector::CLNF clnf_model_;
    FaceAnalysis::FaceAnalyserParameters face_analysis_params_;
    FaceAnalysis::FaceAnalyser face_analyser_;

private:
    Featureextraction(LandmarkDetector::FaceModelParameters det_parameters,
                      LandmarkDetector::CLNF clnf_model,
                      FaceAnalysis::FaceAnalyserParameters face_analysis_params,
                      FaceAnalysis::FaceAnalyser face_analyser);

    
};
