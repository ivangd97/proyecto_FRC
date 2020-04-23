#ifndef GESTOR_H
#define GESTOR_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "ControlFrame.h"
#include "DataFrame.h"



using namespace std;


class Gestor
{


    private:
        int limit;
        DataFrame fReceive,fSend;
        ControlFrame controlReceive,controlSend;
        HANDLE screen;
        int colouro;
        bool isFile;
        bool endOfFile;
        char stringAux[802];
        bool log;
        int field;
        int i;
        int isControlFrame;
        bool flag;
        int size;
        HANDLE portCOM;
        char PSerie[5];

    public:

        Gestor();
        void choosePort();

        void chooseVel();

        void receiveFrame2(int &field,int &isControlFrame,bool &recibir);
;
        void receiveFrame(int &field,int &isControlFrame,int &colouro);

        void processFile();

        void send(char &carE,char msg[],int &size,int &colouro);

        HANDLE getPortCom();




        virtual ~Gestor();



};

#endif // GESTOR_H
