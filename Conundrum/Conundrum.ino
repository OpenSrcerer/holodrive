#include <XBOXRECV.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

// -------Pin #defs--------
// ENG 1
#define en1A 44
#define in1a 24
#define in2a 25
int quantization1 = 0;
double E1V;

// ENG2
#define en1B 45
#define in3a 26
#define in4a 27
int quantization2 = 0;
double E2V;

// ENG3
#define en1C 46
#define in1b 28
#define in2b 29
int quantization3 = 0;
double E3V;

// ENG4
#define en1D 47
#define in3b 30
#define in4b 31
int quantization4 = 0;
double E4V;

// S - Pin
#define transPin 53
#define transDin2 52
#define transDin3 51

bool tP = false;

// ------- Additives -------
double LeftStickXAxis, LeftStickYAxis, RightStickXAxis;
// -------------------------

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // optional, can remove
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nFailure to connect to USB Shield."));
    while (1);
  }
  Serial.print(F("\r\nInitialization complete.\nSetting up pins."));

 // --------- Pin Setup ----------
  pinMode (en1A, OUTPUT);
  pinMode (in1a, OUTPUT);
  pinMode (in2a, OUTPUT);

  pinMode (en1B, OUTPUT);
  pinMode (in3a, OUTPUT);
  pinMode (in4a, OUTPUT);

  pinMode (en1C, OUTPUT);
  pinMode (in1b, OUTPUT);
  pinMode (in2b, OUTPUT);

  pinMode (en1D, OUTPUT);
  pinMode (in3b, OUTPUT);
  pinMode (in4b, OUTPUT);

  pinMode (transPin, OUTPUT);
  pinMode (transDin2, OUTPUT);
  pinMode (transDin3, OUTPUT);

  Serial.print("Pin setup complete.");
}
 // ------------------------------

void loop() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 1; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getAnalogHat(LeftHatX, i) > 8000 || Xbox.getAnalogHat(LeftHatX, i) < -8000) 
        {
          if (Xbox.getAnalogHat(LeftHatX, i) > 8000)
          {
            LeftStickXAxis = Xbox.getAnalogHat(LeftHatX, i);
          }
          if (Xbox.getAnalogHat(LeftHatX, i) < -8000)
          {
            LeftStickXAxis = Xbox.getAnalogHat(LeftHatX, i);
          }
        }
        if (Xbox.getAnalogHat(LeftHatY, i) > 8000 || Xbox.getAnalogHat(LeftHatY, i) < -8000) 
        {
          if (Xbox.getAnalogHat(LeftHatY, i) > 8000)
          {
            LeftStickYAxis = Xbox.getAnalogHat(LeftHatY, i);
          }
          if (Xbox.getAnalogHat(LeftHatY, i) < -8000)
          {
            LeftStickYAxis = Xbox.getAnalogHat(LeftHatY, i);
          }
        }
        if (Xbox.getAnalogHat(RightHatX, i) > 8000 || Xbox.getAnalogHat(RightHatX, i) < -8000) 
        {
          if (Xbox.getAnalogHat(RightHatX, i) > 8000)
          {
            RightStickXAxis = Xbox.getAnalogHat(RightHatX, i);
          }
          if (Xbox.getAnalogHat(RightHatX, i) < -8000)
          {
            RightStickXAxis = Xbox.getAnalogHat(RightHatX, i);
          }
        }

        if (Xbox.getButtonClick(LEFT, i)) 
        {
          digitalWrite(transDin2, LOW);
        }

        if (Xbox.getButtonClick(RIGHT, i)) 
        {
          digitalWrite(transDin2, HIGH);
          digitalWrite(transDin3, HIGH);
        }
        
        if (Xbox.getButtonClick(UP, i)) 
        {
          digitalWrite(transDin3, LOW);
        }

        if (Xbox.getButtonClick(DOWN, i))
        {
          digitalWrite(transDin3, HIGH);
        }
        
        if (Xbox.getButtonClick(A, i))
        {
          tP = !tP;
        }
        
        if (tP == false)
        {
          digitalWrite(transPin, LOW);
        }
        if (tP == true)
        {
          digitalWrite(transPin, HIGH);
        }
        
        E1V = LeftStickYAxis - LeftStickXAxis + RightStickXAxis; // FR
        E2V = -LeftStickYAxis - LeftStickXAxis - RightStickXAxis; // RL
        E3V = LeftStickYAxis - LeftStickXAxis - RightStickXAxis; // FL
        E4V = -LeftStickYAxis - LeftStickXAxis + RightStickXAxis; // RR
        
        // ENG Polarity 1 --------------------------------------
        if (E1V > 0)
        {
          digitalWrite(in1a, HIGH);
          digitalWrite(in2a, LOW);
          quantization1 = map(E1V, 0, 32767, 0, 255);
          if (quantization1 < 35)
          {
           quantization1 = 0;
          }
        }
        if (E1V < 0)
        {
          digitalWrite(in1a, LOW);
          digitalWrite(in2a, HIGH);
          quantization1 = map(E1V, 0, -32768, 0, 255);
          if (quantization1 < 35)
          {
           quantization1 = 0;
          }
        }
        // ENG Polarity 2 --------------------------------------
        if (E2V > 0)
        {
          digitalWrite(in3a, LOW);
          digitalWrite(in4a, HIGH);
          quantization2 = map(E2V, 0, 32767, 0, 255);
          if (quantization2 < 35)
          {
           quantization2 = 0;
          }
        }
        if (E2V < 0)
        {
          digitalWrite(in3a, HIGH);
          digitalWrite(in4a, LOW);
          quantization2 = map(E2V, 0, -32768, 0, 255);
          if (quantization2 < 35)
          {
           quantization2 = 0;
          }
        }
        // ENG Polarity 3 --------------------------------------
        if (E3V > 0)
        {
          digitalWrite(in1b, HIGH);
          digitalWrite(in2b, LOW);
          quantization3 = map(E3V, 0, 32767, 0, 255);
          if (quantization3 < 35)
          {
           quantization3 = 0;
          }
        }
        if (E3V < 0)
        {
          digitalWrite(in1b, LOW);
          digitalWrite(in2b, HIGH);
          quantization3 = map(E3V, 0, -32768, 0, 255);
          if (quantization3 < 35)
          {
           quantization3 = 0;
          }
        }
        
        // ENG Polarity 4 --------------------------------------
        if (E4V > 0)
        {
          digitalWrite(in3b, HIGH);
          digitalWrite(in4b, LOW);
          quantization4 = map(E4V, 0, 32767, 0, 255);
          if (quantization4 < 35)
          {
           quantization4 = 0;
          }
        }
        if (E4V < 0)
        {
          digitalWrite(in3b, LOW);
          digitalWrite(in4b, HIGH);
          quantization4 = map(E4V, 0, -32768, 0, 255);
          if (quantization4 < 35)
          {
           quantization4 = 0;
          }
        }
		
        // Keeping calculated axis value from exceeding 255
        quantization1 = constrain(quantization1, 0, 255);
        quantization2 = constrain(quantization2, 0, 255);
        quantization3 = constrain(quantization3, 0, 255);
        quantization4 = constrain(quantization4, 0, 255);

        // finalizing engine signal
        analogWrite(en1A, quantization1);
        analogWrite(en1B, quantization2);
        analogWrite(en1C, quantization3);
        analogWrite(en1D, quantization4);

        Serial.print("\n");
        Serial.print("A: ");
        Serial.print(quantization1);
        Serial.print("\n");
        Serial.print("B: ");
        Serial.print(quantization2);
        Serial.print("\n");
        Serial.print("C: ");
        Serial.print(quantization3);
        Serial.print("\n");
        Serial.print("D: ");
        Serial.print(quantization4);

        // Axis reset
        LeftStickYAxis = 0;
        LeftStickXAxis = 0;
        RightStickXAxis = 0;
        quantization1 = 0;
        quantization2 = 0;
        quantization3 = 0;
        quantization4 = 0;
      }
    }
  }
}
