#ifndef TRACKED_OBJECT_H
#define TRACKED_OBJECT_H


#include <opencvheaders.h>

#include <iostream>


#define DEFAULT_R 1.0 //
#define DEFAULT_Q 0.00001//0.001 0.00001

#define DYNAM_PARAM 4
#define MEASURMENT_PARAM 2

class TrackedObject
{
public:
	TrackedObject();
	~TrackedObject();

	/*
	Initialize the Kalman filter with given objects initial position
	*/

	void initialize(int xCoor, int yCoor);
	/*
	Stop tracking the object by clearing all variables
	*/
	void stopTracking();

	/*
		Perform prediction step for KF
	*/
	void KFPredict();

	/*
		Update Kalman Filter state with current x and y position
	*/
	void KFUpdate(int x, int y);

	/*
		get the predicted state of the tracked object
	*/
	void getStatePrediction(std::vector<float> &data);

	/*
		get the corrected state of the object
	*/
	void getStateCorrected(std::vector<float> &data);


	/*
	Print the value of all the matrices...
	*/
	void printData();

	/*
	flag to indicate if object being tracked has been initialized
	*/
	bool initialized = false;

private:
	/*
	Create Kalman Filter Object
	*/
	void setUpKalmanFilter(cv::KalmanFilter *KF);
	void initializeKalmanFilter(cv::KalmanFilter *KF, float x, float y);

	cv::KalmanFilter *Kalmanfilter;
	cv::Mat_<float> measurement;
	long id;
};

#endif // TRACKED_OBJECT_H

