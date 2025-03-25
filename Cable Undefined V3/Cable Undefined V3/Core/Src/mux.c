#include "mux.h"
#include "mux_config.h"
#include "serial.h"
#include "signal_analyzer.h"
#include "fault.h"
#include "leds.h"

#include <string.h>
#include <stdio.h>

// Define CS configurations for all 34 multiplexers
MUX muxes[34] = {
    {GPIOC, LL_GPIO_PIN_0, &xPins1, &yPins1},  // CS_1 (PC0)
    {GPIOC, LL_GPIO_PIN_1, &xPins2, &yPins2},  // CS_2 (PC1)
    {GPIOC, LL_GPIO_PIN_2, &xPins3, &yPins3},  // CS_3 (PC2)
    {GPIOC, LL_GPIO_PIN_3, &xPins4, &yPins4},  // CS_4 (PC3)
    {GPIOC, LL_GPIO_PIN_4, &xPins5, &yPins5},  // CS_5 (PC4)
    {GPIOC, LL_GPIO_PIN_5, &xPins6, &yPins6},  // CS_6 (PC5)
    {GPIOC, LL_GPIO_PIN_6, &xPins7, &yPins7},  // CS_7 (PC6)
    {GPIOC, LL_GPIO_PIN_7, &xPins8, &yPins8},  // CS_8 (PC7)
    {GPIOC, LL_GPIO_PIN_8, &xPins9, &yPins9},  // CS_9 (PC8) PROBLEM???? WITH LL_GPIO_PIN_8
    {GPIOC, LL_GPIO_PIN_9, &xPins10, &yPins10},  // CS_10 (PC9) PROBLEM???? WITH LL_GPIO_PIN_9
    {GPIOC, LL_GPIO_PIN_13, &xPins11, &yPins11}, // CS_11 (PC13) PROBLEM???? WITH LL_GPIO_PIN_11

    {GPIOD, LL_GPIO_PIN_0, &xPins12, &yPins12}, // CS_12 (PD0) PROBLEM???? WITH LL_GPIO_PIN_3
    {GPIOD, LL_GPIO_PIN_1, &xPins13, &yPins13},  // CS_13 (PD1)
    {GPIOD, LL_GPIO_PIN_3, &xPins14, &yPins14},  // CS_14 (PD3)
    {GPIOD, LL_GPIO_PIN_4, &xPins15, &yPins15},  // CS_15 (PD4)
    {GPIOD, LL_GPIO_PIN_7, &xPins16, &yPins16},  // CS_16 (PD7)
    {GPIOD, LL_GPIO_PIN_8, &xPins17, &yPins17},  // CS_17 (PD8)
    {GPIOD, LL_GPIO_PIN_9, &xPins18, &yPins18},  // CS_18 (PD9)
    {GPIOD, LL_GPIO_PIN_10, &xPins19, &yPins19},  // CS_19 (PD10)
    {GPIOD, LL_GPIO_PIN_11, &xPins20, &yPins20}, // CS_20 (PD11)
    {GPIOD, LL_GPIO_PIN_12, &xPins21, &yPins21}, // CS_21 (PD12)
    {GPIOD, LL_GPIO_PIN_13, &xPins22, &yPins22}, // CS_22 (PD13)
    {GPIOD, LL_GPIO_PIN_14, &xPins23, &yPins23}, // CS_23 (PD14)
    {GPIOD, LL_GPIO_PIN_15, &xPins24, &yPins24}, // CS_24 (PD15)

    {GPIOE, LL_GPIO_PIN_0, &xPins25, &yPins25},  // CS_25 (PE0)
    {GPIOE, LL_GPIO_PIN_1, &xPins26, &yPins26},  // CS_26 (PE1)
    {GPIOE, LL_GPIO_PIN_2, &xPins27, &yPins27},  // CS_27 (PE2)
    {GPIOE, LL_GPIO_PIN_3, &xPins28, &yPins28},  // CS_28 (PE3)
    {GPIOE, LL_GPIO_PIN_4, &xPins29, &yPins29},  // CS_29 (PE4)
    {GPIOE, LL_GPIO_PIN_5, &xPins30, &yPins30},  // CS_30 (PE5)
    {GPIOE, LL_GPIO_PIN_6, &xPins31, &yPins31},  // CS_31 (PE6)
    {GPIOE, LL_GPIO_PIN_7, &xPins32, &yPins32},  // CS_32 (PE7)
    {GPIOE, LL_GPIO_PIN_8, &xPins33, &yPins33},  // CS_33 (PE8)
    {GPIOE, LL_GPIO_PIN_9, &xPins34, &yPins34}   // CS_34 (PE9)
};

const char* getPortName(GPIO_TypeDef* port) {
    if (port == GPIOA) return "GPIOA";
    if (port == GPIOB) return "GPIOB";
    if (port == GPIOC) return "GPIOC";
    if (port == GPIOD) return "GPIOD";
    if (port == GPIOE) return "GPIOE";
    return "UNKNOWN";
}

const char* getPinName(uint32_t pin) {
    switch (pin) {
        case LL_GPIO_PIN_0:  return "Pin 0";
        case LL_GPIO_PIN_1:  return "Pin 1";
        case LL_GPIO_PIN_2:  return "Pin 2";
        case LL_GPIO_PIN_3:  return "Pin 3";
        case LL_GPIO_PIN_4:  return "Pin 4";
        case LL_GPIO_PIN_5:  return "Pin 5";
        case LL_GPIO_PIN_6:  return "Pin 6";
        case LL_GPIO_PIN_7:  return "Pin 7";
        case LL_GPIO_PIN_8:  return "Pin 8";
        case LL_GPIO_PIN_9:  return "Pin 9";
        case LL_GPIO_PIN_10: return "Pin 10";
        case LL_GPIO_PIN_11: return "Pin 11";
        case LL_GPIO_PIN_12: return "Pin 12";
        case LL_GPIO_PIN_13: return "Pin 13";
        case LL_GPIO_PIN_14: return "Pin 14";
        case LL_GPIO_PIN_15: return "Pin 15";
        default: return "Unknown Pin";
    }
}

void printMUXDetails(MUX *mux) {
    if (mux == NULL) {
        printf("Invalid MUX pointer.\n");
        return;
    }

    printf("MUX Details:\n");
    printf("---------------------------------------------------\n");
    printf("Control Pin: Port %s, %s\n", getPortName(mux->port), getPinName(mux->pin));
    printf("---------------------------------------------------\n");
    fflush(stdout);
}


MainTrack mainTracks[32] = {0};
SignalAnalyzerTrack signalAnalyzerTracks[8] = {0};
uint8_t ledsToMainTracks[120] = {0};

void setConnection(int x, int y, MUX mux, uint8_t mode) {
    if (x < 0 || x > 15 || y < 0 || y > 7) {
        return;
    }

//    printMUXDetails(&mux);

    // Activate multiplexer using the struct for CS
    LL_GPIO_ResetOutputPin(mux.port, mux.pin);
    LL_mDelay(20);
    LL_GPIO_SetOutputPin(mux.port, mux.pin);
    LL_mDelay(20);

    // Set X address
    (x & 0x01) ? LL_GPIO_SetOutputPin(AX_GPIO, AX0_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX0_PIN);
    (x & 0x02) ? LL_GPIO_SetOutputPin(AX_GPIO, AX1_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX1_PIN);
    (x & 0x04) ? LL_GPIO_SetOutputPin(AX_GPIO, AX2_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX2_PIN);
    (x & 0x08) ? LL_GPIO_SetOutputPin(AX_GPIO, AX3_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX3_PIN);

    // Set Y address
    (y & 0x01) ? LL_GPIO_SetOutputPin(AY_GPIO, AY0_PIN) : LL_GPIO_ResetOutputPin(AY_GPIO, AY0_PIN);
    (y & 0x02) ? LL_GPIO_SetOutputPin(AY_GPIO, AY1_PIN) : LL_GPIO_ResetOutputPin(AY_GPIO, AY1_PIN);
    (y & 0x04) ? LL_GPIO_SetOutputPin(AY_GPIO, AY2_PIN) : LL_GPIO_ResetOutputPin(AY_GPIO, AY2_PIN);


    // Set data pin
    mode ? LL_GPIO_SetOutputPin(DAT_GPIO, DAT_PIN) : LL_GPIO_ResetOutputPin(DAT_GPIO, DAT_PIN);

    // Strobe signal
    LL_mDelay(20);
	LL_GPIO_SetOutputPin(STB_GPIO, STB_PIN);
	LL_mDelay(20);
	LL_GPIO_ResetOutputPin(STB_GPIO, STB_PIN);

	LL_mDelay(20);

	LL_GPIO_ResetOutputPin(DAT_GPIO, DAT_PIN);
    LL_GPIO_ResetOutputPin(mux.port, mux.pin);

	LL_mDelay(20);
}

void routeBreadboard(int breadboardPin1, int breadboardPin2, int net_id, MUX *muxes, size_t muxCount, uint8_t mode, RGB_t rgb) {

    if (breadboardPin1 > 120 || breadboardPin1 < 1 || breadboardPin2 > 120 || breadboardPin2 < 1){
    	return; // Maybe message to UART idk?
    }

    if (mode != 0 && mode != 1){
		return; // Maybe message to UART idk?
	}

    if (rgb.color.r > 255 || rgb.color.r < 0 || rgb.color.g > 255 || rgb.color.g < 0 || rgb.color.b > 255 || rgb.color.b < 0){
		return; // Maybe message to UART idk?
    }

	char pin1Name[6], pin2Name[6];
    snprintf(pin1Name, sizeof(pin1Name), "B_%d", breadboardPin1);
    snprintf(pin2Name, sizeof(pin2Name), "B_%d", breadboardPin2);



//    printf("LedsToMainTrakcs[%d] = %d\n", breadboardPin1 - 1, ledsToMainTracks[breadboardPin1 - 1]);
//    printf("LedsToMainTrakcs[%d] = %d\n", breadboardPin2 - 1, ledsToMainTracks[breadboardPin2 - 1]);

    for (int i = 0; i < 120; i++){
    	if (ledsToMainTracks[i] < 0){
    		ledsToMainTracks[i] = 0;
    	}
    }

    printf("Routing %s to %s with net ID: %d\n", pin1Name, pin2Name, net_id);
    fflush(stdout);

    int xIndex1 = -1, yIndex1 = -1;
    int xIndex2 = -1, yIndex2 = -1;
    MUX *mux1 = NULL;
    MUX *mux2 = NULL;
    MainTrack *selectedTrack = NULL;
    int muxIndex = -1;

    // Find an available or existing main track for the given net ID
    for (int x = 0; x < 32; ++x) {
        if (mainTracks[x].net_id == net_id) {
            selectedTrack = &mainTracks[x];  // Reuse existing track if net ID matches
            break;
        } else if (!mainTracks[x].is_used) {
            selectedTrack = &mainTracks[x];  // Assign new track if available
            selectedTrack->is_used = 1;
            selectedTrack->net_id = net_id;
            selectedTrack->track_id = x; // Assign track ID here
            printf("Selected Track id: %d\n", x);
            break;
        }
    }

    if (!selectedTrack) {
        printf("Error: No available main track for net ID %d\n", net_id);
        return;
    }

    muxIndex = (selectedTrack->track_id < 16) ? 0 : 1;

	for (size_t i = muxIndex; i < muxCount; i += 2) {
		for (int y = 0; y < 8; ++y) {
			if (strcmp((*muxes[i].yPins)[y], pin1Name) == 0 && mux1 == NULL) {
				xIndex1 = selectedTrack->track_id % 16;
				yIndex1 = y;
				mux1 = &muxes[i];
			}
			if (strcmp((*muxes[i].yPins)[y], pin2Name) == 0 && mux2 == NULL) {
				xIndex2 = selectedTrack->track_id % 16;
				yIndex2 = y;
				mux2 = &muxes[i];
			}
		}
		if (mux1 && mux2) break;
	}


    // Update connection counter based on mode
	if (mode == 1) {
		if (mux1->usage[xIndex1][yIndex1] > 0 && mux2->usage[xIndex2][yIndex2] > 0) {
			printf("Already connected, RETURN. NOT incrementing usage, maintrack, led counters\n\n");
			return;
		}

		selectedTrack->current_connections++; // Increment current connections
		printf("Main Track %d incremented. Current connections: %d\n", selectedTrack->track_id, selectedTrack->current_connections);


	    // Keep track of leds:
	    ledsToMainTracks[breadboardPin1 - 1]++;
	    ledsToMainTracks[breadboardPin2 - 1]++;

		addToPin(breadboardPin1, rgb);
		addToPin(breadboardPin2, rgb);
	} else if (mode == 0) {
		selectedTrack->current_connections--; // Decrement current connections
		if (selectedTrack->current_connections <= 0) { // If counter is 0, free track
			printf("Main Track %d is now free\n", selectedTrack->track_id);
			selectedTrack->is_used = 0;
			selectedTrack->net_id = -1;
			selectedTrack->current_connections = 0;
		}else{
			printf("Main Track %d decremented, but still occupied. Current connections: %d\n", selectedTrack->track_id, selectedTrack->current_connections);

		}

	    // Keep track of leds:
		ledsToMainTracks[breadboardPin1 - 1]--;
		ledsToMainTracks[breadboardPin2 - 1]--;

		ledsToMainTracks[breadboardPin1 - 1] == 0 ? removeFromPin(breadboardPin1) : (void)0;
		ledsToMainTracks[breadboardPin2 - 1] == 0 ? removeFromPin(breadboardPin2) : (void)0;
	}


	if (mux1 && mux2) {
		const char *action1 = "SKIP";
		const char *action2 = "SKIP";

		uint8_t *usage1 = &mux1->usage[xIndex1][yIndex1];
		uint8_t *usage2 = &mux2->usage[xIndex2][yIndex2];

		// Only connect if usage was 0 before
		if (mode == 1) {

			action1 = (*usage1 == 0) ? "Connecting" : "Already Connected";
			action2 = (*usage2 == 0) ? "Connecting" : "Already Connected";

			if (*usage1 == 0) setConnection(xIndex1, yIndex1, *mux1, 1);
			if (*usage2 == 0) setConnection(xIndex2, yIndex2, *mux2, 1);

			(*usage1)++;
			(*usage2)++;
		} else {

			action1 = (*usage1 == 1) ? "Disconnecting" : "Still In Use";
			action2 = (*usage2 == 1) ? "Disconnecting" : "Still In Use";

			if (*usage1 == 1) setConnection(xIndex1, yIndex1, *mux1, 0);
			if (*usage2 == 1) setConnection(xIndex2, yIndex2, *mux2, 0);

			if (*usage1 > 0) (*usage1)--;
			if (*usage2 > 0) (*usage2)--;
		}

		printf(
			"%s (X:%d, Y:%d, Usage:%d) at MUX[%ld] CS: %s, Pin: %s, Main Track: %d\n"
			"%s (X:%d, Y:%d, Usage:%d) at MUX[%ld] CS: %s, Pin: %s, Main Track: %d\n\n",
			action1, xIndex1, yIndex1, *usage1, mux1 - muxes + 1, getPortName(mux1->port), getPinName(mux1->pin), selectedTrack->track_id,
			action2, xIndex2, yIndex2, *usage2, mux2 - muxes + 1, getPortName(mux2->port), getPinName(mux2->pin), selectedTrack->track_id
		);

		fflush(stdout);

		sendPixelData();
	} else {
		printf("Error: Unable to route pins - No available MUX found!\n");
		fflush(stdout);
	}
}

void routeSignalAnalyzer(int net_id, MUX *muxes, uint8_t mode) {
    MainTrack *selectedTrack = NULL;
    int trackIndex = -1;
    char trackName[10];  // Buffer for formatted string
    int xIndex = -1, yIndex = -1;
    MUX *mux = NULL;

    // Step 1: Find a main track that is used and has the matching net_id
    for (int i = 0; i < 32; i++) {
        if (mainTracks[i].is_used && mainTracks[i].net_id == net_id) {
            selectedTrack = &mainTracks[i];
            trackIndex = i;
            snprintf(trackName, sizeof(trackName), "MT_%d", trackIndex + 1); // Convert to "MT_X"
			printf("Found Main Track %d (Name: %s) for Net ID %d\n", selectedTrack->track_id, trackName, net_id);
			break;
        }
    }

    if (!selectedTrack) {
        printf("Error: No active Main Track found for Net ID %d\n", net_id);
        return;
    }

    // Step 2: Check MUX 33 and 34
    MUX *mux33 = &muxes[32]; // MUX 33 (Array index 32)
    MUX *mux34 = &muxes[33]; // MUX 34 (Array index 33)

    // Step 3: Look for the track ID in X Pins of MUX 33 and 34

    for (int i = 0; i < 16; i++) {
        if (strcmp((*mux33->xPins)[i], trackName) == 0) {
            xIndex = i;
            mux = mux33;
            break;
        }
        if (strcmp((*mux34->xPins)[i], trackName) == 0) {
            xIndex = i;
            mux = mux34;
            break;
        }
    }

    if (mode == 1) {  // Connecting
		for (int i = 0; i < 8; i++) {
			if (!signalAnalyzerTracks[i].is_used) {
				yIndex = i;
				signalAnalyzerTracks[i].is_used = 1;
				signalAnalyzerTracks[i].net_id = net_id;
				signalAnalyzerTracks[i].track_id = xIndex;
				printf("Assigned Y Pin %d to Net ID %d\n", yIndex, net_id);
				break;
			}
		}
	} else {  // Disconnecting
		for (int i = 0; i < 8; i++) {
			if (signalAnalyzerTracks[i].is_used && signalAnalyzerTracks[i].net_id == net_id) {
				yIndex = i;
				signalAnalyzerTracks[i].is_used = 0;
				signalAnalyzerTracks[i].net_id = -1;
				signalAnalyzerTracks[i].track_id = -1;
				printf("Freed Y Pin %d for Net ID %d\n", yIndex, net_id);
				break;
			}
		}
	}

	if (yIndex == -1) {
		printf("Error: No available Y Pin found for Signal Analyzer\n");
		return;
	}

	printf("Connecting Signal Analyzer to MUX %d at (X: %d, Y: %d) for Net ID %d\n",
		   (mux == mux33 ? 33 : 34), xIndex, yIndex, net_id);

	printf("\n");

	// Step 5: Set the connection
	setConnection(xIndex, yIndex, *mux, mode);

	if (mode == 1) {
	    adcChannelConfig.channel_enabled[yIndex] = 1;  // Enable the channel
	    printf("(ADC%d) ENABLED\n", yIndex);
	} else {
	    adcChannelConfig.channel_enabled[yIndex] = 0;  // Disable the channel
	    printf("(ADC%d) DISABLED\n", yIndex);
	}
}

void clear() {
    // Step 1: Clear all main tracks
    for (int i = 0; i < 32; i++) {
        mainTracks[i].is_used = 0;
        mainTracks[i].net_id = -1;
        mainTracks[i].current_connections = 0;
        mainTracks[i].track_id = -1;
    }

    // Step 2: Clear all signal analyzer tracks
    for (int i = 0; i < 8; i++) {
        signalAnalyzerTracks[i].is_used = 0;
        signalAnalyzerTracks[i].net_id = -1;
        signalAnalyzerTracks[i].track_id = -1;
    }

    // Step 3: Clear all Leds assigned to main tracks
    for (int i = 0; i < 120; i++){
		ledsToMainTracks[i] = 0;
//		printf("ledsToMainTracks[%d] = %d\n", i, ledsToMainTracks[i]);
    }

    // Step 4: Reset all multiplexer usage values
	for (int i = 0; i < 34; i++) {
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 8; y++) {
				muxes[i].usage[x][y] = 0;
			}
		}
	}

    // Step 5: Disable ADC's
	for (int i = 0; i < 8; i++){
	    adcChannelConfig.channel_enabled[i] = 0;
	}

    // Step 6: Perform hardware reset using RST_GPIO
    LL_GPIO_SetOutputPin(RST_GPIO, RST_PIN);
    LL_mDelay(20);
    LL_GPIO_ResetOutputPin(RST_GPIO, RST_PIN);
    LL_mDelay(20);

    printf("CLR complete.\n\n");
}

void processCommand(char *command) {
	if (!fault_3v3_triggered || !fault_5v_triggered){ // test tmr

		if (strncmp(command, "RB", 2) == 0) {  // Check if it's a "ROUTE" command
				int pin1, pin2, net_id, mode, r, g, b;
				int parsed = sscanf(command, "RB %d %d %d %d %d %d %d",
									&pin1, &pin2, &net_id, &mode, &r, &g, &b);
				if (parsed == 7) {  // Ensure all arguments were parsed

					RGB_t color = {b, r, g}; // This is brg, because of the Union, the union is brg, cuz the leds take brg
					printf("Calling routeBreadboard with: Pin1=%d, Pin2=%d, NetID=%d, Mode=%d, RGB(%d,%d,%d)\n",
						   pin1, pin2, net_id, mode, r, g, b);
					fflush(stdout);

					routeBreadboard(pin1, pin2, net_id, muxes, sizeof(muxes) / sizeof(muxes[0]), mode, color);
				} else {
					printf("Error: Invalid RB command format!\n");
					fflush(stdout);
				}
			}
		}
		if (strncmp(command, "RS", 2) == 0) {
			int net_id, mode;
			int parsed = sscanf(command, "RS %d %d",
								&net_id, &mode);
			if (parsed == 2) {  // Ensure all arguments were parsed
				printf("Calling routeSignalAnalyzer with: NetID=%d, Mode=%d\n",
					   net_id, mode);
				fflush(stdout);

				routeSignalAnalyzer(net_id, muxes, mode);
			} else {
				printf("Error: Invalid RS command format!\n");
				fflush(stdout);
		}

	}

    if (strncmp(command, "CLR", 3) == 0) {
		clear();
		clearLeds();
	}

    if (strncmp(command, "EN 5V", 5) == 0) {
    	FAULT_5v_HandleMsg();
	}

    if (strncmp(command, "EN 3V3", 6) == 0) {
    	FAULT_3v3_HandleMsg();
	}

//    if (command[0] == 'S') {
//    	int channel;
//		char state[4];
//
//		// Match format: S<number> <ON|OFF>
//		if (sscanf(command, "S%d %3s", &channel, state) == 2) {
//			if (channel < 1 || channel > 8) {  // S1 to S8, so validate channel
//				printf("Invalid channel: S%d\n", channel);
//				return;
//			}
//
//			// Map S1-S8 to 0-7 (for ADC array)
//			int adc_index = channel - 1;
//
//			if (strcmp(state, "ON") == 0) {
//				adcChannelConfig.channel_enabled[adc_index] = 1;  // Enable the corresponding ADC channel
//				printf("Channel S%d (ADC%d) ENABLED\n", channel, adc_index);
//			} else if (strcmp(state, "OFF") == 0) {
//				adcChannelConfig.channel_enabled[adc_index] = 0;  // Disable the corresponding ADC channel
//				printf("Channel S%d (ADC%d) DISABLED\n", channel, adc_index);
//			} else {
//				printf("Invalid state: %s. Use ON or OFF.\n", state);
//			}
//		} else {
//			printf("Invalid command format. Use: Sx ON/OFF\n");
//		}
//	}
}
