#include "Cabbage.hpp"


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
	int numInputs, numOutputs, numControlChannels;
	vector<CabbageControl> cabbageControls;

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

		cabbageControls = getCabbageControlVector("./plugins/CabbageRack/src/test.csd");
		numControlChannels = getNumberOfControlChannels(cabbageControls);

		cout << "Number of Control Channels is " << numControlChannels << "\n";
		for (auto channel : cabbageControls)
		{
			cout << channel.type << "\n";
			cout << channel.channel << "\n";
			cout << channel.range[Range::min] << "\n";
			cout << channel.range[Range::max] << "\n";
			cout << channel.range[Range::value] << "\n";
		}
    }

	CabbageRack() : 
	Module(numControlChannels+1, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
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

};



void CabbageRack::step() {
		
	if(kIndex == ksmps)
	{
		kIndex = 0;
		compileError = csound->PerformKsmps();
		int controlIndex = 0;
		for( int i = 0 ; i < (int)cabbageControls.size();i++)
		{
			if(cabbageControls[i].hasChannel)
			{
				csound->SetChannel(cabbageControls[i].channel.c_str(), params[controlIndex+1].value);
				controlIndex++;
			}				
		}
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


MyModuleWidget::MyModuleWidget() 
{
	CabbageRack *module = new CabbageRack();
	setModule(module);

	for( auto control : module->cabbageControls)
	{
		if(control.type == "form")
		{
			box.size = Vec(control.width, control.height);
			createFormBackgroundSVG(plugin->path, control.width, control.height, 
									control.rgbaColour[Colour::r], 
									control.rgbaColour[Colour::g], 
									control.rgbaColour[Colour::b], 
									control.rgbaColour[Colour::a]);

			cout << "\n" << createRGBAString(control.rgbaColour[Colour::r], 
									control.rgbaColour[Colour::g], 
									control.rgbaColour[Colour::b], 
									control.rgbaColour[Colour::a]) << "\n";
		}
	}


	//box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/background.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

	for( int i = 0 ; i < module->cabbageControls.size() ; i++)
	{
		if(module->cabbageControls[i].hasChannel)
		{
			if(module->cabbageControls[i].type == "rslider")
			{
				createRSliderSVG(plugin->path, 100, 100, createRGBAString(255, 255, 255, 0), createRGBAString(0, 0, 0, 0), createRGBAString(147, 210, 0, 0));
				addParam(createParam<cabbageRSlider>(Vec(module->cabbageControls[i].bounds[0], module->cabbageControls[i].bounds[1]), module, i, 
																				module->cabbageControls[i].range[Range::min], 
																				module->cabbageControls[i].range[Range::max],
																				module->cabbageControls[i].range[Range::value]));
		
			}
		}

	}
		

	addInput(createInput<PJ301MPort>(Vec(33, 186), module, CabbageRack::PITCH_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(46, 275), module, CabbageRack::OUTPUT1));
	addOutput(createOutput<PJ301MPort>(Vec(16, 275), module, CabbageRack::OUTPUT2));

	addChild(createLight<MediumLight<RedLight>>(Vec(41, 59), module, CabbageRack::BLINK_LIGHT));
}
