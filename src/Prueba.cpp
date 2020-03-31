#include "Prueba.h"

Prueba::Prueba()
{
    //ctor
}

Prueba::~Prueba()
{
    //dtor
}


//We receive the frame and manage it
void receiveFrame2(int &field,HANDLE &portCOM,int &isControlFrame) {
    char carR = RecibirCaracter(portCOM);

    unsigned char bce;
    // If our string have any character, it will be shown
    if (carR != 0) {
        //this switch will save the received attributes of a control frame and will build it
        //it will print a message announcing the type of the control frame received
        //With Data Frame modification, this switch will choose between control and data frame, building it to be sent
        switch(field) {

        case 1:

            if (carR==22) {
                controlReceive.setS(carR);
                fReceive.setS(carR);
                field++; }
            if(carR == '{'){
                    //Receiver know that the following frames will be about file process
                    isFile = true;
            }
            //Receiver know that the file process is ending
            if(carR == '}'){
                outStream.close();
                isFile = false;
                endOfFile = true;
                printf("Fichero Recibido. \n");
                if(log){
                    logStream << "Fichero Recibido. \n";
                }
            }
            break;

        case 2:

            controlReceive.setD(carR);
            fReceive.setD(carR);
            field++;
            break;

        case 3:


            if(carR == 02) {
                fReceive.setC(carR);
                isControlFrame = 0;
            }
            if(carR !=02) {
                controlReceive.setC(carR);
                isControlFrame = 1;
            }
            field++;
            break;

        //Case 4 will print the kind of the control frame received (if it's a control frame, else continue)
        case 4:

            if(isControlFrame == 1) {
                controlReceive.setNT(carR);
                field = 1;

                if(controlReceive.getC() == 05) {
                    printf("Se ha recibido una trama ENQ\n");
                    if(log){logStream <<"Se ha recibido una trama ENQ\n";}}

                else if (controlReceive.getC()==04) {
                    printf("Se ha recibido una trama EOT\n");
                    if(log){logStream <<"Se ha recibido una trama EOT\n";}}
                else if (controlReceive.getC()==06) {
                    printf("Se ha recibido una trama ACK\n");
                    if(log){logStream <<"Se ha recibido una trama ACK\n";}}
                else if  (controlReceive.getC()==21) {
                    printf("Se ha recibido una trama NACK\n");
                    if(log){logStream <<"Se ha recibido una trama NACK\n";}

                    }
            }
            else {
                fReceive.setNT(carR);
                field++;
                 }
            break;

        case 5:

            fReceive.setL((unsigned char)carR);
            field++;

        case 6:

            RecibirCadena(portCOM,fReceive.getData(),(int)fReceive.getL());
            fReceive.insertData('\0');
            field++;
            break;

        case 7:

            fReceive.setBCE((unsigned char) carR);
            field = 1;

            //Here, bce will be calculated based in the received data
            bce = fReceive.calculateBCE();
            if(bce = fReceive.getBCE()) {
            //If bce is well calculated, the data has been received without issues, show data
                if(isFile){
                    //if file process is initialized, instead f show data, the file will be written
                    fReceive.writeFile(outStream,colour,screen,log,logStream);



                }else if (endOfFile){
                    //If the file process is end, we will send the size of the document
                    printf("El fichero recibido tiene un tamanio de %s bytes.\n", fReceive.getData());
                    if(log){logStream <<"El fichero recibido tiene un tamanio de "<<fReceive.getData()<<" bytes.\n";}
                    endOfFile = false;

            }else{
                    fReceive.showData(screen,colour);
                    if(log){logStream.write(fReceive.getData(),fReceive.getL());}
            }
            }else{
                if(isFile){
                    printf("Error en la recepcion de la trama del fichero. \n");
                    if(log){logStream <<"Error en la rececpcion del la trama del fichero. \n";}
                }else{
                    printf("Error en la trama recibida \n");
                    if(log){logStream <<"Error en la trama recibida \n";}
                }
            }
            break;

        default:

            printf("Trama no recibida correctamente. \n");
            if(log){logStream <<"Trama no recibida correctamente.\n";}
            break;

        }
    }
}
