
#include "CabbageRackModule.hpp"
#include "CabbageWidgets.hpp"


CabbageRackWidget::CabbageRackWidget(CabbageRackModule *module) 
{
	
    setModule(module);
	//Cabbage will need to export a dummy SVG...
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CabbageRack.svg")));
	box.size.x = 40;
    int kRateControlIndex = 0;
	int aRateInputIndex = 0;
	int aRateOutputIndex = 0;
	int lightIndex = 0;

    

    // CabbageForm* form = new CabbageForm();

	//int width, height, centre;
    if (module)
    {
        for( auto& control : module->cabbageControls)
        {
            if(control.type == "form")
            {
                CabbageForm* form = new CabbageForm(control);
				std::cout << std::endl << control.bounds[Bounds::width] << std::endl;
				box.size.x = control.width;
                addChild(form);	
            }		

			else if(control.type == "label")
				addChild(new CabbageLabel(control));

			else if(control.type == "groupbox")
				addChild(new CabbageGroupbox(control));

			else if(control.type == "image")
				addChild(new CabbageImage(control));

			//these were customised in v0.6, but can't get them to work in v1.n
			else if(control.type == "cvinput")
				addInput(createInput<PJ301MPort>(Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]), module, aRateInputIndex++));
				// addInput(new CabbagePort(control, module, aRateInputIndex++));	

			else if(control.type == "cvoutput")
				//addOutput(new CabbagePort(control, module, aRateOutputIndex++));
				addOutput(createOutput<PJ301MPort>(Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]), module, aRateOutputIndex++));
				
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
				else if(control.type == "cslider")
					addParam(new CabbageChoiceSlider(control, module, kRateControlIndex++));			
			}	
		}
		
		}

	addChild(createWidget<ScrewSilver>(Vec(0, 0)));
	addChild(createWidget<ScrewSilver>(Vec(box.size.x - 16, 0)));
	addChild(createWidget<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createWidget<ScrewSilver>(Vec(box.size.x - 16, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		// addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 46.063)), module, CabbageRackModule::PITCH_PARAM));

		// addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 77.478)), module, CabbageRackModule::PITCH_INPUT));

		// addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.713)), module, CabbageRackModule::SINE_OUTPUT));

		// addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 25.81)), module, CabbageRackModule::BLINK_LIGHT));
}