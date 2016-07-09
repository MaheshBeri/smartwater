/*
   Flow Sensor Tutorial 
   http://www.roboindia.com/tutorials/flow-sensor   
   Show the readings from flow sensor on Serial Monitor in L/H.


Circuit:

Sensor Singnal Pin ( Yellow ) : Arduino Pin 2
Sensor Black wire: Arduino +5V 
Sensor Red wire: Arduino GND 


*/

volatile int  flow_frequency;  // Measures flow meter pulses
unsigned int  l_hour;          // Calculated litres/hour                      
unsigned char flowmeter = 2;  // Flow Meter Pin number
unsigned long currentTime;
unsigned long cloopTime;
const int buttonPin = 7;     // the number of the pushbutton pin
const int buttonPinClose = 8;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
boolean stopSignal= false;  
boolean startSignal= false;  

String inData;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int buttonStateClose = 0;         // variable for reading the pushbutton status
void flow ()                  // Interruot function
{ 
   flow_frequency++;
} 

int solenoidPin = 4; 
void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  pinMode(solenoidPin, OUTPUT);           //Sets the pin as an output

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
    // initialize the pushbutton pin as an input:
  pinMode(buttonPinClose, INPUT);
 
   pinMode(flowmeter, INPUT);
   Serial.begin(19200); 
   attachInterrupt(0, flow, RISING); // Setup Interrupt 
                                     // see http://arduino.cc/en/Reference/attachInterrupt
   sei();                            // Enable interrupts  
   currentTime = millis();
   cloopTime = currentTime;
}

void loop() {
  // put your main code here, to run repeatedly:
// put your main code here, to run repeatedly:  
 // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // read the state of the pushbutton value:
  buttonStateClose = digitalRead(buttonPinClose);

//Serial.print("Loop start ..");

while (Serial.available() > 0){
    char recieved = Serial.read();
    inData += recieved; 
    // Process message when new line character is recieve6d
     if (recieved == '\n'){
            //Serial.print("Arduino Received: ");
            //Serial.print(inData);
            if(inData.equals("STOP\n")){             
                stopSignal=true;
            }
            if(inData.equals("START\n")){             
                startSignal=true;
            }            
             inData = ""; // Clear recieved buffer
        }     
    
  }

   // if it is, the buttonState is HIGH:
  if (buttonState == HIGH  || startSignal==true) {     
    //open valve   
        digitalWrite(ledPin, HIGH);  
        digitalWrite(solenoidPin, HIGH);    //Switch Solenoid ON
        stopSignal=false;
        startSignal=false;
    }
    if (buttonStateClose == HIGH || stopSignal==true) {     
    //Close valve   
         digitalWrite(ledPin, LOW);      
         digitalWrite(solenoidPin, LOW);   //Switch Solenoid OFF
         stopSignal=false;
         startSignal=false;
  }
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
 //delay(1000);
  currentTime = millis();
  //Serial.print("Current time ");
  //Serial.println(currentTime);            
  //Serial.print("Loop time ");
  //Serial.println(cloopTime);            
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {     
      cloopTime = currentTime;              // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      flow_frequency = 0;                   // Reset Counter
      Serial.print(l_hour, DEC);            // Print litres/hour
      Serial.println("L");
   }  
}
