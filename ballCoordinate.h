
/**
 * @Author Dat nguyen
 *
 * This library establish a protocol for transmitting
 * data from the raspberry pi to arduino
 * The data need to be format in the following manner
 * ex: 
 * ;C1,x1,y1,z1;C2,x2,y2,z2;.....; 
 * ;1,5,12,32;0,12,43,53;
 * ;1,3,103,12;
 *
 * If there is no pingpong to pickj 
 * ;0; 
 *
 * Where:
 * C: color where ( R:0, g:1, b:2)
 * x: x-axis value being pass. 
 * y: y-axis value being pass
 * z: z-axis value being pass
 *
 * Unit: Raspbery pi will pass pixel value
 */

 #ifndef BALL_COORDINATES   /* Include guard */
 #define BALL_COORDINATES

#include "stdint.h"
#include "stdlib.h"
#include <Arduino.h>
#include<Driving.h>

#define MAXAPPLE 9
#define DATALENGTH 100

extern char dataStream[DATALENGTH];
extern uint8_t* color;
extern uint8_t* simpleCoordinate;
extern float* x;
extern float * y;
extern float* z;
extern uint8_t column;
extern uint8_t numOfPoint;

/******************score keeper counter*****************/
extern int row1[MAXAPPLE];
extern int row2[MAXAPPLE];
extern int currentZone; 
extern int numOfTrig; 
/**Constant**/
// Raspbery pi info
#define SCREEN_WIDTH 720.00
#define SCREEN_LENGTH 1024.00
#define X_CAMERA_TO_ARM 4.47401575
#define Y_CAMERA_TO_ARM 6.4125

const float L_COOR_X1[3] = {-5.7,2.8,7.5};
const float L_COOR_X2[3] = {-5.0,2.9,10.5};
const float L_COOR_X3[3] = {-5.2,5.5,12.0};
const float L_COOR_X4[3] = {-5.7,7.0,9.1};
const float L_COOR_DROP[3] = {-5.0,9.0,10.5};
const float L_COOR_BASKET[3] = {-5.7,7.0,9.1};

// TODO: delete these .
#define COOR_X_1 -5.7
#define COOR_Y_1 2.8
#define COOR_Z_1 7.5

#define COOR_X_2 -5.0
#define COOR_Y_2 2.9
#define COOR_Z_2 10.5

#define COOR_X_3 -5.2
#define COOR_Y_3 5.5
#define COOR_Z_3 12.0

#define COOR_X_4 -5.7
#define COOR_Y_4 7.0
#define COOR_Z_4 9.10

#define COOR_X_DROP -5.0
#define COOR_Y_DROP 9.0
#define COOR_Z_DROP 10.50


// FUNCTION

/**
 * This function print the point exist in 3 global array (color,y,z)
 * return
 * param
 */
void printPoint();

void printSimplePoint(void);

/**
 * This function get data transmited from raspberry pi then store
 * data in dataStream char array  variable(gloabl variable)
 * return 1 if valid dataStream length got recieve
 */
bool getDataStream(char* package);

/**
 *This function will parse data by using split function
 * to get it token then put it on to a map/struct
 * return true if data is in correct format. or doesnot spot any wierd data.
 * return false; if spot incorrect formatted data.
 */
bool parseData(void);

/**
 * This method compute and transform the pixel coordinate
 * passed over by the PI to actual coordinate in inches
 * that respect to Robot Arm Base
 *
 * param xVal(pixel) and yVal(pixel) and zVal(value retrieve from
 * ultrasonic sensor
 *
 * return Nothing
 * Change xVal and yVal by reference.
 */
void transformation(double *xVal, double *yVal,const double zVal);

/**
 * This method parse the data using split function
 * using hard coded value then send it to the arm.
 * return true if it parse success full
 */
bool parseSimpleData(void);

/**
 * This method convert the given
/**
 * This function split the input string by a delimiter
 * then return a DEEp copy of a 2d array
 * n is the number of element got splited.( optional);
 */
char ** split(const char *str, char delimiter, uint8_t *n);

/*
 * This function deepcopy a array
 */
char * deepCopy(const char *s);

#endif
