#include "plugin.hpp"


Plugin *pluginInstance;


void init(Plugin *p) {
	pluginInstance = p;
	const std::string pluginName = pluginInstance->path.substr(pluginInstance->path.find_last_of("/")+1);
	std::cout << pluginName;
	// Add modules here
	modelCabbageRack->slug = pluginName;
	p->addModel(modelCabbageRack);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}


