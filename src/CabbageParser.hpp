#include "rack.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iomanip>


enum Range {
	min, max, value	
};

enum Colour {
	r,g,b,a
};

enum Bounds {
	x, y, width, height 
};

//====================================
// parsing Cabbage file
//====================================
struct CabbageControl
{
	
	NVGcolor colour, trackerColour,outlineColour,fontColour,textColour,colour0,colour1;

	std::vector<int> bounds = {0,0,100,100};
	std::vector<float> range = {0, 1, 0, 1, .01};
	int width, height, corners=3;
	float value = 0.f;
	string channel, label, caption, type;
	vector<string> text;
	bool hasChannel = false;
	string file = "";

	CabbageControl(string type)
	{
		//set defaults for each type of widget
		if(type == "rslider")
		{
			colour = nvgRGBA(220, 220, 220, 255);
			trackerColour = nvgRGBA(147, 210, 0, 255);
			outlineColour = nvgRGBA(25, 25, 25, 255);
			textColour = nvgRGBA(255, 255, 255, 255);
			bounds = {0, 0, 80, 80};
			range = {0, 1, 0, 1, .01};
			text.push_back("");	
		}
		else if(type == "button")
		{
			colour1 = nvgRGBA(30, 30, 30, 255);
			colour0 = nvgRGBA(20, 20, 20, 255);
			textColour = nvgRGBA(255, 255, 255, 255);
			fontColour = nvgRGBA(255, 255, 255, 255);
			bounds = {0, 0, 80, 30};
			range = {0, 1, 0, 1, 1};
			text.push_back("Off");
			text.push_back("On");				
		}
		else if(type == "checkbox")
		{
			colour1 = nvgRGBA(147, 210, 0, 255);
			colour0 = nvgRGBA(20, 20, 20, 255);
			textColour = nvgRGBA(255, 255, 255, 255);
			fontColour = nvgRGBA(255, 255, 255, 255);
			bounds = {0, 0, 80, 30};
			range = {0, 1, 0, 1, 1};
			text.push_back("");	
		}
		else if(type == "label")
		{
			colour = nvgRGBA(255, 255, 255, 0);
			fontColour = nvgRGBA(255, 255, 255, 255);
			bounds = {0, 0, 80, 30};
			text.push_back("");	
		}
		else if(type == "groupbox")
		{
			colour = nvgRGBA(255, 255, 255, 0);
			fontColour = nvgRGBA(255, 255, 255, 255);
			outlineColour = nvgRGBA(255, 255, 255, 255);
			bounds = {0, 0, 80, 30};
			text.push_back("Groupbox");	
		}
		else if(type == "image")
		{
			colour = nvgRGBA(255, 255, 255, 255);
			outlineColour = nvgRGBA(255, 255, 255, 255);
			bounds = {0, 0, 80, 30};
		}
		else if(type == "form")
		{
			colour = nvgRGBA(20, 20, 20, 255);
		}
	}
};

struct CabbageParser
{
	static int* getColourFromText(string colIdent, string line)
	{
		const string colourIdentifier = colIdent+"(";
		static int colourArray[4] = {255,255,255,255};
		string colour = line.substr(line.find(colourIdentifier) + colourIdentifier.size());
		colour = colour.substr(0, colour.find(")"));
		const int numArgs = std::count(colour.begin(), colour.end(), ',');
		char *p = strtok(&colour[0u], ",");
		int argCount = 0;
		
		while (p)
		{

			colourArray[argCount] = atof(p);
			
			if (argCount == numArgs)
				break;
			p = strtok(NULL, ",");
			
			argCount++;
		}

		return colourArray;
	}

	static string getParameter(string line, string identifier)
	{
		if (line.find(identifier) != std::string::npos)
		{
			string value = line.substr(line.find(identifier) + identifier.size());
			value = value.substr(0, value.find(")"));
			value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
			return value;
		}
	}			

	static string trim(const std::string& str,
					const std::string& whitespace = " \t")
	{
		int start = str.find_first_not_of(whitespace);
		if (start == std::string::npos)
			return ""; // no content

		const int end = str.find_last_not_of(whitespace);
		const int  range = end - start + 1;

		return str.substr(start, range);
	}

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
				control.find("form") != std::string::npos ||
				control.find("image") != std::string::npos ||
				control.find("label") != std::string::npos)
			{
				CabbageControl cabbageCtrl(control);
				cabbageCtrl.type = control;
				//init range
				cabbageCtrl.range[Range::min] = 0;
				cabbageCtrl.range[Range::max] = 1;
				cabbageCtrl.range[Range::value] = 0;

				if (line.find(" caption(") != std::string::npos)
					cabbageCtrl.caption = getParameter(line, "caption(");

				if (line.find(" text(") != std::string::npos)
				{
					string text = line.substr(line.find(" text(") + 7);
					text = text.substr(0, text.find(")") - 1);
					text.erase(std::remove(text.begin(), text.end(), '\"'), text.end());
					const int numArgs = std::count(text.begin(), text.end(), ',')+1;
					int argIndex = 0;
					if(numArgs>1)
					{
						char myString[32];
						sprintf(myString, text.c_str(), 32);
						char *p = strtok(myString, ",");

						while (p) 
						{
							cabbageCtrl.text[argIndex] = trim(string(p));							
							p = strtok(NULL, ",");
						}
					}
					else
						cabbageCtrl.text[0] = text;
				}

				if (line.find(" channel(") != std::string::npos)
				{
					cabbageCtrl.channel = getParameter(line, " channel(");
					cabbageCtrl.hasChannel = true;
				}

				if (line.find(" file(") != std::string::npos)
				{
					string file = getParameter(line, " file(");
					file.erase(std::remove(file.begin(), file.end(), '\"'), file.end());
					cabbageCtrl.file = file;
				}

				if (line.find(" colour(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" colour", line);
					cabbageCtrl.colour = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" outlinecolour(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" outlinecolour", line);
					cabbageCtrl.outlineColour = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" colour:0(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" colour:0", line);
					cabbageCtrl.colour0 = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" colour:1(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" colour:1", line);
					cabbageCtrl.colour1 = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" textcolour(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" textcolour", line);
					cabbageCtrl.textColour = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" trackercolour(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" trackercolour", line);
					cabbageCtrl.trackerColour = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" fontcolour(") != std::string::npos)
				{
					int *colourArray = getColourFromText(" fontcolour", line);
					cabbageCtrl.fontColour = nvgRGBA(colourArray[0], colourArray[1], colourArray[2], colourArray[3]);
				}

				if (line.find(" bounds(") != std::string::npos)
				{
					string bounds = line.substr(line.find(" bounds(") + 8);
					bounds = bounds.substr(0, bounds.find(")"));
					char *p = strtok(&bounds[0u], ",");
					int argCount = 0;
					while (p)
					{
						cabbageCtrl.bounds[argCount] = atof(p);
						argCount++;
						if (argCount == 4)
							break;
						p = strtok(NULL, ",");
					}
				}

				if (line.find(" size(") != std::string::npos)
				{
					string size = line.substr(line.find(" size(") + 6);
					cabbageCtrl.width = stoi(size.substr(0, size.find(",")));
					cabbageCtrl.height = stoi(size.substr(size.find(",")+1, size.find(")")));
				}

				if (line.find(" range(") != std::string::npos)
				{
					string range = line.substr(line.find(" range(") + 7);
					range = range.substr(0, range.find(")"));
					char *p = strtok(&range[0u], ",");
					int argCount = 0;
					while (p)
					{
						cabbageCtrl.range[argCount] = atof(p);
						argCount++;
						if (argCount == 3)
							break;
						p = strtok(NULL, ",");
					}
				}

				if (line.find(" value(") != std::string::npos)
					cabbageCtrl.value = getParameter(line, " value(").length() > 0 ? atof(getParameter(line, " value(").c_str()) : 0;

				if (line.find(" corners(") != std::string::npos)				
					cabbageCtrl.corners = atoi(getParameter(line, " corners(").c_str());

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
};