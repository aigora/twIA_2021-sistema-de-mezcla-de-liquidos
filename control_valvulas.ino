
//Se definen las variables que cuentan en SEGUNDOS
unsigned int tRele[]={100, 100, 100}; //Conteo máximo. 
                    //Salida del pin9 = 100 segundos
                    //Salida del pin10 = 100 segundos
                    //Salida del pin11 = 100segundos
unsigned int tBase[]={0, 0, 0};//Contadores de tiempo

//Activacion de los Reles
boolean isRele=false;
boolean rRele[]={false, false, false};
boolean indRele[]={true, false, false};//Vector de la Independencia para temporizar
                                       //de manera separada la salida digital del pin9

//Se definen los pines de salida (Conexion a los Reles):
int sRele[]={9,10,11};

int sMainRele = 12;//Rele principal de la serie de elctroválvulas
//LED intero Arduino UNO=pin 13=LED_BUILTIN
int pinEstado = LED_BUILTIN;//Pin de estado de Arduino

//EN UN PRINCIPIO IBAMOS A USAR PULSADORES QUE ACTIVARAN LAS VALVULAS
int eRele[]={8,7,6};

//Variables de tiempo 
int interval=100; //100 mS
unsigned long previousMillis=0;
unsigned int baseT=0;//base para contar cada 10 equivale 1 segundo.

//Variables de propósito general
int i; 
int numSalidas;
int blinkState = HIGH; 

void setup() {

//Se configuran todos los pines entradas/salidas:
pinMode(pinEstado, OUTPUT);//Indicador de funcionamiento.
pinMode(sMainRele, OUTPUT);//Rele principal

//El numero de salidas se utiliza para controlar las entradas deben ser iguales
//excepto por el rele principal que es independiente.
numSalidas=sizeof(sRele)/sizeof(int);//Saca el tamaño de la variable sRele.
for (i=0;i<numSalidas;i++){
  pinMode(sRele[i], OUTPUT);
  pinMode(eRele[i], INPUT);
}



void loop() {
//Lectura de las entradas:
int pRele[numSalidas];
for (i=0;i<numSalidas;i++){
  pRele[i]=digitalRead(eRele[i]);
}
//Anti-Rebote:
delay(20);

//Reinicio de los temporizadores al presionar un pulsador
for (i=0;i<3;i++){
  if (pRele[i]==LOW){
    tBase[i]=0;
    rRele[i]=true;
    if (isRele==false){
      baseT=0;
    }
    isRele=true; 
  }
}


//Temporización cada 100 mS
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    baseT++;
    if (isRele){//Hay algun rele activo?  
      int tDisable=0;//Contador de apagar el rele principal
      for (i=0;i<numSalidas;i++){//Mirar cual(es) esta(n) activo(s)
        if (rRele[i] && tBase[i]<tRele[i]){
          if(!indRele[i]){
            digitalWrite(sMainRele, LOW);//Activo  
          }else{
            digitalWrite(sMainRele, HIGH);//Inactivo  
          }
          digitalWrite(sRele[i],LOW);//Activo
          if (baseT==10){
            tBase[i]++;//incrementa la cuenta de segundos
          }
        }else{
          digitalWrite(sRele[i], HIGH); //Rele no activo, apagar o dejar apagado.
          tDisable++;//contador de desactivar rele principal
        }
      }
      if (tDisable==numSalidas){//Si tDisable llego al numero de salidas que se manejan
        isRele=false;//Desactivamos la variable general de los reles 
        digitalWrite(sMainRele, HIGH);//Apagar rele principal
      }
      //Este if es exclusivo para el parpadeo del LED interno del Arduino. Encendido, no cuenta
      //Parpadeando, esta temporizando alguna salida
      if (baseT == 5 || baseT == 10){
        if (baseT==10){
          baseT=0;
        }
        if (blinkState==LOW){
          blinkState=HIGH;
        }else{
          blinkState=LOW;
        }
      }
    }else{//Ya no se encuentra en funcionamiento el sistema, dejar LED interno encendido
      blinkState=HIGH;
      if (baseT==10){
        baseT=0;
      }
    }



//Cambiar estado del LED interno, o LED de estado.
    digitalWrite(pinEstado, blinkState);
    
  }else if (currentMillis < previousMillis){
      previousMillis=currentMillis;
  }
  
}
