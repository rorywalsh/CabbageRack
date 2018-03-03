
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

    Csound* csound;
	MYFLT** audioOutputChannels;
	MYFLT** audioInputChannels;

    int ksmps, result, compileError; 
	int kIndex = 0;
	int samplePos = 0;
    int const nchnls = 2;       // 2 inputs and 2 outputs in csd, although not used..
	float csScale = 1;
	float phase = 0.0;
	float blinkPhase = 0.0;
	int numControlChannels, numAudioInputChannels, numAudioOutputChannels;
	vector<CabbageControl> cabbageControls;
	vector<CabbageControl> audioOutputControls;
	vector<CabbageControl> audioInputControls;

    CabbageRack();
	
    ~CabbageRack()
	{
		free(audioInputChannels);
		free(audioOutputChannels);
		delete csound;    
	}

	void step() override;
	void createAndCompileCsound();
};