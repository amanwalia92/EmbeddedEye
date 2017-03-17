//
// Created by aman on 15/03/17.
//

#include "MotorMatrix.h"



MotorMatrix::MotorMatrix() {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MotorMatrix::~MotorMatrix()
{
    motorTargets.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of utility functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MotorMatrix::readToBufferPointer(std::string fileName, float *buffer, long sizeOfBufferElements)
{
    FILE *f = fopen(fileName.c_str(), "rb");
    if (fread(buffer, sizeof(float), sizeOfBufferElements, f)){ std::cout << "File read sucess!\n"; }
    else{ std::cout << "Failed to read File!\n"; }
    fclose(f);
}


void MotorMatrix ::loadMotorTargetsLUT() {

    motorTargets.clear();
    float *buffer;
    buffer = new float[FRAME_HEIGHT * FRAME_WIDTH * MATRIX_SIZE];
    readToBufferPointer(MOTOR_LUT_FILE_PATH, buffer, FRAME_HEIGHT * FRAME_WIDTH * MATRIX_SIZE);

    motorTargets.resize(MATRIX_SIZE);

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        motorTargets[i].resize(FRAME_WIDTH);
        for (int j = 0; j < FRAME_WIDTH; ++j)
            motorTargets[i][j].resize(FRAME_HEIGHT);
    }

    //copy into vector
    float*ptr = buffer;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < FRAME_WIDTH; j++)
            for (int k = 0; k < FRAME_HEIGHT; k++)
                motorTargets[i][j][k] = *ptr++;
    //free(buffer);
    delete buffer;

}

void MotorMatrix ::getPanTiltFromCoordinates(int xPixCoordinate, int yPixCoordinate, float *panTargetVal,
                                             float *tiltTargetVal) {

    *panTargetVal = motorTargets[0][xPixCoordinate][yPixCoordinate];
    *tiltTargetVal = motorTargets[1][xPixCoordinate][yPixCoordinate];
}
