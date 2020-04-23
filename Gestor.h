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

    public:

        Gestor();
        void choosePort(char PSerie[]);

        int chooseVel();

        void receiveFrame2(int &field,HANDLE portCOM,int &isControlFrame,bool &recibir);

        void receiveFrame(int &field,HANDLE portCOM,int &isControlFrame,int &colouro);

        void processFile();

        void send(char carE,char msg[],int &size,HANDLE &portCOM,int &colouro);















        virtual ~Gestor();



};

#endif // GESTOR_H
