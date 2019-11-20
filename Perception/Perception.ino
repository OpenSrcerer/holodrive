#include <XBOXRECV.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// -------------------
// DEFINE PINS HERE
// -------------------

// ------ FR ---------
#define PWMFRPin 44
#define FRPolA 22
#define FRPolB 23

// ------ FL ---------
#define PWMFLPin 45
#define FLPolA 24
#define FLPolB 25

// ------ RR ---------
#define PWMRRPin 46
#define RRPolA 26
#define RRPolB 27

// ------ RL ---------
#define PWMRLPin 47
#define RLPolA 28
#define RLPolB 29

// -------------------
// BOT CONTROLLER CLASS
// -------------------

USB Sein; // USB Object Definition
XBOXRECV Xbox(&Sein); 

class Controller
{
    public:
    void updateAxis()
    {
        // Updates Axes
        Sein.Task();
        if (Xbox.XboxReceiverConnected && Xbox.Xbox360Connected[0]) 
        {
           if (Xbox.getAnalogHat(LeftHatY, 0) > 8000 || 
            Xbox.getAnalogHat(LeftHatY, 0) < -8000) 
            {
                StickAxes[0] = Xbox.getAnalogHat(LeftHatY, 0);
            }
            if (Xbox.getAnalogHat(LeftHatX, 0) > 8000 || 
            Xbox.getAnalogHat(LeftHatX, 0) < -8000) 
            {
                StickAxes[1] = Xbox.getAnalogHat(LeftHatX, 0);
            }
            if (Xbox.getAnalogHat(RightHatX, 0) > 8000 || 
            Xbox.getAnalogHat(RightHatX, 0) < -8000) 
            {
                StickAxes[2] = Xbox.getAnalogHat(RightHatX, 0);
            }
        }
    }

    void calcResultant()
    {
        // calculates PWM values using
        // the rules of holodrive
        PWM[0] = StickAxes[0] - StickAxes[1] - StickAxes[2];    // Front Right
        PWM[1] = StickAxes[0] + StickAxes[1] + StickAxes[2];   // Front Left
        PWM[2] = -StickAxes[0] - StickAxes[1] - StickAxes[2];   // Rear Right
        PWM[3] = -StickAxes[0] + StickAxes[1] + StickAxes[2];    // Rear Left
    }

    double mapPWM(double &bonks, int &sensitivity, int polarization)
    {
        bonks = constrain(map(bonks, 0, polarization, 0, 255), 0, getSpeedMode()); // set and constrain
        if (bonks < sensitivity)
            bonks = 0;
        return bonks;
    }

    void setSpeedMode()
    {
        if (Xbox.getButtonClick(A, 0))
            speedMode = !speedMode;
    }

    int getSpeedMode()
    {
        if (speedMode)
        {
            return 255;
        }
        else
        {
            return 100;
        }
    }

    void setMotorValues(bool verbose, int sensitivity)
    {
        // iterate through the PWM Array
        // in order to take raw calculated
        // stick values, find polarities,
        // and PWM VALUES

        for (int it = 0; it <= 3; ++it)
        {
            if (PWM[it] > 0)
            {
                if (verbose)
                {
                    Serial.print("\n");
                    Serial.print(it);
                    Serial.print(": ");
                    Serial.print("Positive");
                }
                switch (it) // switch for every case (fuck strings)
                {
                    // polarities being set
                    case 0:
                        digitalWrite(FRPolA, LOW);
                        digitalWrite(FRPolB, HIGH);
                        break;
                    case 1:
                        digitalWrite(FLPolA, LOW);
                        digitalWrite(FLPolB, HIGH);
                        break;
                    case 2:
                        digitalWrite(RRPolA, LOW);
                        digitalWrite(RRPolB, HIGH);
                        break;
                    case 3:
                        digitalWrite(RLPolA, LOW);
                        digitalWrite(RLPolB, HIGH);
                        break;
                }
                PWM[it] = mapPWM(PWM[it], sensitivity, 32767);
            } 
            else 
            {
                if (verbose)
                {
                    Serial.print("\n");
                    Serial.print(it);
                    Serial.print(": ");
                    Serial.print("Negative");
                }
                switch (it) // same thing
                {
                    // same
                    case 0:
                        digitalWrite(FRPolA, HIGH);
                        digitalWrite(FRPolB, LOW);
                        break;
                    case 1:
                        digitalWrite(FLPolA, HIGH);
                        digitalWrite(FLPolB, LOW);
                        break;
                    case 2:
                        digitalWrite(RRPolA, HIGH);
                        digitalWrite(RRPolB, LOW);
                        break;
                    case 3:
                        digitalWrite(RLPolA, HIGH);
                        digitalWrite(RLPolB, LOW);
                        break;
                }
                PWM[it] = mapPWM(PWM[it], sensitivity, -32767);
            }
        }
        analogWrite(PWMFRPin, PWM[0]);
        analogWrite(PWMFLPin, PWM[1]);
        analogWrite(PWMRRPin, PWM[2]);
        analogWrite(PWMRLPin, PWM[3]);

        if (verbose)
        {
            Serial.print("\n\n");
            Serial.print("FR: ");
            Serial.print(PWM[0]);
            Serial.print("\nFL: ");
            Serial.print(PWM[1]);
            Serial.print("\nRR: ");
            Serial.print(PWM[2]);
            Serial.print("\nRL: ");
            Serial.print(PWM[3]);
        }
    }

    void setZero() // self explanatory
    {
        for (int it = 0; it < 3; ++it)
        {
            StickAxes[it] = 0;
        }
        for (int it = 0; it < 4; ++it)
        {
            PWM[it] = 0;
        }
    }

    private:
    bool speedMode = true;
    double PWM[4], StickAxes[3]; // no idea why these are doubles? copying from old

    /*
    Analog Values Structure:
    LeftStickYAxis, LeftStickXAxis, RightStickXAxis
     ^                  ^               ^
     StickAxes[0]       StickAxes[1]        StickAxes[2]

    ----------------------------------------------------

    PWM Array Structure:
    PWMFR,   PWMFL,    PWMRR,     PWMRL
    ^ PWM[0] ^ PWM[1]  ^ PWM[2]   ^ PWM[3] 
    */
} Ori;

void setup()
{

    // -------------------
    // USB INIT
    // -------------------
    Serial.begin(115200);
    if (Sein.Init() == -1) 
    {
        Serial.print(F("\r\nFailure to connect to USB Shield."));
        while (1);
    }
    Serial.print(F("\r\nInitialization complete."));

    // -------------------
    // MOTOR PIN MODES
    // -------------------
    pinMode (PWMFLPin, OUTPUT);
    pinMode (FLPolA, OUTPUT);
    pinMode (FLPolB, OUTPUT);

    pinMode (PWMFRPin, OUTPUT);
    pinMode (FRPolA, OUTPUT);
    pinMode (FRPolB, OUTPUT);

    pinMode (PWMRLPin, OUTPUT);
    pinMode (RLPolA, OUTPUT);
    pinMode (RLPolB, OUTPUT);

    pinMode (PWMRRPin, OUTPUT);
    pinMode (RRPolA, OUTPUT);
    pinMode (RRPolB, OUTPUT);

    // -------------------
    // EXTRAS PIN MODES
    // -------------------
    pinMode(LED_BUILTIN, OUTPUT); // cuz why tf not
}

void loop()
{
    Ori.updateAxis();
    Ori.calcResultant(); 
    Ori.setSpeedMode(); // updates the speed mode from LO to HI
    Ori.setMotorValues(true, 50); // verbose, sensitivity, constraint
    Ori.setZero(); // set all motor values back to zero
}