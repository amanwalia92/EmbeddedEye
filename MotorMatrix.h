//
// Created by aman on 15/03/17.
//

#ifndef MOTORCOORD_MOTORMATRIX_H
#define MOTORCOORD_MOTORMATRIX_H

#define MATRIX_SIZE 2
#define FRAME_HEIGHT 480
#define FRAME_WIDTH  640
#define MOTOR_LUT_FILE_PATH "./points.txt"

#include <iostream>
#include <vector>


class MotorMatrix
{

public:
    MotorMatrix();
    ~MotorMatrix();


    void loadMotorTargetsLUT();
    void getPanTiltFromCoordinates(int xPixCoordinate, int yPixCoordinate, float *panTargetVal, float *tiltTargetVal);
    static void readToBufferPointer(std::string fileName, float *buffer, long sizeOfBufferElements);
    void resetMatrices();



private:


    std::string motorTargetsLUTFilename;
    std::vector<std::vector<std::vector<float> > > motorTargets;  //motor targets LUT

};

#endif //MOTORCOORD_MOTORMATRIX_H
