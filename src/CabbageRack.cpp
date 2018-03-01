#include "Cabbage.hpp"


using namespace std;

void MessageCallback(CSOUND* cs, int attr, const char *format, va_list valist)
{
  vprintf(format, valist);   
  return;
}

//============================================================
// MyModuleWidget implementation
//============================================================
MyModuleWidget::MyModuleWidget() 
{
	CabbageRack *module = new CabbageRack();
	setModule(module);
	int kRateControlIndex = 0;
	int aRateInputIndex = 0;
	int aRateOutputIndex = 0;
	int width, height, centre;
	
	for( auto& control : module->cabbageControls)
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
				addParam(new CabbageRotarySlider(control, module, kRateControlIndex++));			

			else if(control.type == "button")
				addParam(new CabbageButton(control, module, kRateControlIndex++));	
			
			else if(control.type == "checkbox")
				addParam(new CabbageCheckbox(control, module, kRateControlIndex++));	

			else if(control.type == "combobox")
				addParam(new CabbageCombobox(control, module, kRateControlIndex++));

			else if(control.type == "cvinput")
				addInput(new CabbagePort(control, module, aRateInputIndex++));	

			else if(control.type == "cvoutput")
				addOutput(new CabbagePort(control, module, aRateOutputIndex++));				
		}	
	
	}

	addChild(createScrew<ScrewSilver>(Vec(0, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 16, 0)));
	addChild(createScrew<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 16, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	
	//addChild(createLight<MediumLight<RedLight>>(Vec(41, 59), module, CabbageRack::BLINK_LIGHT));
}

//============================================================
// CabbageRack implementation
//============================================================
CabbageRack::CabbageRack() : 
	Module(1, 2, 2, 2)
{
	createAndCompileCsound();                
	params.resize(numControlChannels);  
	inputs.resize(audioInputControls.size());
	outputs.resize(audioOutputControls.size());              
	csound->SetMessageCallback(MessageCallback);
}

void CabbageRack::createAndCompileCsound() 
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

	cabbageControls = CabbageParser::getCabbageControlVector(csdFileName);
	numControlChannels = CabbageParser::getNumberOfControlChannels(cabbageControls);
	audioOutputControls = CabbageParser::getAudioChannels(cabbageControls, "cvoutput");
	audioInputControls = CabbageParser::getAudioChannels(cabbageControls, "cvinput");

	if(compileError != 0)
		cout << "Csound could not compile" << endl;
	else
	{    
		ksmps = csound->GetKsmps();
		audioOutputChannels = new MYFLT*[audioOutputControls.size()];
		audioInputChannels = new MYFLT*[audioInputControls.size()];

		for( int i = 0 ; i < audioOutputControls.size() ; i++)
		{
			audioOutputChannels[i] = new MYFLT(0.0);
			csoundGetChannelPtr(csound->GetCsound(), &audioOutputChannels[i], audioOutputControls[i].channel.c_str(),
                            CSOUND_AUDIO_CHANNEL | CSOUND_OUTPUT_CHANNEL);
		}
        	
		for( int i = 0 ; i < audioInputControls.size() ; i++)
		{
			audioInputChannels[i] = new MYFLT(0.0);
			csoundGetChannelPtr(csound->GetCsound(), &audioInputChannels[i], audioInputControls[i].channel.c_str(),
                           CSOUND_AUDIO_CHANNEL | CSOUND_OUTPUT_CHANNEL);
		}      	
			
		csScale = csound->Get0dBFS();
	}	
}

void CabbageRack::step() 
{
	if(compileError==0)
	{		
		if(kIndex == ksmps)
		{
			kIndex = 0;
			compileError = csound->PerformKsmps();

            //csound->GetAudioChannel("out2", audioChannel2);
			int controlIndex = 0;
			int audioInputIndex = 0;
			int audioOutputIndex = 0;
			for( int i = 0 ; i < (int)cabbageControls.size();i++)
			{
				if(cabbageControls[i].hasChannel && compileError == 0)
				{
					// if(cabbageControls[i].type == "cvoutput")
            		// 	csound->GetAudioChannel(cabbageControls[i].channel.c_str(), audioOutputChannels[audioOutputIndex++]);
					// else if(cabbageControls[i].type == "cvinput")
            		//     	csound->SetChannel(cabbageControls[i].channel.c_str(), audioInputChannels[audioInputIndex++]);
					// else
						csound->SetChannel(cabbageControls[i].channel.c_str(), params[controlIndex++].value);
				}				
			}
		}
		
		if (compileError == 0)
		{
			for ( int i = 0 ; i < audioInputControls.size() ; i++)
			 	audioInputChannels[i][kIndex] = inputs[i].value;	
			for ( int i = 0 ; i < audioOutputControls.size() ; i++)
			 	outputs[i].value = (audioOutputChannels[i][kIndex] / csScale ) * 10.f;	
			
            kIndex++;
		}
	}
}



