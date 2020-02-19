#include "ControlFrame.h"
#include <iostream>

//Defaukt constructor where C attribute is set as NULL
ControlFrame::ControlFrame()
{
    S  = 22;
    D  = 'T';
    C  = ' ';
    NT = '0';
}
//Revisar
//Constructor which have the C attribute set by the user
ControlFrame::ControlFrame(char value)
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
        C=04;
        break;

    case 3:
        printf("Trama ACK \n");
        C=06;
        break;

    case 4:
        printf("Trama NACK \n");
        C=21;
        break;

    default:
        printf("Trama incorrecta, seleccione de nuevo.\n");
        break;
    }
}
/*unsigned char ControlFrame::getAtribute()
{

    return this->C;


}*/
void ControlFrame::setC(char value)
{
    this->C = value;

}
void ControlFrame::setD(char value)
{
    this->D = value;


}
void ControlFrame::setS(char value)
{
    this->S = value;

}
void ControlFrame::setNT(char value)
{

    this->NT = value;
}


unsigned char ControlFrame::getC()
{
    return this->C;
}
unsigned char ControlFrame::getD()
{
    return this->D;
}
unsigned char ControlFrame::getS()
{
    return this->S;
}

unsigned char ControlFrame::getNT()
{
    return this->NT;
}


void ControlFrame::setAtribute(char value,int atribute)
{
    switch(atribute)
    {
    case 1:
        this->S = value;
        break;
    case 2:
        this->D = value;
        break;
    case 3:
        this->C = value;
        break;
    case 4:
        this->NT = value;
        break;
    }
}


