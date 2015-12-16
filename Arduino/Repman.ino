#include <CapPin.h>
/* 
  This sketch reads the acceleration from the Bean's on-board accelerometer. 
  
  The acceleration readings are sent over serial and can be accessed in Arduino's Serial Monitor.
  
  To use the Serial Monitor, set Arduino's serial port to "/tmp/tty.LightBlue-Bean"
  and the Bean as "Virtual Serial" in the OS X Bean Loader.
    
  This example code is in the public domain.
*/

#define INT_MAX 32767
#define INT_MIN -32768

const int numReadings = 5;

int readingsX[numReadings];      // the readings from the analog input
int readingsY[numReadings];
int readingsZ[numReadings];
bool firstRepRegistered[3] = {0, 0, 0};
int distances[3] = {INT_MIN, INT_MIN, INT_MIN};
int data[10];
int dataIdx = 0;                // used ONLY to initialize data
int readIndex = 0;              // the index of the current reading
int totalX = 0;                 // the running total
int totalY = 0;
int totalZ = 0;
int average = 0;                // the average

int reps = 0;
int maxReps = 10;
int lastRepIdx = 0;
int currIdx = 0;
bool seenRep = 0;
bool pressed = 0;
int seenPress = 0;
bool on = false;
int count = 0;
int sets = 0;
int rest = 10000;
char rps[10];
char s[10];
char r[10];

bool sleeping = 0;

//Kiran: Changed 13 to 20 because of problems with shoulder press counting double
int distThresh = 20;
int peakThresh = INT_MAX;

CapPin cPin_4 = CapPin(4);   // read pin 4

void setup() {
  // Bean Serial is at a fixed baud rate. Changing the value in Serial.begin() has no effect.
  //original Serial.begin(); 
  Serial.begin(115200);
  Serial.println("start");
  pinMode(3, OUTPUT);  
  pinMode(2, OUTPUT);
  //pinMode(1, OUTPUT);
  //digitalWrite(2, HIGH);
  //digitalWrite(1, LOW);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsX[thisReading] = 0;
    readingsY[thisReading] = 0;
    readingsZ[thisReading] = 0;
  }

  // zero init data
  for(int i = 0; i < 10; i++)
    data[i] = 0;
  // Optional: Use Bean.setAccelerationRange() to set the sensitivity to something other than the default of ±2g.
}

void reset() {
  memset(readingsX, 0, sizeof(readingsX));
  memset(readingsY, 0, sizeof(readingsY));
  memset(readingsZ, 0, sizeof(readingsZ));
  memset(data, 0, sizeof(data));
  totalX = 0;
  totalY = 0;
  totalZ = 0;
  readIndex = 0;
  dataIdx = 0;
  average = 0;
  reps = 0;

  lastRepIdx = 0;
  currIdx = 0;
  seenRep = 0;
  pressed = 0;
  count = 0;
}

void modifyData() {
  if (dataIdx < 10) {
    data[dataIdx] = totalY/numReadings;
    dataIdx ++;
  } else {
    for(int i = 0; i < 9; i++) {
      data[i] = data[i+1];
    }
    data[9] = totalY/numReadings;
  }
}

int avgMiddle() {
  int tot = 0;
  for(int i = 3; i < 8; i++) {
    tot += data[i];
  }
  
  return tot/5;
}

void buzz(int ms) {
  digitalWrite(3, HIGH);
  //tone(2, 2000);
  delay(ms);
  digitalWrite(3, LOW);
  //noTone(2);
  delay(ms);
}

void handleFinish() {
  sets--;
  buzz(500);
  if (sets == 0)
  {
    buzz(300);
    buzz(300);
    buzz(300);
  }
  on = false;
  reset();
  sleeping = 1;
  if (sets > 0)
  {
    Bean.sleep(rest);
    buzz(300);
    buzz(300);
    buzz(300);
  }
  on = false;
  seenPress = 0;
  sleeping = 0;
}


void checkForRep() {
  if (dataIdx < 10) return;
  int avg = avgMiddle();

  // check that the tail of the list is less than head of list
  bool isPeak = data[9] < data[0] && avg > data[9] + 10 && 
                avg > data[0] + 10;
                
  // check that middle is greater than peak threshold
  bool validPeak = peakThresh == INT_MAX || avg > peakThresh;
//  Serial.println(avg);
//  Serial.println(peakThresh);

  // check that current peak is not too close to last peak
  bool peakSig = (currIdx - lastRepIdx) >= distThresh;

  //Serial.print(seenRep); Serial.print(isPeak); Serial.print(validPeak); Serial.println(peakSig);

  if(!seenRep && isPeak && validPeak && peakSig) {
    reps ++;
    Serial.write(reps);
    lastRepIdx = currIdx;
    seenRep = 1;
    if (reps == maxReps) {
      handleFinish();
    }
    if (peakThresh == INT_MAX) peakThresh = data[9];
    else peakThresh = (peakThresh + data[9]) / 2;
  }

  if(data[0] > avg && avg > data[9])
    seenRep = 0;
}

void loop() {

  if (sets == 0) {
        char input[64];
    size_t length = 64;       
    length = Serial.readBytes(input, length);
    if (length > 0)
    {
      Serial.write(input);
      sscanf(input, "%[^','],%[^','],%[^',']", rps, s, r);
      maxReps = atoi(rps);
      sets = atoi(s);
      rest = atoi(r)*1000;
      Serial.write(maxReps);
      Serial.write(sets);
      Serial.write(rest);
      reset();
    }
    Serial.flush();   
  }
  else {    
    long total =  cPin_4.readPin(2000);
    //Serial.print("Cap \t");
    //Serial.print(total);
    //Serial.println("");
    if (total > 100 && pressed == 0)
      pressed = 1;
  
    if (total < 5 && pressed == 1)
    {
      pressed = 0;
      seenPress++;
    }
  
    if (seenPress%2 == 1 && !sleeping && on == false)
    {
      on = true;
      seenPress = 0;
      buzz(500);
      //Serial.println(on);
    }
  
    if (on) {
      //Serial.println(pressed);
      //Serial.print("num");
      //Serial.println(seenPress);
      // subtract the last reading:
      totalX = totalX - readingsX[readIndex];
      totalY = totalY - readingsY[readIndex];
      totalZ = totalZ - readingsZ[readIndex];
      // read from the sensor:
      // Get the current acceleration with range of ±2g, and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units. 
      AccelerationReading acceleration = Bean.getAcceleration();
      readingsX[readIndex] = acceleration.xAxis;
      readingsY[readIndex] = acceleration.yAxis;
      readingsZ[readIndex] = acceleration.zAxis;
      // add the reading to the total:
      totalX = totalX + readingsX[readIndex];
      totalY = totalY + readingsY[readIndex];
      totalZ = totalZ + readingsZ[readIndex];
      // advance to the next position in the array:
      readIndex = readIndex + 1;
      
      // Format the serial output like this:    "X: 249  Y: -27   Z: -253"
      String stringToPrint = String();
    
      // if we're at the end of the array...
      if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
      }
    
      if(currIdx > 3) {
        modifyData();
        checkForRep();
      }
    
      currIdx ++;
    }
  }

}
