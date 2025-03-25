#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "CH446Q.h"
#include "ConfigMUX.h"

extern Adafruit_NeoPixel strip;

MUX mux1(csPin1, xPins1, yPins1);
MUX mux2(csPin2, xPins2, yPins2);
MUX mux3(csPin3, xPins3, yPins3);
MUX mux4(csPin4, xPins4, yPins4);
MUX mux5(csPin5, xPins5, yPins5);
MUX mux6(csPin6, xPins6, yPins6);
MUX mux7(csPin7, xPins7, yPins7);
MUX mux8(csPin8, xPins8, yPins8);
MUX mux9(csPin9, xPins9, yPins9);
MUX mux10(csPin10, xPins10, yPins10);
MUX mux11(csPin11, xPins11, yPins11);
MUX mux12(csPin12, xPins12, yPins12);
MUX mux13(csPin13, xPins13, yPins13);
MUX mux14(csPin14, xPins14, yPins14);
MUX mux15(csPin15, xPins15, yPins15);
MUX mux16(csPin16, xPins16, yPins16);
MUX mux17(csPin17, xPins17, yPins17);
MUX mux18(csPin18, xPins18, yPins18);
MUX mux19(csPin19, xPins19, yPins19);
MUX mux20(csPin20, xPins20, yPins20);
MUX mux21(csPin21, xPins21, yPins21);
MUX mux22(csPin22, xPins22, yPins22);
MUX mux23(csPin23, xPins23, yPins23);
MUX mux24(csPin24, xPins24, yPins24);
MUX mux25(csPin25, xPins25, yPins25);
MUX mux26(csPin26, xPins26, yPins26);
MUX mux27(csPin27, xPins27, yPins27);
MUX mux28(csPin28, xPins28, yPins28);
MUX mux29(csPin29, xPins29, yPins29);
MUX mux30(csPin30, xPins30, yPins30);
MUX mux31(csPin31, xPins31, yPins31);
MUX mux32(csPin32, xPins32, yPins32);

// mux1.printPins();
// mux2.printPins();

std::vector<MUX> muxes = {mux1, mux2, mux3, mux4, mux5, mux6, mux7, mux8, mux9, mux10, mux11, mux12, mux13, mux14, mux15, mux16, mux17, mux18, mux19, mux20, mux21, mux22, mux23, mux24, mux25, mux26, mux27, mux28, mux29, mux30, mux31, mux32};

void setup()
{
    pinMode(RST_PIN, OUTPUT);
    pinMode(DAT_PIN, OUTPUT);
    pinMode(STB_PIN, OUTPUT);
    pinMode(AX0_PIN, OUTPUT);
    pinMode(AX1_PIN, OUTPUT);
    pinMode(AX2_PIN, OUTPUT);
    pinMode(AX3_PIN, OUTPUT);
    pinMode(AY0_PIN, OUTPUT);
    pinMode(AY1_PIN, OUTPUT);
    pinMode(AY2_PIN, OUTPUT);

    pinMode(EN1_PIN, OUTPUT);
    pinMode(EN2_PIN, OUTPUT);
    pinMode(EN3_PIN, OUTPUT);

    strip.begin();
    delay(100);
    strip.show();
    delay(100);

    Serial.begin(115200);
    delay(100);

    setupPMU(true);

    resetMuxes();

    // route(muxes, 1, 1, true, strip.Color(135, 45, 78));
    // delay(10000);
    // route(muxes, 7, 3, false);

    // mux1.setConnection(0, 0, true, 0, 1, strip.Color(255, 255, 0));
}

void loop()
{
    if (Serial.available() > 0)
    {
        String input = Serial.readStringUntil('\n');
        int8_t muxIndex1, muxIndex2, modeInt;
        int8_t r, g, b;
        char CODE[3];

        // Parse the incoming data from Python (e.g., "1 2 1 FF5733")
        sscanf(input.c_str(), "%d %d %d %d %d %d %s", &muxIndex1, &muxIndex2, &modeInt, &r, &g, &b, CODE);

        // Serial.print("muxIndex1: "); Serial.print(muxIndex1); Serial.print(", ");
        // Serial.print("muxIndex2: "); Serial.print(muxIndex2); Serial.print(", ");
        // Serial.print("mode: "); Serial.print(modeInt); Serial.print(", ");
        // Serial.print("r: "); Serial.print(r); Serial.print(", ");
        // Serial.print("g: "); Serial.print(g); Serial.print(", ");
        // Serial.print("b: "); Serial.print(b); Serial.print(", ");

        // Convert modeInt to a boolean for mode
        if (strcmp(CODE, "CLR") == 0)
        {
            resetMuxes();
            for (size_t i = 0; i < muxes.size(); ++i)
            {
                muxes[i].clearConnections();
            }

            clearAllMainTracks();
            resetLEDs();
            Serial.println(200);
        }
        else
        {
            bool mode = (modeInt != 0);

            // Call the route function with parsed parameters
            route(muxes, muxIndex1, muxIndex2 - 60, mode, strip.Color(r, g, b));

            Serial.println(200);
        }
    }
}
