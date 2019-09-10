
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
				addInput(createInput<CabbagePort>(Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]), module, aRateInputIndex++));
				// addInput(new CabbagePort(control, module, aRateInputIndex++));	

			else if(control.type == "cvoutput")
				//addOutput(new CabbagePort(control, module, aRateOutputIndex++));
				addOutput(createOutput<CabbagePort>(Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]), module, aRateOutputIndex++));
				
			else if(control.type == "light")
			 	addChild(new CabbageLight(control, module, lightIndex++));		

			else if(control.type == "screw")
				addChild(createWidget<CabbageScrew>(Vec(control.bounds[Bounds::x], control.bounds[Bounds::y])));
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
	}
}