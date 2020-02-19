#include "ControlFrame.h"
#include <iostream>

//Default constructor where C attribute is set as NULL
ControlFrame::ControlFrame()
{
    S  = 22;
    D  = 'T';
    C  = 02;
    NT = '0';
}

//Constructor which have the C attribute set by the user
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

    default:
        printf("Trama incorrecta, seleccione de nuevo.\n");
    }
}

void ControlFrame::setC(unsigned char value)
{
    this->C = value;

}
void ControlFrame::setD(unsigned char value)
{
    this->D = value;


}
void ControlFrame::setS(unsigned char value)
{
    this->S = value;

}
void ControlFrame::setNT(unsigned char value)
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

