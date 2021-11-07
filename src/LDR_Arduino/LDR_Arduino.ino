/* LDR Color Sensor(Color Detect) v2.0
   Code by : Anuradha Gunawardhana
           : Dinuka Bandara
   Date    : 09:05:2016
 */


#include <Adafruit_NeoPixel.h>    // Include Adafruit NeoPixel library
#define ledPin          5         // LED on digital pin 3
#define NUMLEDS         12        // Total number of LEDs
#define wait            5         // Delay between each color(standard)
#define readingDelay    1         // Delay between two readings

boolean calibration = false;
int rgb[3] = {0, 0, 0};
int average;
//byte rdClrX;
//byte rdClrY;
//byte rdClrZ;
String clrOut;

Adafruit_NeoPixel led = Adafruit_NeoPixel(NUMLEDS, ledPin, NEO_GRB + NEO_KHZ800);       // Create 'led' object to drive LED

int whiteBalance[5][8][3] = {
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
};

int blackBalance[5][8][3] = {
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
};

int colorOut[5][8][3] = {
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
  {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
};


//int temp[5][8] = {
//  {0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0},
//};

byte k[8][3] = {
  {0, 0, 0},
  {0, 0, 1},
  {0, 1, 0},
  {0, 1, 1},
  {1, 0, 0},
  {1, 0, 1},
  {1, 1, 0},
  {1, 1, 1},
};


void setup() {
  led.begin();                                      // Initialize 'led' object
  for (byte k = 0; k < 12; k++) {
    led.setPixelColor(k, led.Color(0, 0, 0));   // Turn off the LED
    led.show();                                  // Show new values
  }
  Serial.begin(9600);
  for (byte i = 0; i < 6; i++) {
    pinMode(i, OUTPUT);
    //    Serial.println("Done");
  }
  // getReagings(times, 0-W||1-B||2-C)
  getReadings(20, 0);
  led.setPixelColor(1, led.Color(255, 255, 0));   // For debug
  led.show();
  delay(5000);
  getReadings(20, 1);
  led.setPixelColor(1, led.Color(255, 255, 0));   // Turn off the LED
  led.show();
  delay(5000);
  getReadings(10, 2);
  led.setPixelColor(1, led.Color(255, 255, 0));  // Turn off the LED
  led.show();
  delay(5000);

  //printWhite();
  // printBlack();
  // printClr();

  int row = 0;
  int col = 0;
  for (byte i = 0; i < 5; i++) {


    for ( byte u = 0; u < 8; u++) {
      int color_mat[3] ;
      col++;
      for (byte p = 0; p < 3; p++) {
        color_mat[p]  = colorOut[i][u][p];

      }
      String str = printColor(color_mat, row, col);
      Serial.println(str);

    }
  }
  //  Serial.println("-----------------C");



}

void loop() {
  if (calibration == false) {
    //    getWhite();






    //    for (byte m = 0; m < 5; m++) {
    //      for (byte n = 0; n < 8; n++) {
    //        Serial.print(a[m][n]);
    //        Serial.print("-");
    //      }
    //      Serial.print("<-->");
    //    }
    //    Serial.println("");
  }
}



void getReadings(byte times, byte func) {
  int reading = 0;
  int total = 0;
  //  byte rgb[] = {0, 0, 0};
  //------------------------------------White balance-------------------------------------//
  if (func == 0) {        // White calibration
    for (int q0 = 0; q0 <= 2; q0++) {
      byte rgb[] = {0, 0, 0};
      if (q0 == 0) {
        rgb[0] = 255;
      }
      else if (q0 == 1) {
        rgb[1] = 255;
      }
      else if (q0 == 2) {
        rgb[2] = 255;
      }
      for (byte l0 = 0; l0 < 12; l0++) {
        led.setPixelColor(l0, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
      }

      for (byte u0 = 0; u0 < 8; u0++) {              // Select which pin of the demultiplexer connects to the analog pin
        //        rdClrY = u;
        for (byte i0 = 0; i0 < 3; i0++) {            // Select the bit values to write to the multiplexer through the digital pins
          digitalWrite(4 - i0, k[u0][i0]);           // Write the selected bit to the demultiplexer
          for (byte p0 = 0; p0 < 5; p0++) {          // Select the .....
            //            rdClrX = p;
            total = 0;
            for (byte k = 1; k <= times; k++) {      // Averaging times
              reading = analogRead(p0);
              total += reading;                      // Make the total by adding each rading values together
              //              whiteBalance[p][u][q] = average;
              delay(readingDelay);2
            }
            //            whiteBalance[p0][u0][q0] = analogRead(p0);
            average = total / times;               // Calculate the average
            whiteBalance[p0][u0][q0] = average;
          }
        }
      }
    }
  }
  //-----------------------------------------Black Balance-----------------------------------------//
  if (func == 1) {        // Black calibration
    for (int q1 = 0; q1 <= 2; q1++) {
      byte rgb[] = {0, 0, 0};
      if (q1 == 0) {
        rgb[0] = 255;
      }
      else if (q1 == 1) {
        rgb[1] = 255;
      }
      else if (q1 == 2) {
        rgb[2] = 255;
      }
      for (byte l1 = 0; l1 < 12; l1++) {
        led.setPixelColor(l1, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
      }

      for (byte u1 = 0; u1 < 8; u1++) {              // Select which pin of the demultiplexer connects to the analog pin
        //        rdClrY = u;
        for (byte i1 = 0; i1 < 3; i1++) {            // Select the bit values to write to the multiplexer through the digital pins
          digitalWrite(4 - i1, k[u1][i1]);           // Write the selected bit to the demultiplexer
          for (byte p1 = 0; p1 < 5; p1++) {          // Select the .....
            //            rdClrX = p;
            total = 0;
            for (byte k = 1; k <= times; k++) {      // Averaging times
              reading = analogRead(p1);
              total += reading;                      // Make the total by adding each rading values together
              //              whiteBalance[p][u][q] = average;
              delay(readingDelay);
            }
            average = total / times;               // Calculate the average
            blackBalance[p1][u1][q1] = average;
          }
        }
      }
    }
  }

  //-----------------------------------get readings----------------------------------------//
  if (func == 2) {        // loop reading
    for (int q2 = 0; q2 <= 2; q2++) {
      byte rgb[] = {0, 0, 0};
      if (q2 == 0) {
        rgb[0] = 255;
      }
      else if (q2 == 1) {
        rgb[1] = 255;
      }
      else if (q2 == 2) {
        rgb[2] = 255;
      }
      for (byte l2 = 0; l2 < 12; l2++) {
        led.setPixelColor(l2, led.Color(rgb[0], rgb[1], rgb[2]));
        led.show();
      }

      for (byte u2 = 0; u2 < 8; u2++) {              // Select which pin of the demultiplexer connects to the analog pin
        //        rdClrY = u;
        for (byte i2 = 0; i2 < 3; i2++) {            // Select the bit values to write to the multiplexer through the digital pins
          digitalWrite(4 - i2, k[u2][i2]);           // Write the selected bit to the demultiplexer
          for (byte p2 = 0; p2 < 5; p2++) {          // Select the .....
            //            rdClrX = p;
            total = 0;
            for (byte k = 1; k <= times; k++) {   // Averaging times
              reading = analogRead(p2);
              total += reading;                   // Make the total by adding each rading values together
              average = total / times;            // Calculate the average
              //              whiteBalance[p][u][q] = average;
              //              blackBalance[p2][u2][q2] = analogRead(p2);

              delay(readingDelay);
            }

            if (average < blackBalance[p2][u2][q2]) {
              average = blackBalance[p2][u2][q2];
            }
            if (average > whiteBalance[p2][u2][q2]) {
              average = whiteBalance[p2][u2][q2];
            }

            float differ1 = whiteBalance[p2][u2][q2] - blackBalance[p2][u2][q2];
            float differ2 = average - blackBalance[p2][u2][q2];
            colorOut[p2][u2][q2] = (differ2 / differ1) * 255;

          }
        }
      }
    }
  }
}


//
//void getWhite() {                         // Get values from a white surface
//  for (int i = 0; i <= 2; i++) {
//    int rgb[] = {0, 0, 0};
//    if (i == 0) {
//      rgb[0] = 255;
//    }
//    else if (i == 1) {
//      rgb[1] = 255;
//    }
//    else if (i == 2) {
//      rgb[2] = 255;
//    }
//    led.setPixelColor(0, led.Color(rgb[0], rgb[1], rgb[2]));
//    led.show();
//    delay(20);
//  }
//}


void printWhite() {
  for (byte i = 0; i < 5; i++) {
    Serial.print("_");
    for ( byte u = 0; u < 8; u++) {
      Serial.println("=");
      for (byte p = 0; p < 3; p++) {
        Serial.print("+");
        Serial.print(whiteBalance[i][u][p]);
      }
    }
  }
  Serial.println("-------------W");
}

void printBlack() {
  for (byte i = 0; i < 5; i++) {
    Serial.print("_");
    for ( byte u = 0; u < 8; u++) {
      Serial.println("=");
      for (byte p = 0; p < 3; p++) {
        Serial.print("+");
        Serial.print(whiteBalance[i][u][p]);
      }
    }
  }
  Serial.println("-----------------B");
}

void printClr() {
  for (byte i = 0; i < 5; i++) {
    Serial.print("_");
    for ( byte u = 0; u < 8; u++) {
      Serial.println("=");
      for (byte p = 0; p < 3; p++) {
        Serial.print("+");
        Serial.print(colorOut[i][u][p]);
      }
    }
  }
  Serial.println("-----------------C");
}

String  printColor(int color_mat[], int rowNum, int clmNum) {                   // Print RGB values
  clrOut = "";
  //  byte rowNum = random(30);
  //  byte clmNum = random(30);
  //..........................
  for (byte i = 0; i <= 2; i++) {                        // Print red and green values
    if (color_mat[i] < 10) {                                // If a value less than 10, add two zeros in front
      clrOut = clrOut + "00";
      clrOut = clrOut + String(color_mat[i]);
    }
    else if (color_mat[i] > 9 && color_mat[i] < 100) {         // If a value grater than 9 and less than 99, add one zero in front
      clrOut = clrOut + "0";
      clrOut = clrOut + String(color_mat[i]);
    }
    else clrOut = clrOut + String(color_mat[i]);                        // Else, print the value
    clrOut = clrOut + ":";
  }
  if (rowNum < 10) {                  // Set row numbers "XX:"
    clrOut = clrOut + "0";
    clrOut = clrOut + String(rowNum);
  }
  else if (rowNum > 9 && rowNum <= 100) {
    clrOut = clrOut + String(rowNum);
  }
  clrOut = clrOut + ":";

  if (clmNum < 10) {                  // Set column number "XX"
    clrOut = clrOut + "0";
    clrOut = clrOut + String(clmNum);
  }
  else if (clmNum > 9 && clmNum <= 41) {
    clrOut = clrOut + String(clmNum);
  }

  return clrOut;
  //
}

