#include "Cabbage.hpp"
#include "CabbageRackGUI.hpp"

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
	int controlIndex = 0;
	for( auto control : module->cabbageControls)
	{
		if(control.type == "form")
		{
			box.size = Vec(control.width, control.height);
			CabbageForm *form = new CabbageForm(box.size, control.colour[Colour::r], control.colour[Colour::g], control.colour[Colour::b], control.colour[Colour::a]);
			addChild(form);
		}

		else if(control.hasChannel)
		{
			if(control.type == "rslider")
			{
				ParamWidget* widget = createParam<CabbageRotarySlider>(Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]), module, controlIndex, 
														control.range[Range::min], 
														control.range[Range::max],
														control.range[Range::value]);
				dynamic_cast<CabbageRotarySlider*>(widget)->box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
				dynamic_cast<CabbageRotarySlider*>(widget)->setColours(nvgRGBA(control.outlineColour[Colour::r],
																			control.outlineColour[Colour::g],
																			control.outlineColour[Colour::b],
																			control.outlineColour[Colour::a]),
																  	nvgRGBA(control.colour[Colour::r],
																			control.colour[Colour::g],
																			control.colour[Colour::b],
																			control.colour[Colour::a]),
																 	nvgRGBA(control.trackerColour[Colour::r],
																			control.trackerColour[Colour::g],
																			control.trackerColour[Colour::b],
																			control.trackerColour[Colour::a]));
				addParam(widget);
			}
		}	

		controlIndex++;	
	}
	

	// CabbageLabel *label = new CabbageLabel(Vec(20, 20), Vec(100, 20), " Test");
	// label->setLabelColour(255, 0, 255, 255);
	// label->setBackgroundColour(0, 0, 0, 255);
	// addChild(label);

	//CabbageRSlider *slider = new CabbageRSlider(Vec(20, 200), Vec(50, 50), " Test");
	//slider->setLabelColour(255, 0, 255, 255);
	//slider->setBackgroundColour(0, 0, 0, 255);

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));


	addInput(createInput<PJ301MPort>(Vec(33, 186), module, CabbageRack::PITCH_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(46, 275), module, CabbageRack::OUTPUT1));
	addOutput(createOutput<PJ301MPort>(Vec(16, 275), module, CabbageRack::OUTPUT2));

	//addChild(createLight<MediumLight<RedLight>>(Vec(41, 59), module, CabbageRack::BLINK_LIGHT));
}
