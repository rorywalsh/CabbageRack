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
	int lightIndex = 0;
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

		else if(control.type == "cvinput")
			addInput(new CabbagePort(control, module, aRateInputIndex++));	

		else if(control.type == "cvoutput")
			addOutput(new CabbagePort(control, module, aRateOutputIndex++));

		else if(control.type == "light")
		 	addChild(new CabbageLight(control, module, lightIndex++));		

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
		}	
	
	}

	addChild(createScrew<ScrewSilver>(Vec(0, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 16, 0)));
	addChild(createScrew<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 16, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

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
	lights.resize(numLights);
	outputs.resize(audioOutputControls.size());              
	csound->SetMessageCallback(MessageCallback);
}

void CabbageRack::createAndCompileCsound() 
{
	csound = new Csound(); 
	csound->SetOption((char*)"-n");
	csound->SetOption((char*)"-d");
	const string csdFileName(plugin->path+ "/" +plugin->path.substr(plugin->path.find("/", 3)+1)+".csd");
	csound->SetHostImplementedAudioIO(1, 0);

#ifndef ARCH_MAC
	CSOUND_PARAMS* csoundParams = nullptr;
	csoundParams = new CSOUND_PARAMS();
	csoundParams->sample_rate_override = engineGetSampleRate();
	csoundParams->displays = 0;		
	csound->SetParams(csoundParams);
	compileError = csound->Compile(csdFileName.c_str());
#else
	string sr_override = "--sample-rate=" + to_string(engineGetSampleRate());
	compileError = csound->Compile(csdFileName.c_str(), (char *) sr_override.c_str());
#endif
	
	

	cabbageControls = CabbageParser::getCabbageControlVector(csdFileName);
	numControlChannels = CabbageParser::getNumberOfControlChannels(cabbageControls);
	numLights = CabbageParser::getNumberOfLights(cabbageControls);
	
	audioOutputControls = CabbageParser::getAudioChannels(cabbageControls, "cvoutput");
	audioInputControls = CabbageParser::getAudioChannels(cabbageControls, "cvinput");

	if(compileError != 0)
		cout << "Csound could not compile" << endl;
	else
	{    
		ksmps = csound->GetKsmps();
		audioOutputChannels = new MYFLT*[audioOutputControls.size()];
		audioInputChannels = new MYFLT*[audioInputControls.size()];

		for( int i = 0 ; i < (int)audioOutputControls.size() ; i++)
		{
			audioOutputChannels[i] = new MYFLT[ksmps];
			csoundGetChannelPtr(csound->GetCsound(), &audioOutputChannels[i], audioOutputControls[i].channel.c_str(),
                            CSOUND_AUDIO_CHANNEL | CSOUND_OUTPUT_CHANNEL);
		}
        	
		for( int i = 0 ; i < (int)audioInputControls.size() ; i++)
		{
			audioInputChannels[i] = new MYFLT[ksmps];
			csoundGetChannelPtr(csound->GetCsound(), &audioInputChannels[i], audioInputControls[i].channel.c_str(),
                           CSOUND_AUDIO_CHANNEL | CSOUND_INPUT_CHANNEL);
		}      	

		for ( auto& control : cabbageControls)
			csound->SetChannel(control.channel.c_str(), control.value);

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
			if(csound)
				compileError = csound->PerformKsmps();

			int controlIndex = 0;
			int lightIndex = 0;

			if(compileError == 0)
			{
				for( int i = 0 ; i < (int)cabbageControls.size();i++)
					if(cabbageControls[i].hasChannel)
						csound->SetChannel(cabbageControls[i].channel.c_str(), params[controlIndex++].value);	
					else if(cabbageControls[i].isLight)
					 	lights[lightIndex++].value = csound->GetChannel(cabbageControls[i].channel.c_str());			

			}
		}
		
		for ( int i = 0 ; i < (int)audioInputControls.size() ; i++)
			audioInputChannels[i][kIndex] = inputs[i].value;	
		for ( int i = 0 ; i < (int)audioOutputControls.size() ; i++)
			outputs[i].value = (audioOutputChannels[i][kIndex] / csScale ) * 10.f;	
		
		kIndex++;
	}
}



