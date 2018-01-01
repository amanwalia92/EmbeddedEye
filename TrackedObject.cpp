#include "TrackedObject.h"



TrackedObject::TrackedObject() :
Kalmanfilter(NULL),
initialized(false)
{

}


TrackedObject::~TrackedObject()
{
	if (Kalmanfilter)
	{
		delete Kalmanfilter;
	}
}

void TrackedObject::initialize(int xCoor, int yCoor)
{

	if (initialized) // make sure object stopped tracking before reinitilizing it again
	{
		stopTracking();
	}

	measurement = cv::Mat_<float>(MEASURMENT_PARAM, 1);//this will hold the measurements
	measurement.setTo(cv::Scalar(0.0));

	// create new Kalman Filter Object
	Kalmanfilter = new cv::KalmanFilter(DYNAM_PARAM, MEASURMENT_PARAM, 0);

	setUpKalmanFilter(Kalmanfilter);

	// initialize this kalman filter to given parameters
	initializeKalmanFilter(Kalmanfilter, xCoor, yCoor); //initializing kalman filter to specified coordinates of object to be tracked

	// set the initialized flag to true
	initialized = true;
}

void TrackedObject::stopTracking()
{
	initialized = false;

	if (Kalmanfilter)
	{
		delete Kalmanfilter;
	}

	Kalmanfilter = NULL;

	measurement.release();
}

void TrackedObject::KFPredict()
{
	if (initialized)
	{
		Kalmanfilter->predict();
	}

}

void TrackedObject::KFUpdate(int x, int y)
{
	if (initialized)
	{
		measurement.at<float>(0) = x;
		measurement.at<float>(1) = y;

		Kalmanfilter->correct(measurement);
	}

}


void TrackedObject::getStatePrediction(std::vector<float> &data)
{
	data.clear();

	data.push_back(Kalmanfilter->statePre.at<float>(0));
	data.push_back(Kalmanfilter->statePre.at<float>(1));
	data.push_back(Kalmanfilter->statePre.at<float>(2));
	data.push_back(Kalmanfilter->statePre.at<float>(3));

}

void TrackedObject::getStateCorrected(std::vector<float> &data)
{
	data.clear();

	data.push_back(Kalmanfilter->statePost.at<float>(0));
	data.push_back(Kalmanfilter->statePost.at<float>(1));
	data.push_back(Kalmanfilter->statePost.at<float>(2));
	data.push_back(Kalmanfilter->statePost.at<float>(3));

}

void TrackedObject::setUpKalmanFilter(cv::KalmanFilter *KF)
{
	// setting some parameters

	//A

	/*KF->transitionMatrix = *(cv::Mat_<float>(DYNAM_PARAM, DYNAM_PARAM) << 1, 0, 1, 0, 0.5, 0,
	0, 1, 0, 1, 0, 0.5,
	0, 0, 1, 0, 0, 0,
	0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 1
	);*/
	KF->transitionMatrix = *(cv::Mat_<float>(DYNAM_PARAM, DYNAM_PARAM) << 1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1
		);


	// H
	cv::setIdentity(KF->measurementMatrix);

	// R
	/*KF->measurementNoiseCov = DEFAULT_R*(cv::Mat_<float>(MEASURMENT_PARAM, MEASURMENT_PARAM) << 1, 0,
	0, 1
	);*/
	cv::setIdentity(KF->measurementNoiseCov); KF->measurementNoiseCov = KF->measurementNoiseCov*DEFAULT_R;

	// Q
	/*KF->processNoiseCov = DEFAULT_Q*(cv::Mat_<float>(DYNAM_PARAM, DYNAM_PARAM) << 1, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 1

	);*/
	cv::setIdentity(KF->processNoiseCov); KF->processNoiseCov = KF->processNoiseCov*DEFAULT_Q;

	KF->errorCovPost = (cv::Mat_<float>(DYNAM_PARAM, DYNAM_PARAM) << 1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1
		);
}

void TrackedObject::initializeKalmanFilter(cv::KalmanFilter *KF, float x, float y)
{
	KF->statePre.at<float>(0) = x;
	KF->statePre.at<float>(1) = y;
	KF->statePre.at<float>(2) = 0;
	KF->statePre.at<float>(3) = 0;
	//KF->statePre.at<float>(4) = 0;
	//KF->statePre.at<float>(5) = 0;

	KF->statePost.at<float>(0) = x;
	KF->statePost.at<float>(1) = y;
	KF->statePost.at<float>(2) = 0;
	KF->statePost.at<float>(3) = 0;
	//KF->statePost.at<float>(4) = 0;
	//KF->statePost.at<float>(5) = 0;
}

void TrackedObject::printData()
{
	if (Kalmanfilter && initialized)
	{
		std::cout << Kalmanfilter->statePre << std::endl;
		std::cout << Kalmanfilter->statePost << std::endl;
	}
}