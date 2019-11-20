# 1 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino"
# 2 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino" 2



# 6 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino" 2

// -------------------
// DEFINE PINS HERE
// -------------------

// ------ FR ---------




// ------ FL ---------




// ------ RR ---------




// ------ RL ---------




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
        PWM[0] = StickAxes[0] - StickAxes[1] - StickAxes[2]; // Front Right
        PWM[1] = StickAxes[0] + StickAxes[1] + StickAxes[2]; // Front Left
        PWM[2] = -StickAxes[0] - StickAxes[1] - StickAxes[2]; // Rear Right
        PWM[3] = -StickAxes[0] + StickAxes[1] + StickAxes[2]; // Rear Left
    }

    double mapPWM(double &bonks, int &sensitivity, int polarization)
    {
        bonks = ((map(bonks, 0, polarization, 0, 255))<(0)?(0):((map(bonks, 0, polarization, 0, 255))>(getSpeedMode())?(getSpeedMode()):(map(bonks, 0, polarization, 0, 255)))); // set and constrain
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
                        digitalWrite(22, 0x0);
                        digitalWrite(23, 0x1);
                        break;
                    case 1:
                        digitalWrite(24, 0x0);
                        digitalWrite(25, 0x1);
                        break;
                    case 2:
                        digitalWrite(26, 0x0);
                        digitalWrite(27, 0x1);
                        break;
                    case 3:
                        digitalWrite(28, 0x0);
                        digitalWrite(29, 0x1);
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
                        digitalWrite(22, 0x1);
                        digitalWrite(23, 0x0);
                        break;
                    case 1:
                        digitalWrite(24, 0x1);
                        digitalWrite(25, 0x0);
                        break;
                    case 2:
                        digitalWrite(26, 0x1);
                        digitalWrite(27, 0x0);
                        break;
                    case 3:
                        digitalWrite(28, 0x1);
                        digitalWrite(29, 0x0);
                        break;
                }
                PWM[it] = mapPWM(PWM[it], sensitivity, -32767);
            }
        }
        analogWrite(44, PWM[0]);
        analogWrite(45, PWM[1]);
        analogWrite(46, PWM[2]);
        analogWrite(47, PWM[3]);

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
# 220 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino"
} Ori;

void setup()
{

    // -------------------
    // USB INIT
    // -------------------
    Serial.begin(115200);
    if (Sein.Init() == -1)
    {
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 231 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 231 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino"
                    "\r\nFailure to connect to USB Shield."
# 231 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino" 3
                    ); &__c[0];}))
# 231 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino"
                    )));
        while (1);
    }
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 234 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 234 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino"
                "\r\nInitialization complete."
# 234 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino" 3
                ); &__c[0];}))
# 234 "c:\\Users\\WeedPot\\Desktop\\Robotics 2019\\C_Workspace\\Perception\\Perception.ino"
                )));

    // -------------------
    // MOTOR PIN MODES
    // -------------------
    pinMode (45, 0x1);
    pinMode (24, 0x1);
    pinMode (25, 0x1);

    pinMode (44, 0x1);
    pinMode (22, 0x1);
    pinMode (23, 0x1);

    pinMode (47, 0x1);
    pinMode (28, 0x1);
    pinMode (29, 0x1);

    pinMode (46, 0x1);
    pinMode (26, 0x1);
    pinMode (27, 0x1);

    // -------------------
    // EXTRAS PIN MODES
    // -------------------
    pinMode(13, 0x1); // cuz why tf not
}

void loop()
{
    Ori.updateAxis();
    Ori.calcResultant();
    Ori.setSpeedMode();
    Ori.setMotorValues(true, 50); // verbose, sensitivity, constraint
    Ori.setZero();
}
