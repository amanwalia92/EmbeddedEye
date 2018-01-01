#ifndef MOTION_EVENT_DETECTOR_H
#define MOTION_EVENT_DETECTOR_H

#include <opencvheaders.h>

#include <stdio.h>
#include <iostream>
#include <sstream>

#include "TrackedObject.h"

#define TRACKER_MIN_OBJECT_AREA (150)
#define TRACKER_THRESHOLD_ (0.75*255)
#define NUM_FRAMES_TO_WAIT_BEFORE_DETECTION 20
#define MAX_NUM_POINTS 50
#define LARGE_NUMBER 1000000000

class MotionEventDetector
{
public:

	/*
		Constructor for  MotionEventDetector class.
	*/
	MotionEventDetector(){

		//elementDilate = getStructuringElement(cv::MORPH_RECT, cv::Size(8, 8)); // room 8, 30
		elementDilate = getStructuringElement(cv::MORPH_RECT, cv::Size(8, 20)); // lecture
		//elementDilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); // people

		cv::namedWindow("Foreground", CV_WINDOW_NORMAL);
		cv::namedWindow("MorphologyEx", CV_WINDOW_NORMAL);
		cv::namedWindow("Blur", CV_WINDOW_NORMAL);
		cv::namedWindow("After Threshold and Dilation", CV_WINDOW_NORMAL);
		cv::namedWindow("After Background Mask", CV_WINDOW_NORMAL);

		blurKernel = cv::Size(3, 5);

		trackedState = std::vector<float>(4, 0.0);
	};

	~MotionEventDetector(){};

	/*
		detectMotion(cv::Mat &frame, cv::Mat &prob, bool process): simply returns the binary image output with all the blobs parts of the foreground
		after applying background subtraction algorith.
	*/
	void detectMotion(cv::Mat &frame, cv::Mat &prob, bool process){

		cv::resize(frame, resized, cv::Size(), _setResizeFactor, _setResizeFactor);

		doBackgroundSubtraction(resized);

		if (process)
			processForeground();

		prob = foreground;

	}
	/*
	setRectangleMask(cv::Size s): sets the mask region for where to actually focus on detecting motion.
	Size s defines the top border and the horizontal border equal lenght
	*/
	void setRectangleMask(cv::Size s){
		_maskSize = s;
	}

	/*
	trackMotion(cv::Mat &frame): tracks motion using the kalman filter. Selects the biggest moving object,
	(biggest blob) and tracks it. The result is stored in the trackedPoint attribute of this class
	*/
	void trackMotion(cv::Mat &frame){

		cv::resize(frame, resized, cv::Size(), _setResizeFactor, _setResizeFactor);

		if (!backMask.data)
		{
			// make it initially point to the centre of the image
			trackedState[0] = frame.cols / 2; trackedState[1] = frame.rows / 2;

			backMask = cv::Mat(resized.rows, resized.cols, CV_8UC1, cv::Scalar(0, 0, 0));
			backMask(cv::Rect(_maskSize.width, _maskSize.height, backMask.cols - _maskSize.width * 2, backMask.rows - _maskSize.height * 2)) = cv::Scalar(255, 255, 255);
		}


		doBackgroundSubtraction(resized);

		isOutOfBounds(); // check if out of bounds


		if (_currFrame++ < NUM_FRAMES_TO_WAIT_BEFORE_DETECTION) // wait for background to be 'learned'
			return;

		processForeground();


		int ind = findROIs(foreground, frame, coordinates, ROIRectangles);

		processDetections(frame);

		if (ind > -1) // initialize tracking of the largest one
		{

			if (!tracker.initialized)
			{
				detections.push_back(cv::Point(coordinates[ind][0] / _setResizeFactor, coordinates[ind][1] / _setResizeFactor));
				if (detections.size() > MAX_NUM_POINTS){ detections.erase(detections.begin()); }

				// initialize tracked point to given coordinates
				trackedState[0] = coordinates[ind][0] / _setResizeFactor;
				trackedState[1] = coordinates[ind][1] / _setResizeFactor;

				tracker.initialize(coordinates[ind][0] / _setResizeFactor, coordinates[ind][1] / _setResizeFactor);

				estimates.push_back(cv::Point(coordinates[ind][0] / _setResizeFactor, coordinates[ind][1] / _setResizeFactor));
				if (estimates.size() > MAX_NUM_POINTS){ estimates.erase(estimates.begin()); }
			}
			else
			{
				tracker.KFPredict();  // predict

				tracker.getStateCorrected(trackedState);

				int closest = getClosestPoint(trackedState[0] * _setResizeFactor, trackedState[1] * _setResizeFactor, trackedState[2], trackedState[3]);

				if (closest > -1)
				{

					detections.push_back(cv::Point(coordinates[closest][0] / _setResizeFactor, coordinates[closest][1] / _setResizeFactor));
					if (detections.size() > MAX_NUM_POINTS){ detections.erase(detections.begin()); }

					// update with the closest
					tracker.KFUpdate(coordinates[closest][0] / _setResizeFactor, coordinates[closest][1] / _setResizeFactor);

					// get most updated estimate
					tracker.getStateCorrected(trackedState);

					//cv::circle(frame, cv::Point(trackedState[0], trackedState[1]), 6, cv::Scalar(0, 255, 0), 2);

					estimates.push_back(cv::Point(trackedState[0], trackedState[1]));
					if (estimates.size() > MAX_NUM_POINTS){ estimates.erase(estimates.begin()); }

				}
				else
				{
					tracker.getStatePrediction(trackedState);
					//cv::circle(frame, cv::Point(trackedState[0], trackedState[1]), 6, cv::Scalar(0, 0, 255), 2);
					estimates.push_back(cv::Point(trackedState[0], trackedState[1]));
					if (estimates.size() > MAX_NUM_POINTS){ estimates.erase(estimates.begin()); }
				}

			}
		}
		else if (tracker.initialized)
		{
			tracker.KFPredict();
			//std::vector<float> kalmanPredicted;
			tracker.getStatePrediction(trackedState);

			//cv::circle(frame, cv::Point(trackedState[0], trackedState[1]), 6, cv::Scalar(0, 0, 255), 2);

			estimates.push_back(cv::Point(trackedState[0], trackedState[1]));
			if (estimates.size() > MAX_NUM_POINTS){ estimates.erase(estimates.begin()); }

		}

		drawLines(frame);

		int b = 0;

	}
	/*
	setResizeFactor(float s): sets by how much image should be resized before processing
	*/
	void setResizeFactor(float s){
		_setResizeFactor = s;
	}

	/*
	StopTracking: stops tracking motion
	*/

	void stopTracking(){
		tracker.stopTracking(); _currFrame = NUM_FRAMES_TO_WAIT_BEFORE_DETECTION / 2;
	}

	// stores the current object being tracked
	std::vector<float> trackedState;

private:


	/*
	processForeground():a processes the extracted foreground image
	*/
	void processForeground(){

		//cv::dilate(foreground, foreground, elementDilate);
		//cv::threshold(foreground, foreground, TRACKER_THRESHOLD_, 255, CV_THRESH_BINARY);
		//imshow("After Background Mask", foreground);
		//foreground = foreground&backMask;
		//imshow("After Background Mask", foreground);

		cv::morphologyEx(foreground, foreground, cv::MORPH_CLOSE, elementDilate);
		imshow("MorphologyEx", foreground);
		cv::blur(foreground, foreground, blurKernel);
		imshow("Blur", foreground);
		cv::threshold(foreground, foreground, TRACKER_THRESHOLD_, 255, CV_THRESH_BINARY);
		cv::dilate(foreground, foreground, elementDilate);
		imshow("After Threshold and Dilation", foreground);
		foreground = foreground&backMask;
		imshow("After Background Mask", foreground);
	}

	/*
	doBackgroundSubtraction(cv::Mat &resized): performs backgroound subtraction algorithm
	*/
	void doBackgroundSubtraction(cv::Mat &resized){
		//pMOG2.operator()(resized, foreground, 0.001);//0.001
		pMOG2.operator()(resized, foreground);
		imshow("Foreground", foreground);

	}
	/*
	findROIs(cv::Mat &prob, cv::Mat image, std::vector<std::vector<float>> &coordinates, std::vector<cv::Rect> &ROIRectangles):
	finds contours, filters objects by area, and find the bounding rectangle and centre of the blobs.
	*/
	int	findROIs(cv::Mat &prob, cv::Mat image, std::vector<std::vector<float>> &coordinates, std::vector<cv::Rect> &ROIRectangles)
	{
		coordinates.clear();
		ROIRectangles.clear();

		std::vector<std::vector<cv::Point>> contours;

		// find the contours
		findContours(prob, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


		double largestArea = 0, currentArea = 0; int largestIndex = -1;
		int j = 0;//need this because we only use index if there was an object large enough found


		// get the bounding rectangles and filter by minimum object area
		for (int i = 0; i < contours.size(); i++)
		{
			if (cv::contourArea(contours[i], false) >= TRACKER_MIN_OBJECT_AREA*_setResizeFactor)
			{

				cv::Rect bound_rectangle = cv::boundingRect(contours[i]);
				float midX; float midY;

				cv::Rect bound_rectangle_scaled;
				bound_rectangle_scaled.x = bound_rectangle.x / _setResizeFactor;
				bound_rectangle_scaled.y = bound_rectangle.y / _setResizeFactor;
				bound_rectangle_scaled.width = bound_rectangle.width / _setResizeFactor;
				bound_rectangle_scaled.height = bound_rectangle.height / _setResizeFactor;

				cv::rectangle(image, bound_rectangle_scaled, cv::Scalar(0, 255, 255), 2); //DRAWING rectangle on screen <-----------------


				cv::Moments moment = moments((cv::Mat)contours[i]); //computing moment to determine x and y coordinate of blob
				currentArea = moment.m00;

				midX = moment.m10 / currentArea;
				midY = moment.m01 / currentArea;


				cv::circle(image, cv::Point(midX / _setResizeFactor, midY / _setResizeFactor), 3, cv::Scalar(0, 255, 255), -1);//DRAWING moment middle point <------------

				ROIRectangles.push_back(bound_rectangle); //store bounding rectangle
				coordinates.push_back({ midX, midY }); //store coordinates

				if (currentArea > largestArea)
				{
					largestArea = currentArea;
					largestIndex = j;
				}
				j++;
			}
		}

		return largestIndex;
	}
	/*
	getCentreOfRectangle(cv::Rect r): computes the centre of a rectangle
	*/
	cv::Point getCentreOfRectangle(cv::Rect r)
	{
		int x = r.width / 2 + r.x;
		int y = r.height / 2 + r.y;

		return cv::Point(x, y);
	}


	/*
	drawLines(cv::Mat &frame): draws the previous MAX_NUM_POINTS tracked .
	*/
	void drawLines(cv::Mat &frame)
	{
		int lineSize = 4;

		if (detections.size() > 1)
		{
			for (int i = 1; i < detections.size(); i++)
			{
				cv::Point a = cv::Point(detections[i].x, detections[i].y);
				cv::Point b = cv::Point(detections[i - 1].x, detections[i - 1].y);
				cv::line(frame, a, b, cv::Scalar(255, 0, 255), lineSize);
			}
		}

		if (estimates.size() > 1)
		{
			for (int i = 1; i < estimates.size(); i++)
			{
				cv::Point a = cv::Point(estimates[i].x, estimates[i].y);
				cv::Point b = cv::Point(estimates[i - 1].x, estimates[i - 1].y);
				cv::line(frame, a, b, cv::Scalar(0, 255, 0), lineSize);
			}
		}
	}

	/*
	processDetections(cv::Mat &frame): simply selects the region of interest to the upper half of the detections
	*/
	void processDetections(cv::Mat &frame){

		for (int i = 0; i < ROIRectangles.size(); i++)
		{

			ROIRectangles[i].height = ROIRectangles[i].height / 2;
			cv::Point c = getCentreOfRectangle(ROIRectangles[i]);
			coordinates[i][0] = c.x;
			coordinates[i][1] = c.y;

			cv::Rect bound_rectangle_scaled;
			bound_rectangle_scaled.x = ROIRectangles[i].x / _setResizeFactor;
			bound_rectangle_scaled.y = ROIRectangles[i].y / _setResizeFactor;
			bound_rectangle_scaled.width = ROIRectangles[i].width / _setResizeFactor;
			bound_rectangle_scaled.height = ROIRectangles[i].height / _setResizeFactor;

			cv::rectangle(frame, bound_rectangle_scaled, cv::Scalar(255, 0, 255), 2);
			cv::circle(frame, cv::Point(coordinates[i][0] / _setResizeFactor, coordinates[i][1] / _setResizeFactor), 3, cv::Scalar(255, 0, 255), 2);

		}
	}

	/*
	getClosestPoint(float px, float py, float p_dx, float p_dy): finds the closest point
	to (px,py) given the current set of detected points (coordinates)
	*/
	int getClosestPoint(float px, float py, float p_dx, float p_dy)
	{
		//float maxDist = p_dx*p_dx + p_dy*p_dy; maxDist *= 100;
		float dist = 0;
		float minDist = LARGE_NUMBER;
		int ind = -1;
		for (int i = 0; i < coordinates.size(); i++)
		{
			dist = (px - coordinates[i][0])*(px - coordinates[i][0]) + (py - coordinates[i][1])*(py - coordinates[i][1]);

			// find the closest one
			//if (dist <= minDist && (dist <= maxDist || maxDist == 0))
			if (dist <= minDist)
			{
				minDist = dist;
				ind = i;
			}

			dist = 0;
		}

		return ind;
	}
	/*
	isOutOfBounds() checks if the current tracked point is out of bounds of the frame. If it is, stop tracking and reinitialize

	*/
	void isOutOfBounds()
	{
		if (tracker.initialized)
		{
			if (trackedState[0] >= (foreground.cols - _maskSize.width) / _setResizeFactor || trackedState[0] <= _maskSize.width)
			{
				tracker.stopTracking(); _currFrame = NUM_FRAMES_TO_WAIT_BEFORE_DETECTION / 2;
			}
			else if (trackedState[1] >= (foreground.rows - _maskSize.height) / _setResizeFactor || trackedState[1] <= _maskSize.height)
			{
				tracker.stopTracking(); _currFrame = NUM_FRAMES_TO_WAIT_BEFORE_DETECTION / 2;
			}
		}
	}

	cv::Mat foreground; //fg mask fg mask generated by MOG2 method
	cv::BackgroundSubtractorMOG2 pMOG2;
	cv::Mat resized;
	cv::Mat elementDilate;
	cv::Size blurKernel;
	cv::Mat backMask;
	cv::Size _maskSize;

	std::vector<std::vector<float>> coordinates;
	std::vector<cv::Rect> ROIRectangles;

	std::vector<cv::Point> detections;
	std::vector<cv::Point> estimates;


	int _currFrame = 0;
	float _setResizeFactor = 0.25;

	// Tracking stuff
	TrackedObject tracker;

};

#endif // MOTION_EVENT_DETECTOR_H // 
