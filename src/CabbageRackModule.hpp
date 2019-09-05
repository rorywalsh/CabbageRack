#include "plugin.hpp"

struct CabbageRackModule : Module {

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
	int numControlChannels, numAudioInputChannels, numAudioOutputChannels, numLights;;
	std::vector<CabbageControl> cabbageControls;
	std::vector<CabbageControl> audioOutputControls;
	std::vector<CabbageControl> audioInputControls;

    CabbageRackModule();
	
    ~CabbageRackModule()
	{
		free(audioInputChannels);
		free(audioOutputChannels);
		delete csound;    
	}

	void process(const ProcessArgs &args) override;
	void createAndCompileCsound();
};

struct CabbageRackWidget : ModuleWidget {
	CabbageRackWidget(CabbageRackModule *module);
};
// struct CabbageRack : Module {
// 	enum ParamIds {
// 		PITCH_PARAM,
// 		NUM_PARAMS
// 	};
// 	enum InputIds {
// 		PITCH_INPUT,
// 		NUM_INPUTS
// 	};
// 	enum OutputIds {
// 		SINE_OUTPUT,
// 		NUM_OUTPUTS
// 	};
// 	enum LightIds {
// 		BLINK_LIGHT,
// 		NUM_LIGHTS
// 	};

// 	CabbageRack() {
// 		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
// 		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
// 	}

// 	void process(const ProcessArgs &args) override {
// 	}
// };


