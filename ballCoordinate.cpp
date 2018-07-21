
#include "ballCoordinate.h"
#include <CameraArmDriver.h>
#include <stdlib.h>
#include <stdio.h>
#include <arduino.h>

char dataStream[DATALENGTH];
uint8_t numOfPoint;

uint8_t* color;
uint8_t* simpleCoordinate;
float* x;
float* y;
float* z;

int row1[MAXAPPLE];
int row2[MAXAPPLE];
int numOfTrig; 



bool getDataStream(char* package){
  Serial2.begin(115200);
  Serial.begin(115200);
  numOfTrig ++; 
  // code Reuse from last year competition

  //first clear all data in the struct
  memset(dataStream, 0, DATALENGTH);

  // Serial Communication with Raspberry Pi on Serial port 1 begins
  
  // // int cameraPos = getCameraFacePosition();
  // char package[30];

  // //don't trust reading if sonaroffset is 1111 or -1111 
  // sprintf(package,";%d;%d;",sonaOffset,cameraPos);
  // Serial.print("package sending to rasp -offset,ServoPOs");
  Serial2.println(package);

  while(Serial2.available() == 0 ){}

  int dataindex = 0;
  while(Serial2.available()> 0){

  // Fills datastream
    if(dataindex < DATALENGTH-1)
      {
          char inChar = Serial.read(); // Read a character
          dataStream[dataindex] = inChar; // Store it
          dataindex++; // Increment where to write next
      }
    delayMicroseconds(100);
  }
  dataStream[dataindex] = '\0'; // Null terminate the string
  Serial2.flush();

  Serial.print("DataStream(contain): ");Serial.println(dataStream);
  return true;
}

bool parseData(void){

  //split package  to main token by ';'
  char **stringToken = NULL;
  stringToken = split(dataStream,';',&numOfPoint);

  // ;1,3,4,5;2,4,1,3;
  numOfPoint -= 2; // minus end and start delimiter.

  if(numOfPoint < 1){
    Serial.println("insecure package. package does not covered by delimter"); 
    return false; 
  }

  //check if there are any point being pass; 
  Serial.print("stringtoekn[1]");
  Serial.println(atoi(stringToken[1]));
  if(atoi(stringToken[1]) == 404){
    Serial.println("No apple at this zone"); 
    return false;
  }

   //counting coordinate that have apple. 

  // numOfPoint = atoi(stringToken[0]);

  //TODO: error checking

  //set dynamic array size.
  //create dynamic array size.
  // make sure it only have a size of 1 or 2 acordingly to numOfPoint.
  color = (uint8_t*) malloc(sizeof(uint8_t) * numOfPoint);
  x = (float*) malloc(sizeof(float) * numOfPoint);
  y = (float*) malloc(sizeof(float) * numOfPoint);
  z = (float*) malloc(sizeof(float) * numOfPoint);


  char **pointToken = NULL;
  for(int i = 0;i < numOfPoint; i++){
    uint8_t n = 0 ;// dummies varable for debug or output valiedation
    pointToken = split(stringToken[i],',',&n);

    //deposit in to global variable
    color[i] = atoi(pointToken[0]);
    x[i] = String(pointToken[1]).toFloat();
    y[i] = String(pointToken[2]).toFloat();
    z[i] = String(pointToken[3]).toFloat();
  }


  // free all 2d array token to prevent mem leak
  for(int i = 0; i < numOfPoint; i++) free(pointToken[i]);
  free(pointToken);
  for(int i = 0; i < numOfPoint; i++) free(stringToken[i]);
  free(stringToken);

  //TODO: ERROR checking and return true if success ful else return false;
  printPoint();
  return true;
}

void printPoint(){
  for(int i = 0; i<numOfPoint;i++){
    Serial.print(color[i]);Serial.print(",");Serial.print(x[i]);Serial.print(",");Serial.print(y[i]);Serial.print(",");Serial.print(z[i]);
    Serial.println("");
  }
}

void printSimplePoint(){
  for(int i = 0; i<4;i++){
    Serial.print(color[i]);Serial.println(",");
  }
}

void transformation(double *xVal, double *yVal,const double zVal){
	// convert to real inches value from pixel
}

bool parseSimpleData(void){
  uint8_t n = 0; //number of coordinate
  //split package  to main token by ';'
  char **stringToken = NULL;
  stringToken = split(dataStream,';',&n);

  //store number of coordinate is passing
  numOfPoint = atoi(stringToken[0]);

  // exit if receive bad data;
  //TODO: added more validation
  if(numOfPoint > 2 || numOfPoint < 1) {
    //sSerial.println("fail in parse");
    return false;
  }

  //set dynamic array size.
  //create dynamic array size.
  // make sure it only have a size of 1 or 2 acordingly to numOfPoint.
  color = (uint8_t*) malloc(sizeof(uint8_t) * numOfPoint);
  simpleCoordinate = (int8_t*) malloc(sizeof(int8_t) * numOfPoint);

  char **pointToken = NULL;
  Serial.println("debug");
  Serial.print("n");Serial.println(n);
  for(int i = 0;i < n; i++){
    uint8_t d = 0 ;// dummies varable for debug or output valiedation
    pointToken = split(stringToken[i+1],',',&d);

    //deposit in to global variable

	color[i] = atoi(pointToken[0]);
    Serial.print(color[i]);Serial.println(",");
  }



  // free all 2d array token to prevent mem leak
  for(int i = 0; i < numOfPoint; i++) free(pointToken[i]);
  free(pointToken);
  for(int i = 0; i < numOfPoint; i++) free(stringToken[i]);
  free(stringToken);

  return true;
}

char ** split(const char *str, char delimiter, uint8_t *n) {

  int i, numDelimiters = 0;
  char delimit[] = {delimiter, '\0'};
  char *s = deepCopy(str);
  for(i=0; i<strlen(str); i++) {
    if(str[i] == delimiter) {
      numDelimiters++;
    }
  }

  char **result = (char **) malloc(sizeof(char**) * (numDelimiters+1));
  char *token = strtok(s, delimit);
  i = 0;
  while(token != NULL) {
    result[i] = deepCopy(token);
    token = strtok(NULL, delimit);
    i++;
  }

  free(s);
  *n = (numDelimiters+1);
  return result;
}

char * deepCopy(const char *s) {
  char *copy = (char *) malloc( (strlen(s) + 1) * sizeof(char) );
  strcpy(copy, s);
  return copy;
}


