//============================================================================
//----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------
//---------------------------- CURSO 2019/20 ---------------------------------
//--------Autores: Rubén Costa Barriga e Iván Gonzalez Dominguez--------------
//============================================================================
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include "ControlFrame.h"
#include <iostream>
#include <stdio.h>

//Default constructor where C attribute is set as NULL
ControlFrame::ControlFrame()
{
    S  = 22;
    D  = 'T';
    C  = 02;
    NT = '0';
}

//Constructor which have the C attribute set by the user, switch cases inside this constructor will give the
//control frame it C value.
ControlFrame::ControlFrame(int value)
{
    S  = 22;
    D  = 'T';
    NT = '0';
    switch(value)
    {
    case 1:
        C = 05;
        printf("Trama ENQ \n");

        break;

    case 2:
        printf("Trama EOT \n");
        C= 04  ;
        break;

    case 3:
        printf("Trama ACK \n");
        C= 06;
        break;

    case 4:
        printf("Trama NACK \n");
        C= 21;
        break;
    //If the given number isn't valid, we will repeat the question
    default:
        printf("Trama incorrecta, seleccione de nuevo.\n");
    }
}

//Set C attribute
void ControlFrame::setC(unsigned char value)
{
    this->C = value;
}

//Set D attribute
void ControlFrame::setD(unsigned char value)
{
    this->D = value;
}

//Set S attribute
void ControlFrame::setS(unsigned char value)
{
    this->S = value;
}

//Set NT attribute
void ControlFrame::setNT(unsigned char value)
{
    this->NT = value;
}

//Get C attribute
unsigned char ControlFrame::getC()
{
    return this->C;
}

//Get D attribute
unsigned char ControlFrame::getD()
{
    return this->D;
}

//Get S attribute
unsigned char ControlFrame::getS()
{
    return this->S;
}

//Get NT attribute
unsigned char ControlFrame::getNT()
{
    return this->NT;

}


void ControlFrame::imprimirTramaControl(int opcion){
    switch(opcion){
	case 1:
		switch (C) {
			case 05:
				printf("E %c ENQ %c\n",D,NT);
				break;
			case 04:
				printf("E %c EOT %c\n",D,NT);
				break;
			case 06:
				printf("E %c ACK %c\n",D,NT);
				break;
			case 21:
				printf("E %c NACK %c\n",D,NT);
				break;
			default:

				printf("Trama incorrecta\n");
				break;
			}
		break;
		case 2:
			switch (C) {
				case 05:
					printf("R %c ENQ %c\n",D,NT);
					break;
				case 04:
					printf("R %c EOT %c\n",D,NT);
					break;
				case 06:
					printf("R %c ACK %c\n",D,NT);
					break;
				case 21:
					printf("R %c NACK %c\n",D,NT);
					break;
				default:

					printf("Trama incorrecta\n");
					break;
				}

	}






}
void ControlFrame::changeNT() {
	if (NT == '0') {
		NT = '1';
	} else {
		NT = '0';
	}
}


void ControlFrame::sendControl(HANDLE &portCOM){
    EnviarCaracter(portCOM,S);
    EnviarCaracter(portCOM,D);
    EnviarCaracter(portCOM,C);
    EnviarCaracter(portCOM,NT);


}


void ControlFrame::sendControlFrame(HANDLE &portCOM,bool log,ofstream &logStream,HANDLE pantalla) {
    //Change the color of the screen to not confuse with the color of the file process
    int colour =12 + 0*16;
    pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute (pantalla, colour);
    char controlFrame;
    bool exit = false;
           printf("Trama de control a enviar :\n 0: Salir \n 1: Trama ENQ. \n 2: Trama EOT. \n 3: Trama ACK. \n 4: Trama NACK. \n");
           if(log){logStream<<"Trama de control a enviar :\n 0: Salir \n 1: Trama ENQ. \n 2: Trama EOT. \n 3: Trama ACK. \n 4: Trama NACK. \n";}

    while(!exit) {
        //Interface to choose the type of control frame which the user will send
        controlFrame=getch();
        switch (controlFrame) {
        //0 case will exit
        case '0':
            printf("Saliendo...\n");
            if(log){logStream<<"Saliendo...\n";}
            exit=true;
            break;

        case '1':
            printf("Has enviado Trama ENQ \n");
            if(log){logStream<<"Has enviado Trama ENQ \n";}
            exit = true;
            setC(05);
            EnviarCaracter(portCOM,S);
            EnviarCaracter(portCOM,D);
            EnviarCaracter(portCOM,C);
            EnviarCaracter(portCOM,NT);


            break;

        case '2':
            printf("Has enviado Trama EOT \n");
            if(log){logStream<<"Has enviado Trama EOT \n";}
            exit = true;

            setC(04);

            EnviarCaracter(portCOM,S);
            EnviarCaracter(portCOM,D);
            EnviarCaracter(portCOM,C);
            EnviarCaracter(portCOM,NT);

            break;

        case '3':
            printf("Has enviado Trama ACK \n");
            if(log){logStream<<"Has enviado Trama ACK \n";}
            exit = true;

            setC(06);

            EnviarCaracter(portCOM,S);
            EnviarCaracter(portCOM,D);
            EnviarCaracter(portCOM,C);
            EnviarCaracter(portCOM,NT);

            break;

        case '4':
            printf("Has enviado Trama NACK \n");
            if(log){logStream<<"Has enviado Trama NACK \n";}
            exit = true;
            setC(21);

            EnviarCaracter(portCOM,S);
            EnviarCaracter(portCOM,D);
            EnviarCaracter(portCOM,C);
            EnviarCaracter(portCOM,NT);

            break;

        default:
            printf("Trama incorrecta, seleccione de nuevo.\n");
            if(log){logStream<<"Trama incorrecta, seleccione de nuevo.\n";}
            break; }

    }
}
