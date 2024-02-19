
 /*
 * This is the Arduino code Ublox NEO-6M GPS module
 * this code extracts the GPS latitude and longitude so it can be used for other purposes

 * 
 * Written by Ahmad Nejrabi for Robojax Video
 * Date: Jan. 24, 2017, in Ajax, Ontario, Canada
 * Permission granted to share this code given that this
 * note is kept with the code.
 * Disclaimer: this code is "AS IS" and for educational purpose only.
 * 
 */
//***************************Libraries and initializaitons****************************************************
#include <OneWire.h>
#include <DallasTemperature.h>


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

int len = 102;
char result[102]=""; // maximum length without pH sensor is 75


//...................................pH...............................

const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

// .........................................GPS.................................
String inputString = ""; // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
String signal = "$GPGLL";


//.............Water Temp Sensor................................

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celsius = 0;
float Fahrenheit = 0;

//...............Water Flow Sensor.............................

volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}

//...............TDS Gravity Temperature ppm..................................

#define TdsSensorPin A1
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;

//...............Temp Pressure..................................


#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);



//***************************SETUP****************************************************

void setup() {
    // initialize serial:
    Serial.begin(9600);
    result[0] = 'A';

    result[len-1] = 'Z';
      for (int i=1;i<len-2;i++){
      result[i] = '0';
    }

 

    //.................GPS.............................
    
    // reserve 200 bytes for the inputString:
    inputString.reserve(200);

    //....................Water temp......................
   sensors.begin();

       //....................Water Flow.....................
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;

   //...............TDS Gravity Temperature ppm..................................
   pinMode(TdsSensorPin,INPUT);
   
//...............Temp Pressure..................................
  //Serial.println(F("BMP280 test"));

  if (!bmp.begin(0x76)) {
    //Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */



//...................................pH...............................
//nothing for setup
   
}

//***************************FUNCTIONS****************************************************


// .........................................GPS.................................
void gps(){
  
  
     // print the string when a newline arrives:
    if (stringComplete) {
        
        String BB = inputString.substring(0, 6);
        if (BB == signal) {
            String LAT = inputString.substring(7, 17);
            int LATperiod = LAT.indexOf('.');
            int LATzero = LAT.indexOf('0');
            if (LATzero == 0) {
                LAT = LAT.substring(1);
            }

            String LON = inputString.substring(20, 31);
            int LONperiod = LON.indexOf('.');
            int LONTzero = LON.indexOf('0');
            if (LONTzero == 0) {
                LON = LON.substring(1);
            }
            String LATdir = inputString.substring(18,19);
            String LONdir = inputString.substring(32,33);
            //Serial.print("Latitude: ");
            //Serial.print(LAT);
            //Serial.println(LATdir);
            //Serial.print("Longitude: ");
            //Serial.print(LON);
            //Serial.println(LONdir);
            //Serial.println();
          
            int LATlen = LAT.length();
            int LONlen = LON.length();
            //Serial.print("LAT length ");
            //Serial.println(LAT.length());
            //Serial.print("LON length: ");
            //Serial.println(LON.length());

            result[1]=',';
            result[5]=',';
            result[20]=',';
            int j=1;
            //result[2]=LAT[0];
            //result[3]=LAT[1];
            //result[4]=LAT[2];
            //for (int i=12;i>=2;i--){
            for (int i=4;i>=2;i--){  
              if (j<=LATlen-8){
                result[i]=LAT[LATlen-8-j];
                j+=1;
              }
            }
            int l=1;
            //for (int i=12;i>=2;i--){
            for (int i=13;i>=6;i--){  
              if (l<=LATlen-2){
                result[i]=LAT[LATlen-l];
                l+=1;
              }
            }

            //result[17]=LON[0];
            //result[18]=LON[1];
            //result[19]=LON[2];
            int m=1;
            //for (int i=12;i>=2;i--){
            for (int i=19;i>=17;i--){  
              if ( m<=LONlen-8){
                result[i]=LON[LONlen-8-m];
                m+=1;
              }
            }
            int k=1;
            for (int l=28;l>=21;l--){ 
              if (k<=LONlen-2){
                result[l]=LON[LONlen-k];
                k+=1;
              }
            }
            result[14]=',';
            result[15]=LATdir[0];
            result[16]=',';
            result[29]=',';
            result[30]=LONdir[0];
            result[31]=',';
            
            water_temp();
            water_flow();
            tds_ppm();
            temp_pressure_bmp280();
            pH();

            for (int i=0;i<len;i++){
              Serial.print(result[i]);
            }
            
            Serial.println();
            //Serial.println("=================READING COMPLETE======================");
            //Serial.println();
        }

        // Serial.println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }
}



//...............Water Temp.................................
void water_temp(){
  //Serial.println();
  //Serial.println("Water Temperature");
  sensors.requestTemperatures();
  String Cel="000000";
  String Fah="000000";
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);
  
  //Serial.print("Celsius length: ");
  //Serial.println(Cel.length());    //  4
  //Serial.print("Fahrenheit length: ");
  //Serial.println(Fah.length());     // 4

  Cel+=String(Celsius,2);
  Fah+=String(Fahrenheit,2);
  //Serial.println("CEL: ");
  //Serial.println(Cel);
  //Serial.println("FAH: ");
  //Serial.println(Fah);
  int Cellen=Cel.length();
  int Fahlen=Fah.length();
  result[38]='*';
  result[39]='C';
  result[40]=',';
  result[47]='*';
  result[48]='F';
  result[49]=',';
  int jj=1;
  for (int i=37;i>=32;i--){ 
    if (jj<=Cellen){
      result[i]=Cel[Cellen-jj];
      jj+=1;
    }
  }
  int kk=1;
  for (int i=46;i>=41;i--){ 
    if (kk<=Fahlen){
      result[i]=Fah[Fahlen-kk];
      kk+=1;
    }
  }

            
  //Serial.print(Celsius);
  //Serial.print(" C  ");
  //Serial.print(Fahrenheit);
  //Serial.println(" F");

  
}

//......................................Water Flow...........................
void water_flow(){
    //Serial.println();
   //Serial.println("Water Flow");
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
      //Serial.print(l_hour); // Print litres/hour
      //Serial.println(" L/hour");

      String Waterflow="000000";
      Waterflow+=String(l_hour);
      //Serial.println("WaterFlow: ");
      //Serial.println(Waterflow);
      int WFlen=Waterflow.length();
      
      int jjj=1;
      for (int i=55;i>=50;i--){ 
        if (jjj<=WFlen){
          result[i]=Waterflow[WFlen-jjj];
          jjj+=1;
        }
      }
      result[56]=',';
   }
}


//...............TDS Gravity Temperature ppm..................................

void tds_ppm(){
  //Serial.println();
  //Serial.println("TDS PPM");
     static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      //Serial.print("TDS Value:");
      //Serial.print(tdsValue);
      //Serial.println("ppm");


      String TDS="0000000";
      TDS+=String(tdsValue);
      //Serial.println("TDS Value String: ");
      //Serial.println(TDS);
      int TDSlen=TDS.length();
      
      int kkk=1;
      for (int i=63;i>=57;i--){ 
        if (kkk<=TDSlen){
          result[i]=TDS[TDSlen-kkk];
          kkk+=1;
        }
      }
      result[64]=',';
   }
}
int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

//...............Temp Pressure..................................

void temp_pressure_bmp280(){
  //Serial.println();
  //Serial.println("TEMP PRESSURE BMP280");
  //Serial.print(F("Temperature = "));
    //Serial.print(bmp.readTemperature());
    //Serial.println(" *C");

    //Serial.print(F("Pressure = "));
    //Serial.print(bmp.readPressure());
    //Serial.println(" Pa");

    //Serial.print(F("Approx altitude = "));
    //Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    //Serial.println(" m");

    //Serial.println();
    delay(2000);

    result[72]='*';
    result[73]='C';
    result[74]=',';
    String Temperature="0000000";
    String Pressure="000000000000";
    String Altitude="000000";
    Temperature+=String(bmp.readTemperature());
    Pressure+=String(bmp.readPressure());
    Altitude+=String(bmp.readAltitude());
    //Serial.println("Temperature String: ");
    //Serial.println(Temperature);
    //Serial.println("Pressure String: ");
    //Serial.println(Pressure);
    //Serial.println("Altitude String: ");
    //Serial.println(Altitude);
    
    int Tmplen=Temperature.length();
    int Prelen=Pressure.length();
    int Altlen=Altitude.length();
    
    int jjjj=1;
    for (int i=71;i>=65;i--){ 
      if (jjjj<=Tmplen){
        result[i]=Temperature[Tmplen-jjjj];
        jjjj+=1;
      }
    }
    int kkkk=1;
    for (int i=86;i>=75;i--){ 
      if (kkkk<=Prelen){
        result[i]=Pressure[Prelen-kkkk];
        kkkk+=1;
      }
    }
    result[87]=',';
    int llll=1;
    for (int i=93;i>=88;i--){ 
      if (llll<=Altlen){
        result[i]=Altitude[Altlen-llll];
        llll+=1;
      }
    }
    result[94]=',';
}


//...................................pH...............................

void pH(){
  for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -4.5 * pHVol + 22.25;
 //Serial.print("sensor = ");
 //Serial.println(phValue);

      String pH="0000000";
      pH+=String(phValue);
      //Serial.println("TDS Value String: ");
      //Serial.println(TDS);
      int pHlen=pH.length();
      
      int lk=1;
      for (int i=99;i>=95;i--){ 
        if (lk<=pHlen){
          result[i]=pH[pHlen-lk];
          lk+=1;
        }
      }
      result[100]=',';

 delay(20);
}

//***********************************LOOP********************************
void loop() {
    // print the string when a newline arrives:
gps();
//water_temp();


//delay(500);
}

/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX. This routine is run between each
time loop() runs, so using delay inside loop can delay
response. Multiple bytes of data may be available.
*/
void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}
