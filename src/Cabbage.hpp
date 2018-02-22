#include "rack.hpp"
#include "csound.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>



enum Range {
	min, max, value	
};

enum Colour {
	r,g,b,a
};

enum Bounds {
	x, y, width, height 
};

using namespace rack;
using namespace std;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct MyModuleWidget : ModuleWidget {
	MyModuleWidget();
};

///////////////////////
// parsing Cabbage file
///////////////////////
struct CabbageControl
{
	float range[3] = {0, 1, 0};
	int colour[4] = {0,0,0,255};
	int trackerColour[4] = {0,0,0,255};
	int outlineColour[4] = {0,0,0,255};
	int bounds[4] = {0,0,100,100};
	int width, height;
	string channel, text, label, caption, type;
	bool hasChannel = false;
};

static vector<CabbageControl> getCabbageControlVector(string csdFile)
{
	vector<CabbageControl> cabbageControls;

	std::ifstream input(csdFile.c_str());

	std::string line;
	while (std::getline(input, line))
	{
		if (line.find("</") != std::string::npos)
			break;

		string newLine = line;
		string control = line.substr(0, line.find(" ") != std::string::npos ? line.find(" ") : 0);
		std::string::size_type i = newLine.find(control);

		if (i != std::string::npos)
			newLine.erase(i, control.length());

		if (control.find("slider") != std::string::npos ||
			control.find("button") != std::string::npos ||
			control.find("checkbox") != std::string::npos ||
			control.find("groupbox") != std::string::npos ||
			control.find("form") != std::string::npos)
		{
			CabbageControl cabbageCtrl;
			cabbageCtrl.type = control;
			//init range
			cabbageCtrl.range[Range::min] = 0;
			cabbageCtrl.range[Range::max] = 1;
			cabbageCtrl.range[Range::value] = 0;

			if (line.find("caption(") != std::string::npos)
			{
				string infoText = line.substr(line.find("caption(") + 9);
				infoText = infoText.substr(0, infoText.find(")") - 1);
				cabbageCtrl.caption = infoText;
			}

			if (line.find("text(") != std::string::npos)
			{
				string text = line.substr(line.find("text(") + 6);
				text = text.substr(0, text.find(")") - 1);
				cabbageCtrl.text = text;
			}

			if (line.find("channel(") != std::string::npos)
			{
				string channel = line.substr(line.find("channel(") + 9);
				channel = channel.substr(0, channel.find(")") - 1);
				cabbageCtrl.channel = channel;
				cabbageCtrl.hasChannel = true;
			}

			if (line.find("colour(") != std::string::npos)
			{
				string colour = line.substr(line.find("colour(") + 7);
				colour = colour.substr(0, colour.find(")"));
				char *p = strtok(&colour[0u], ",");
				int argCount = 0;
				while (p)
				{
					cabbageCtrl.colour[argCount] = atof(p);
					argCount++;
					//not handling increment or log sliders yet
					if (argCount == 4)
						break;
					p = strtok(NULL, ",");
				}
			}

			if (line.find("outlinecolour(") != std::string::npos)
			{
				string colour = line.substr(line.find("outlinecolour(") + 14);
				colour = colour.substr(0, colour.find(")"));
				char *p = strtok(&colour[0u], ",");
				int argCount = 0;
				while (p)
				{
					cabbageCtrl.outlineColour[argCount] = atof(p);
					argCount++;
					//not handling increment or log sliders yet
					if (argCount == 4)
						break;
					p = strtok(NULL, ",");
				}
			}

			if (line.find("trackercolour(") != std::string::npos)
			{
				string colour = line.substr(line.find("trackercolour(") + 14);
				colour = colour.substr(0, colour.find(")"));
				char *p = strtok(&colour[0u], ",");
				int argCount = 0;
				while (p)
				{
					cabbageCtrl.trackerColour[argCount] = atof(p);
					argCount++;
					//not handling increment or log sliders yet
					if (argCount == 4)
						break;
					p = strtok(NULL, ",");
				}
			}

			if (line.find("bounds(") != std::string::npos)
			{
				string bounds = line.substr(line.find("bounds(") + 7);
				bounds = bounds.substr(0, bounds.find(")"));
				char *p = strtok(&bounds[0u], ",");
				int argCount = 0;
				while (p)
				{
					cabbageCtrl.bounds[argCount] = atof(p);
					argCount++;
					//not handling increment or log sliders yet
					if (argCount == 4)
						break;
					p = strtok(NULL, ",");
				}
			}

			if (line.find("size(") != std::string::npos)
			{
				string size = line.substr(line.find("size(") + 5);
				cabbageCtrl.width = stoi(size.substr(0, size.find(",")));
				cabbageCtrl.height = stoi(size.substr(size.find(",")+1, size.find(")")));
			}

			if (line.find("range(") != std::string::npos)
			{
				string range = line.substr(line.find("range(") + 6);
				range = range.substr(0, range.find(")"));
				char *p = strtok(&range[0u], ",");
				int argCount = 0;
				while (p)
				{
					cabbageCtrl.range[argCount] = atof(p);
					argCount++;
					//not handling increment or log sliders yet
					if (argCount == 3)
						break;
					p = strtok(NULL, ",");
				}
			}

			if (line.find("value(") != std::string::npos)
			{
				string value = line.substr(line.find("value(") + 6);
				value = value.substr(0, value.find(")"));
				cabbageCtrl.range[Range::value] = value.length() > 0 ? atof(value.c_str()) : 0;
			}

			cabbageControls.push_back(cabbageCtrl);
		}
	}


	return cabbageControls;
}



static int getNumberOfControlChannels(vector<CabbageControl> csndChannels)
{
	int numberOfControlChannels = 0;

	for (auto channel : csndChannels)
	{
		if(channel.type != "form" && channel.type != "groupbox" )
		numberOfControlChannels++;
	}

	return numberOfControlChannels;
}