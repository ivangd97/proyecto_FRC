#include "Prueba.h"

Prueba::Prueba()
{
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
                controlReceive2.setS(carR);
                fReceive2.setS(carR);
                field2++; }
            if(carR == '{'){
                    //Receiver know that the following frames will be about file process
                    isFile2 = true;
            }
            //Receiver know that the file process is ending
            if(carR == '}'){
                outStream2.close();
                isFile2 = false;
                endOfFile2 = true;
                printf("Fichero Recibido. \n");
                if(log2){
                    logStream2 << "Fichero Recibido. \n";
                }
            }
            break;

        case 2:

            controlReceive2.setD(carR);
            fReceive2.setD(carR);
            field2++;
            break;

        case 3:


            if(carR == 02) {
                fReceive2.setC(carR);
                isControlFrame2 = 0;
            }
            if(carR !=02) {
                controlReceive2.setC(carR);
                isControlFrame2 = 1;
            }
            field2++;
            break;

        //Case 4 will print the kind of the control frame received (if it's a control frame, else continue)
        case 4:

            if(isControlFrame2 == 1) {
                controlReceive2.setNT(carR);
                field2 = 1;

                if(controlReceive2.getC() == 05) {
                    printf("Se ha recibido una trama ENQ\n");
                    if(log2){logStream2 <<"Se ha recibido una trama ENQ\n";}}

                else if (controlReceive2.getC()==04) {
                    printf("Se ha recibido una trama EOT\n");
                    if(log2){logStream2 <<"Se ha recibido una trama EOT\n";}}
                else if (controlReceive2.getC()==06) {
                    printf("Se ha recibido una trama ACK\n");
                    if(log2){logStream2 <<"Se ha recibido una trama ACK\n";}}
                else if  (controlReceive2.getC()==21) {
                    printf("Se ha recibido una trama NACK\n");
                    if(log2){logStream2<<"Se ha recibido una trama NACK\n";}

                    }
            }
            else {
                fReceive2.setNT(carR);
                field2++;
                 }
            break;

        case 5:

            fReceive2.setL((unsigned char)carR);
            field2++;

        case 6:

            RecibirCadena(portCOM,fReceive2.getData(),(int)fReceive2.getL());
            fReceive2.insertData('\0');
            field++;
            break;

        case 7:

            fReceive2.setBCE((unsigned char) carR);
            field = 1;

            //Here, bce will be calculated based in the received data
            bce = fReceive2.calculateBCE();
            if(bce = fReceive2.getBCE()) {
            //If bce is well calculated, the data has been received without issues, show data
                if(isFile2){
                    //if file process is initialized, instead f show data, the file will be written
                    fReceive2.writeFile(outStream2,colour2,screen2,log2,logStream2);



                }else if (endOfFile2){
                    //If the file process is end, we will send the size of the document
                    printf("El fichero recibido tiene un tamanio de %s bytes.\n", fReceive2.getData());
                    if(log2){logStream2 <<"El fichero recibido tiene un tamanio de "<<fReceive2.getData()<<" bytes.\n";}
                    endOfFile2 = false;

            }else{
                    fReceive2.showData(screen2,colour2);
                    if(log2){logStream2.write(fReceive2.getData(),fReceive2.getL());}
            }
            }else{
                if(isFile2){
                    printf("Error en la recepcion de la trama del fichero. \n");
                    if(log2){logStream2 <<"Error en la rececpcion del la trama del fichero. \n";}
                }else{
                    printf("Error en la trama recibida \n");
                    if(log2){logStream2 <<"Error en la trama recibida \n";}
                }
            }
            break;

        default:

            printf("Trama no recibida correctamente. \n");
            if(log2){logStream2 <<"Trama no recibida correctamente.\n";}
            break;

        }
    }
}
