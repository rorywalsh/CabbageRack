
#include "CabbageRackModule.hpp"



void MessageCallback(CSOUND* cs, int attr, const char *format, va_list valist)
{
  vprintf(format, valist);   
  return;
}

CabbageRackModule::CabbageRackModule()
{
	//setting up 1000 dummy parameters to start, resized later..
	config(1000, 2, 2, 2);
	createAndCompileCsound();                

	inputs.resize(audioInputControls.size());
	lights.resize(numLights);
	outputs.resize(audioOutputControls.size());              
	csound->SetMessageCallback(MessageCallback);
}

void CabbageRackModule::createAndCompileCsound() 
{
	csound = new Csound(); 
	csound->SetOption((char*)"-n");
	csound->SetOption((char*)"-d");
	const std::string csdFileName(pluginInstance->path+ "/" + pluginInstance->slug+".csd");
	
	csound->SetHostImplementedAudioIO(1, 0);

#ifndef ARCH_MAC
	CSOUND_PARAMS* csoundParams = nullptr;
	csoundParams = new CSOUND_PARAMS();
	// csoundParams->sample_rate_override = sampleRate;
	csoundParams->displays = 0;		
	csound->SetParams(csoundParams);
	compileError = csound->Compile(csdFileName.c_str());
#else
	//std::string sr_override = "--sample-rate=" + to_string(engineGetSampleRate());
	compileError = csound->Compile(csdFileName.c_str());
#endif
	
	
	int controlIndex = 0;
	cabbageControls = CabbageParser::getCabbageControlVector(csdFileName);
	paramQuantities.resize(cabbageControls.size());  
	
	for ( auto& control : cabbageControls)
	{
		if(control.hasChannel == true)
		{
			configParam(controlIndex, control.range[Range::min], control.range[Range::max], control.range[Range::value]);
			controlIndex++;
		}
	}


	numControlChannels = CabbageParser::getNumberOfControlChannels(cabbageControls);
	numLights = CabbageParser::getNumberOfLights(cabbageControls);
	
	audioOutputControls = CabbageParser::getAudioChannels(cabbageControls, "cvoutput");
	audioInputControls = CabbageParser::getAudioChannels(cabbageControls, "cvinput");

	if(compileError != 0)
		std::cout << "Csound could not compile" << std::endl;
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
		{
			csound->SetChannel(control.channel.c_str(), control.value);
		}
		csScale = csound->Get0dBFS();
	}	
}

void CabbageRackModule::process(const ProcessArgs &args)
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




Model *modelCabbageRack = createModel<CabbageRackModule, CabbageRackWidget>("CabbageRack");