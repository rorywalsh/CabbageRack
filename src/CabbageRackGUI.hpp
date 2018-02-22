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


using namespace std;

struct CabbageRotarySlider : virtual Knob, FramebufferWidget 
{
	NVGcolor outline = nvgRGBA(10, 10, 10, 255);
	NVGcolor inner = nvgRGBA(55, 55, 55, 255);
	NVGcolor tracker = nvgRGBA(147, 210, 0, 255);
	std::shared_ptr<Font> font;
	char text[128];
	int textHeight = 12;

	CabbageRotarySlider()
	{
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		strncpy(text, "Freq.", sizeof(text));
	}

	void setColours(NVGcolor out, NVGcolor in, NVGcolor track)
	{
		outline = out;
		inner = in;
		tracker = track;
	}

	void draw(NVGcontext *vg) override 
	{	
		const int centerx = box.size.x/3.f + box.size.x/6.f;
		const int centery = box.size.y/3.f;
		const int innerSize = box.size.x*.6f;
		const int radius = innerSize/2;
		Widget::draw(vg);
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, box.size.x, box.size.y);
		nvgFillColor(vg, nvgRGBA(255, 9, 0, 255));
		nvgFill(vg);

		//pie segment
		nvgBeginPath(vg);
		nvgMoveTo(vg, centerx, centery);
		nvgArc(vg, centerx, centery, radius, (NVG_PI / 1.5), (NVG_PI / 1.5) + ((NVG_PI* 1.70) * value/1000.f), NVG_CW);   // from angle 45~270
		nvgLineTo(vg, centerx, centery);
		nvgFillColor(vg, tracker);
		nvgFill(vg);
		//outer circle
		nvgBeginPath(vg);					
		nvgCircle(vg, centerx, centery, radius);
		nvgStrokeWidth(vg, 1);
		nvgStrokeColor(vg, outline);
		nvgStroke(vg);
		//innerCircle
		nvgBeginPath(vg);
		nvgCircle(vg, centerx, centery, radius * .6f);
		nvgFillColor(vg, inner);
		nvgFill(vg);
		//text
		nvgBeginPath(vg);
		nvgFontSize(vg, textHeight);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
		nvgText(vg, 0, box.size.y-textHeight, "HELLO", NULL);
		nvgFill(vg);
	}
	

};


struct CabbageForm : Widget 
{
	NVGcolor col;

 	CabbageForm(Vec size, int r, int g, int b, int a = 255) 
	{
        box.size = size;
        col = nvgRGBA(r, g, b, a);
    }
	
    void draw(NVGcontext *vg) override 
    {
		Widget::draw(vg);
		nvgBeginPath(vg);
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
        nvgFillColor(vg, col);
        nvgFill(vg);
	}

};

struct CabbageLabel : Widget 
{
	std::shared_ptr<Font> font;
	NVGcolor labelColour = nvgRGBA(255,0,0,255);
    NVGcolor backgroundColour = nvgRGBA(0,255,0,255);
	char text[128];
	int height = 14;

 	CabbageLabel(Vec pos, Vec size, const char* labelText) 
	{
		box.pos = pos;
		box.size = size;
		height = size.y;
		strncpy(text, labelText, sizeof(text));
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
	}

    void setBackgroundColour(int r, int g, int b, int a)
    {
        backgroundColour = nvgRGBA(r, g, b, a);
    }

    void setLabelColour(int r, int g, int b, int a)
    {
        labelColour = nvgRGBA(r, g, b, a);
    }

	void draw(NVGcontext *vg) override {
		Widget::draw(vg);
		nvgBeginPath(vg);
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
        nvgFillColor(vg, backgroundColour);
        nvgFill(vg);
		nvgFontSize(vg, height*.9);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg,labelColour);
		nvgText(vg, 0, box.size.y-(height*.2), text, NULL);
	}

};

static string createRGBString(int r, int g, int b)
{   
    int rgba = (r<<16) | (g<<8) | b;
    std::stringstream stream;
    stream << std::setw(6) << std::setfill('0') << std::hex << rgba;
    std::string result( stream.str() );
    std::stringstream output;
    output << "\"#" << result << "\"";
    return output.str();
}

static void createFormBackgroundSVG(string path, int width, int height, int r, int g, int b, int a)
{
	std::ostringstream svgText;
	svgText << "<svg width=\"" << width << "\" height=\"" << height << "\">" <<
	"\n<rect width=\"" << width << "\" height=\"" << height << "\" style=\"fill:rgb(" << r << "," << g << "," << b << ") fill-opacity:" << float(a)/255.f <<
	"stroke-width:0\" />" << "\n</svg>";

	ofstream imgFile;
  	imgFile.open (path + "/res/background.svg");
	imgFile << svgText.str();
	imgFile.close(); 
}


static void createRSliderSVG(string path, int width, int height, string outline, string colour, string tracker)
{
    std::ostringstream svgText;
    int innerSize = width*.9f;
    svgText << "<svg height=\"" << width << "\" width=\"" << height << "\">\n";
    svgText << "  <circle cx=\"" << width/2 << "\" cy=\"" << width/2 << "\" r=\"" << innerSize/2 << "\" stroke-width=\"1\" stroke=" << outline << " fill-opacity=\"0\" />\n";
    svgText << "  <circle cx=\"" << width/2 << "\" cy=\"" << width/2 << "\" r=\"" << (innerSize/2) * float(3.f/5.f) << "\" fill=" << colour << " />\n";
    svgText << "  <rect width=\"" << width*.16 << "\" height=\"" << height*.22 << "\" x=\"" << width*.42 << "\" y=\"" << 1 << "\" fill=" << colour << " />\n";
    svgText << "  <rect width=\"" << width*.14 << "\" height=\"" << height*.21 << "\" x=\"" << width*.43 << "\" y=\"" << 2 << "\" fill=" << tracker << " />\n";
    svgText << "</svg> ";
    cout << "==================\n" << svgText.str() << "\n=======================";

	ofstream imgFile;
  	imgFile.open (path + "/res/rslider.svg");
	imgFile << svgText.str();
	imgFile.close(); 
    
}
