#include "plugin.hpp"


Plugin *pluginInstance;


void init(Plugin *p) {
	pluginInstance = p;
	const std::string pluginName = pluginInstance->path.substr(pluginInstance->path.find("/", 3)+1);

	// Add modules here
	p->addModel(modelCabbageRack);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
