#include "Cabbage.hpp"
#include "csound.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void MessageCallback(CSOUND* cs, int attr, const char *format, va_list valist)
{
  vprintf(format, valist);   
  return;
}

struct CabbageRack : Module {
	enum ParamIds {
		PITCH_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT,
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

    Csound* csound;
    MYFLT *spin, *spout;
    int ksmps, result, compileError; 
	int kIndex = 0;
	int samplePos = 0;
    int const nchnls = 2;       // 2 inputs and 2 outputs in csd
	float csScale = 1;
	float phase = 0.0;
	float blinkPhase = 0.0;
	int numInputs, numOutputs;


    void createAndCompileCsound() 
	{
		csound = new Csound(); 
		csound->SetOption((char*)"-n");
		csound->SetOption((char*)"-d");

		CSOUND_PARAMS* csoundParams = nullptr;
		csoundParams = new CSOUND_PARAMS();
		csoundParams->sample_rate_override = engineGetSampleRate();
		csoundParams->displays = 0;		
		csound->SetParams(csoundParams);
		csound->SetHostImplementedAudioIO(1, 0);
		csound->SetHostData(this);

		compileError = csound->Compile("./plugins/CabbageRack/src/test.csd");
		if(compileError != 0)
			cout << "Csound could not compile" << endl;
		else
		{
			spout = csound->GetSpout();                                   
			spin  = csound->GetSpin();                                     
			ksmps = csound->GetKsmps();
			csScale = csound->Get0dBFS();
		}

    }

	CabbageRack() : 
	Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
	numInputs(NUM_INPUTS),
	numOutputs(NUM_OUTPUTS)
	{
		createAndCompileCsound();                                     
        csound->SetMessageCallback(MessageCallback);
	}


    ~CabbageRack()
    {
        delete csound;            
    }

	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void CabbageRack::step() {
		
	if(kIndex == ksmps)
	{
		kIndex = 0;
		compileError = csound->PerformKsmps();
	}

	
	if (compileError == 0)
	{
		samplePos = kIndex*numOutputs;

		for (int channel = 0; channel < numOutputs; ++channel)
		{			
			outputs[channel].value = (spout[samplePos] / csScale);
			++kIndex;
		}
	}

	//Blink light at 1Hz
	float deltaTime = 1.0 / engineGetSampleRate();
	blinkPhase += deltaTime;
	if (blinkPhase >= 1.0)
		blinkPhase -= 1.0;
	lights[BLINK_LIGHT].value = (blinkPhase < 0.5) ? 1.0 : 0.0;
}


MyModuleWidget::MyModuleWidget() {
	CabbageRack *module = new CabbageRack();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/MyModule.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

	addParam(createParam<Davies1900hBlackKnob>(Vec(28, 87), module, CabbageRack::PITCH_PARAM, -3.0, 3.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(33, 186), module, CabbageRack::PITCH_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(46, 275), module, CabbageRack::OUTPUT1));
	addOutput(createOutput<PJ301MPort>(Vec(16, 275), module, CabbageRack::OUTPUT2));

	addChild(createLight<MediumLight<RedLight>>(Vec(41, 59), module, CabbageRack::BLINK_LIGHT));
}
