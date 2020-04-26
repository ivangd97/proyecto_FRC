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
        bool protocolo;
        int cont;
        char cadcolour[200];
        bool sondeo;
        int line;


    public:

        Gestor();
        void choosePort();

        void chooseVel();

;
        int receiveFrame();

        void processFile();

        void send(char &carE,char msg[],int &size,int &colouro);
                //This topic will divide the message in a little ones which its length is 254

        void manageFrame(HANDLE &portCOM,char msg[],int tamanio,int &field,int &isControlframe,int &colouro,DataFrame fSend);
        void imprimirFrame();


        HANDLE getPortCom();

        void rol();

        void rolMaestro();
        void seleccionMaestro();
        void sondeoMaestro();


        void rolEsclavo();
        void seleccionEsclavo();
        void sondeoEsclavo();


        void cierre();

        virtual ~Gestor();



};

#endif // GESTOR_H
