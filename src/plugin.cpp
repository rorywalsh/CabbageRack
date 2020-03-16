#include "plugin.hpp"


Plugin *pluginInstance;

#ifdef ARCH_WIN
__declspec(dllexport) void init(Plugin *p) {
#else
void init(Plugin *p) {
#endif
	pluginInstance = p;
	const std::string pluginName = pluginInstance->path.substr(pluginInstance->path.find_last_of("/")+1);
	std::cout << pluginName;
	// Add modules here
	modelCabbageRack->slug = pluginName;
	p->addModel(modelCabbageRack);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}


