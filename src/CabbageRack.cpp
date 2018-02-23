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

		const string csdFileName(plugin->path+ "/" +plugin->path.substr(plugin->path.find("/", 3)+1)+".csd");
		compileError = csound->Compile(csdFileName.c_str());

		if(compileError != 0)
			cout << "Csound could not compile" << endl;
		else
		{
			spout = csound->GetSpout();                                   
			spin  = csound->GetSpin();                                     
			ksmps = csound->GetKsmps();
			csScale = csound->Get0dBFS();
		}

		cabbageControls = getCabbageControlVector(csdFileName);
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



void CabbageRack::step() 
{
	if(compileError==0)
	{		
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
			spin[kIndex] = inputs[INPUT1].value * 0.1;
			spin[kIndex+1] = inputs[INPUT2].value * 0.1;
			
			outputs[OUTPUT1].value = (spout[kIndex] / csScale);
			outputs[OUTPUT2].value = (spout[kIndex+1] / csScale);
			kIndex+=2;
		}
	}
	//Blink light at 1Hz
	// float deltaTime = 1.0 / engineGetSampleRate();
	// blinkPhase += deltaTime;
	// if (blinkPhase >= 1.0)
	// 	blinkPhase -= 1.0;
	// lights[BLINK_LIGHT].value = (blinkPhase < 0.5) ? 1.0 : 0.0;
}


MyModuleWidget::MyModuleWidget() 
{
	CabbageRack *module = new CabbageRack();
	setModule(module);
	int controlIndex = 0;
	int width, height, centre;
	
	for( auto control : module->cabbageControls)
	{
		if(control.type == "form")
		{
			box.size = Vec(control.width, control.height);
			CabbageForm *form = new CabbageForm(box.size, control.colour);
			width = box.size.x;
			height = box.size.y;
			centre = width/2;
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
				if(CabbageRotarySlider* slider = dynamic_cast<CabbageRotarySlider*>(widget))
				{
					slider->box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
					slider->setColours(control.outlineColour, control.colour, control.trackerColour);
					slider->setText(control.text);
					addParam(widget);
				}
				
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

	//inputs
	for ( int i = 0 ; i < 2 ; i++)
	{
		Port *input = createInput<CabbagePort>(Vec(centre - (i==0 ? 65 : 32), height-55), module, (i==0 ? CabbageRack::INPUT1 : CabbageRack::INPUT2));
		if(CabbagePort* port = dynamic_cast<CabbagePort*>(input))
		{
			port->setText(i==0 ? "In 1" : "In 2");
			addInput(port);
		}
	}

	//outputs
	for ( int i = 0 ; i < 2 ; i++)
	{
		Port *output = createOutput<CabbagePort>(Vec(centre + (i==0 ? 2 : 35), height-55), module, (i==0 ? CabbageRack::OUTPUT1 : CabbageRack::OUTPUT2));
		if(CabbagePort* port = dynamic_cast<CabbagePort*>(output))
		{
			port->setText(i==0 ? "Out 1" : "Out 2");
			addOutput(port);
		}
	}
	
	//addChild(createLight<MediumLight<RedLight>>(Vec(41, 59), module, CabbageRack::BLINK_LIGHT));
}
