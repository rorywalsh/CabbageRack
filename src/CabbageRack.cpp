#include "Cabbage.hpp"
#include "CabbageRackWidgets.hpp"

using namespace std;

void MessageCallback(CSOUND* cs, int attr, const char *format, va_list valist)
{
  vprintf(format, valist);   
  return;
}

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

		cabbageControls = CabbageParser::getCabbageControlVector(csdFileName);
		numControlChannels = CabbageParser::getNumberOfControlChannels(cabbageControls);
		
    }

	CabbageRack() : 
	Module(1, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
	numInputs(NUM_INPUTS),
	numOutputs(NUM_OUTPUTS)
	{
		createAndCompileCsound();                
		params.resize(numControlChannels);                   
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
					csound->SetChannel(cabbageControls[i].channel.c_str(), params[controlIndex].value);
					controlIndex++;
				}				
			}
		}
		
		if (compileError == 0)
		{
			const int pos = kIndex*numOutputs;
			spin[pos] = inputs[INPUT1].value * 0.1;
			spin[pos+1] = inputs[INPUT2].value * 0.1;			
			outputs[OUTPUT1].value = (spout[pos] / csScale)*10;
			outputs[OUTPUT2].value = (spout[pos+1] / csScale)*10;
			kIndex++;
		}
	}
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
			addChild(new CabbageForm(control));
			box.size = Vec(control.width, control.height);
			width = box.size.x;
			height = box.size.y;
			centre = width/2;			
		}

		else if(control.type == "label")
			addChild(new CabbageLabel(control));

		else if(control.type == "groupbox")
			addChild(new CabbageGroupbox(control));

		else if(control.type == "image")
			addChild(new CabbageImage(control));

		else if(control.hasChannel)
		{
			if(control.type == "rslider")
				addParam(new CabbageRotarySlider(control, module, controlIndex));			

			else if(control.type == "button")
				addParam(new CabbageButton(control, module, controlIndex));	
			
			else if(control.type == "checkbox")
				addParam(new CabbageCheckbox(control, module, controlIndex));	

			controlIndex++;					
		}	

			
		
	}

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
