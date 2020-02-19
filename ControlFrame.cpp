#include "ControlFrame.h"

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
	 switch(value){
         case 1:
            printf("Trama ENQ \n");
            exit = true;
            return ENQ;
            break;

        case 2:
            printf("Trama EOT \n");
            exit = true;
            return EOT;
            break;

        case 3:
            printf("Trama ACK \n");
            exit = true;
            return ACK;
            break;

        case 4:
            printf("Trama NACK \n");
            exit = true;
            return NACK;
            break;

        default:
            printf("Trama incorrecta, seleccione de nuevo.\n");
            break;
	 }
}
