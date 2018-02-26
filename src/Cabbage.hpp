
#pragma once 

#include "rack.hpp"
#include "csound.hpp"
#include "CabbageRackWidgets.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>

#define VERSION "1.0"

using namespace rack;
using namespace std;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct MyModuleWidget : ModuleWidget {
	MyModuleWidget();
};

struct CabbageRack : Module {

	enum InputIds {
		INPUT1,
		INPUT2,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT1,
		OUTPUT2,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	// Frame<2> inBuffer[16] = {};
	// int frame = 0;

    Csound* csound;
	vector<MYFLT*> audioOutputChannels;
	vector<MYFLT*> audioInputChannels;
    int ksmps, result, compileError; 
	int kIndex = 0;
	int samplePos = 0;
    int const nchnls = 2;       // 2 inputs and 2 outputs in csd
	float csScale = 1;
	float phase = 0.0;
	float blinkPhase = 0.0;
	int numInputs, numOutputs, numControlChannels, numAudioInputChannels, numAudioOutputChannels;
	vector<CabbageControl> cabbageControls;

    CabbageRack();
    ~CabbageRack(){        delete csound;    }

	void step() override;
		void createAndCompileCsound();

};