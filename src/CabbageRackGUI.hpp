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

//===================================================================
// Form class
//===================================================================
struct CabbageForm : Widget 
{
	NVGcolor col;

 	CabbageForm(Vec size, NVGcolor colour) 
	{
        box.size = size;
        col = colour;
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

//===================================================================
// Rotary Slider class
//===================================================================
struct CabbageRotarySlider : virtual Knob, FramebufferWidget 
{
	NVGcolor outline = nvgRGBA(10, 10, 10, 255);
	NVGcolor inner = nvgRGBA(55, 55, 55, 255);
	NVGcolor tracker = nvgRGBA(147, 210, 0, 255);
	NVGcolor fontColour = nvgRGBA(255, 255, 255, 255);
	std::shared_ptr<Font> font;
	char text[128];
	int textHeight = 12;

	CabbageRotarySlider()
	{
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
	}

	void setColours(NVGcolor out, NVGcolor in, NVGcolor track)
	{
		outline = out;
		inner = in;
		tracker = track;
	}

	void setText(string slidertext)
	{
		strncpy(text, slidertext.c_str(), sizeof(text));
	}

	void draw(NVGcontext *vg) override 
	{	
		const int centerx = box.size.x/2.65f + box.size.x/7.5f;
		const int centery = box.size.y/2.65f;
		const int innerSize = box.size.x*.6f;
		const int radius = innerSize/2;
		Widget::draw(vg);
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
		nvgClosePath(vg);
		//text
		nvgBeginPath(vg);
		nvgFontSize(vg, textHeight);
		nvgFontFaceId(vg, font->handle);
		
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, fontColour);
		nvgTextAlign(vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgTextBox(vg, 0, box.size.y-14, box.size.x, text, NULL);
		nvgFill(vg);
	}
};

//===================================================================
// Checkbox class
//===================================================================
struct CabbageCheckbox : virtual Switch, FramebufferWidget {

	std::shared_ptr<Font> font;
	NVGcolor onColour = nvgRGBA(147, 210, 0, 255);
	NVGcolor offColour = nvgRGBA(55, 55, 55, 255);
	NVGcolor fontColour = nvgRGBA(255, 255, 255, 255);
	char text[120];

	CabbageCheckbox()
	{
		box.size.x = 50;
		box.size.y = 14;
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		setText("Combo");
	}

	void setColours(NVGcolor on, NVGcolor off, NVGcolor fontCol)
	{
		onColour = on;
		offColour = off;
		fontColour = fontCol;
	}

	void setText(string boxText)
	{
		strncpy(text, boxText.c_str(), sizeof(text));
	}

	void onMouseDown(EventMouseDown &e) override 
	{
		if (value >= maxValue)
			setValue(minValue);
		else
			setValue(maxValue);
	}

	void draw(NVGcontext *vg) override 
	{
		Widget::draw(vg);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 0, 0, box.size.y, box.size.y, 2);
		nvgFillColor(vg, nvgRGBA(20, 20, 20, 255));
		nvgRoundedRect(vg, 1, 1, box.size.y-1, box.size.y-1, 2);
		nvgFillColor(vg, nvgRGBA(80, 80, 80, 255));

		nvgRoundedRect(vg, 2, 2, box.size.y-4, box.size.y-4, 2);
		if (value==0)
			nvgFillColor(vg, nvgRGBA(255, 255, 0, 255));
		else
			nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));
		nvgFill(vg);

		nvgBeginPath(vg);
		nvgFontSize(vg, box.size.y);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, fontColour);
		nvgText(vg, box.size.y+4, box.size.y-2, text, NULL);
		nvgFill(vg);
	}
};

//===================================================================
// Label class
//===================================================================
struct CabbageLabel : Widget 
{
	std::shared_ptr<Font> font;
	NVGcolor labelColour = nvgRGBA(255,255,255,255);
    NVGcolor backgroundColour = nvgRGBA(0,0,0,0);
	char text[128];
	int height = 14;

 	CabbageLabel(Vec pos, Vec size, string labelText) 
	{
		box.pos = pos;
		box.size = size;

		height = size.y;
		strncpy(text, labelText.c_str(), sizeof(text));
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
	}

    void setBackgroundColour(NVGcolor colour)
    {
        backgroundColour = colour;
    }

    void setLabelColour(NVGcolor colour)
    {
        labelColour = colour;
    }

	void draw(NVGcontext *vg) override {
		Widget::draw(vg);
		nvgBeginPath(vg);
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 3);
        nvgFillColor(vg, backgroundColour);
        nvgFill(vg);
		nvgClosePath(vg);
		nvgFontSize(vg, height*.9);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgTextAlign(vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgFillColor(vg,labelColour);
		nvgTextBox(vg, 0, 3, box.size.x, text, NULL);
		
	}

};

//===================================================================
// Input / Output port
//===================================================================
struct CabbagePort : Port, FramebufferWidget 
{
	std::shared_ptr<Font> font;
	char text[10];
	CabbagePort()
	{
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		box.size.x = 30;
		box.size.y = 40;
	};

	void setText(string portText)
	{
		strncpy(text, portText.c_str(), sizeof(text));
	}

	void draw(NVGcontext *vg) override
	{
		Widget::draw(vg);
		nvgBeginPath(vg);
        nvgRoundedRect(vg, 0, -5, box.size.x, box.size.y, 3);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
        nvgFill(vg);
		nvgBeginPath(vg);
        nvgRoundedRect(vg, 1, -4, box.size.x-2, box.size.y-2, 3);
        nvgFillColor(vg, nvgRGBA(30, 30, 30, 255));
        nvgFill(vg);
		nvgClosePath(vg);
		nvgFontSize(vg, 10);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
		nvgTextAlign(vg, NVG_ALIGN_CENTER);
		nvgTextBox(vg, 0, 6, box.size.x, text, NULL);
		//nvgText(vg, 5, 6, text, NULL);
		nvgBeginPath(vg);
        nvgCircle(vg, 15, 20, 10);
        nvgFillColor(vg, nvgRGBA(80, 80, 80, 255));
		nvgFill(vg);
		nvgBeginPath(vg);
        nvgCircle(vg, 15, 20, 8);
        nvgFillColor(vg, nvgRGBA(180, 180, 180, 255));
		nvgFill(vg);
		nvgBeginPath(vg);
        nvgCircle(vg, 15, 20, 7);
        nvgFillColor(vg, nvgRGBA(10, 10, 10, 255));
        nvgFill(vg);
	}
};