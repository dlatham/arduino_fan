/* 
  ADAPTED FROM:
  Transmit sketch - RF Calibration
     Written by ScottC 17 July 2014
     Arduino IDE version 1.0.5
     Website: http://arduinobasics.blogspot.com
     Transmitter: FS1000A/XY-FST
     Description: A simple sketch used to calibrate RF transmission.          
 ------------------------------------------------------------- */

 #define rfTransmitPin 4  //RF Transmitter pin = digital pin 4
 #define ledPin 13        //Onboard LED = digital pin 13
 #define offPin 6      //The button being used to turn stuff off
 #define loPin 7
 #define medPin 8
 #define hiPin 9
 
 const int codeSize = 25;      //The size of the code to transmit
 int codeToTransmit[codeSize]; //The array used to hold the RF code
 int fanOFF[]={4,4,4,4,4,4,4,4,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,6}; //The RF code that will turn the fan OFF
 int fanLOW[]={4,4,4,4,4,4,4,4,5,2,2,2,2,2,2,2,2,2,2,2,2,2,1,5,3}; //The RF code that will turn the fan on LOW
 int fanMED[]={4,4,4,4,4,4,4,4,5,2,2,2,2,2,2,2,2,2,2,2,1,5,4,4,3}; //The RF code that will turn the fan on MED (3)
 int fanHI[]={4,4,4,4,4,4,4,4,5,2,2,2,2,2,2,2,2,1,5,2,2,2,2,2,3};
 int codeToggler = 0;  //Used to switch between turning the light ON and OFF
 int timeDelay=100;      // The variable used to calibrate the RF signal lengths.

 
 
 void setup(){
   Serial.begin(9600);        // Turn the Serial Protocol ON
   pinMode(rfTransmitPin, OUTPUT);   //Transmit pin is an output  
   pinMode(ledPin, OUTPUT); 
   pinMode(offPin, INPUT);
   pinMode(loPin, INPUT);
   pinMode(medPin, INPUT);
   pinMode(hiPin, INPUT);
  
 //LED initialisation sequence - gives us some time to get ready
  digitalWrite(ledPin, HIGH); 
  delay(3000);
  digitalWrite(ledPin, LOW); 
  delay(1000);
 }
 
 
 
  void loop(){
    //Select the appropriate code to queue for send based on the active digital pin
    if(digitalRead(offPin)==HIGH){
        toggleCode(0);    // switch between fan of and lo
        transmitCode();  // transmit the code to RF receiver on the Fan/Light
    }
    if(digitalRead(loPin)==HIGH){
        toggleCode(1);    // switch between fan of and lo
        transmitCode();  // transmit the code to RF receiver on the Fan/Light
    }
    if(digitalRead(medPin)==HIGH){
        toggleCode(2);    // switch between fan of and lo
        transmitCode();  // transmit the code to RF receiver on the Fan/Light
    }
    if(digitalRead(hiPin)==HIGH){
        toggleCode(3);    // switch between fan of and lo
        transmitCode();  // transmit the code to RF receiver on the Fan/Light
    }
    //timeDelay+=10;    //Increment the timeDelay by 10 microseconds with every transmission
    //delay(5000);      //Each transmission will be about 5 seconds apart.
  }
  
  
  
  
  /*---------------------------------------------------------------- 
     toggleCode(): Used to queue the appropriate code for send into codeToTransmit() array
  -----------------------------------------------------------------*/
  void toggleCode(int x){
    switch(x){
      case 0:
        for(int i = 0; i<codeSize; i++){
           codeToTransmit[i]=fanOFF[i];
        }
      case 1:
        for(int i = 0; i<codeSize; i++){
           codeToTransmit[i]=fanLOW[i];
        }
      case 2:
        for(int i = 0; i<codeSize; i++){
           codeToTransmit[i]=fanMED[i];
        }
      case 3:
        for(int i = 0; i<codeSize; i++){
           codeToTransmit[i]=fanHI[i];
        }
    }
  }
   
   
   
   
  /*-----------------------------------------------------------------
    transmitCode(): Used to transmit the signal to the RF receiver on
                    the fan/light. There are 6 different HIGH-LOW signal combinations. 
                    
                    SH = short high   or  LH = long high   
                                     PLUS
         SL = short low    or    LL = long low    or    VLL = very long low
                    
  -------------------------------------------------------------------*/
   void transmitCode(){
    // The LED will be turned on to create a visual signal transmission indicator.
    digitalWrite(ledPin, HIGH);
   
   //initialise the variables 
    int highLength = 0;
    int lowLength = 0;
    
    //The signal is transmitted 8 times in succession - this may vary with your remote.       
    for(int j = 0; j<8; j++){
      for(int i = 0; i<codeSize; i++){ 
        switch(codeToTransmit[i]){
          case 1: // SH + SL
            highLength=3;
            lowLength=5;
          break;
          case 2: // SH + LL
            highLength=3;
            lowLength=10;
          break;
          case 3: // SH + VLL
            highLength=3;
            lowLength=52;
          break;
          case 4: // LH + SL
            highLength=8;
            lowLength=5;
          break;
          case 5: // LH + LL
            highLength=8;
            lowLength=10;
          break;
          case 6: // LH + VLL
            highLength=8;
            lowLength=52;
          break;
        }
           
         /* Transmit a HIGH signal - the duration of transmission will be determined 
            by the highLength and timeDelay variables */
         digitalWrite(rfTransmitPin, HIGH);     
         delayMicroseconds(highLength*timeDelay); 
         
         /* Transmit a LOW signal - the duration of transmission will be determined 
            by the lowLength and timeDelay variables */
         digitalWrite(rfTransmitPin,LOW);     
         delayMicroseconds(lowLength*timeDelay);  
      }
    }
    //Turn the LED off after the code has been transmitted.
    digitalWrite(ledPin, LOW); 
 }
