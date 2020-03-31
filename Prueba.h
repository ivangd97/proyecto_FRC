#ifndef PRUEBA_H
#define PRUEBA_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "PuertoSerie.h"
#include "ControlFrame.h"
#include "DataFrame.h"


#define ESC_KEY (27)
#define RETURN_KEY (8)
#define INTRO_KEY (13)
#define F1 (59)
#define F2 (60)
#define F3 (61)
#define F5 (63)

ifstream inStream2;
ofstream outStream2;
ofstream logStream2;

int limit2 = 802;
ControlFrame controlReceive2;
int i2 = 0;
HANDLE screen2;
int colour2 = 0;
bool isFile2 = false;
bool endOfFile2 = false;
char stringAux2[255];
bool log2 = false;
int field2=1;
int isControlFrame2;

// Simultaneous Read/Write of characters :
int size2 = 0;

using namespace std;

class Prueba
{
    public:
        Prueba();
        virtual ~Prueba();

        void receiveFrame2(int &field2,HANDLE &portCOM2,int &isControlFrame2);

    protected:

    private:

};

#endif // PRUEBA_H
