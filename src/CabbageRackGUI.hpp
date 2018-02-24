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
	NVGcolor outline, inner, tracker, fontColour;
	std::shared_ptr<Font> font;
	string text;
	int textHeight = 12;

	CabbageRotarySlider()
	{
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
	}

	void setColours(NVGcolor out, NVGcolor in, NVGcolor track, NVGcolor fCol)
	{
		outline = out;
		inner = in;
		tracker = track;
		fontColour = fCol;
	}

	void setText(string slidertext)
	{
		text = slidertext.c_str();
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
		nvgTextBox(vg, 0, box.size.y-14, box.size.x, text.c_str(), NULL);
		nvgFill(vg);
	}
};

//===================================================================
// Button class
//===================================================================
struct CabbageButton : virtual Switch, FramebufferWidget {

	std::shared_ptr<Font> font;
	NVGcolor onColour, offColour, fontColour;
	string text[2];

	CabbageButton()
	{
		box.size.x = 50;
		box.size.y = 14;
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		setText("On", "Off");
	}

	void setColours(NVGcolor off, NVGcolor on, NVGcolor fontCol)
	{
		onColour = on;
		offColour = off;
		fontColour = fontCol;
	}

	void setText(string onButtonText, string offButtonText)
	{
		text[1] = onButtonText;
		text[0] = offButtonText;
	}

	void onMouseDown(EventMouseDown &e) override 
	{
		if (value >= maxValue)
			setValue(minValue);
		else
			setValue(maxValue);
	}

	void onMouseUp(EventMouseUp &e) override
	{

	}

	void draw(NVGcontext *vg) override 
	{
		Widget::draw(vg);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 0, 0, box.size.x-int(value), box.size.y-int(value), 4);
		nvgFillColor(vg, nvgRGBA(80, 80, 80, 255));
		nvgFill(vg);
		nvgClosePath(vg);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 1, 1, box.size.x-2-int(value), box.size.y-1-int(value), 4);
		nvgFillColor(vg, nvgRGBA(170, 170, 170, 255));
		nvgFill(vg);
		nvgClosePath(vg);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 2, 2, box.size.x-3-int(value), box.size.y-2-int(value), 4);
		if (value==0)
			nvgFillColor(vg, offColour);
		else
			nvgFillColor(vg, onColour);
		nvgFill(vg);

		nvgBeginPath(vg);
		const float numChars = text[int(value)].size();
		nvgFontSize(vg, 11);//box.size.y*(numChars/box.size.y));
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, fontColour);
		nvgTextAlign(vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgTextBox(vg, 0, (box.size.y/2)-(11.f/2.f)-int(value), box.size.x-int(value), text[int(value)].c_str(), NULL);
		nvgFill(vg);
	}
};

//===================================================================
// Checkbox class
//===================================================================
struct CabbageCheckbox : virtual Switch, FramebufferWidget {

	std::shared_ptr<Font> font;
	NVGcolor onColour, offColour, fontColour;
	string text;

	CabbageCheckbox()
	{
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		setText("");
	}

	void setColours(NVGcolor off, NVGcolor on, NVGcolor fontCol)
	{
		onColour = on;
		offColour = off;
		fontColour = fontCol;
	}

	void setText(string boxText)
	{
		text = boxText;
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
			nvgFillColor(vg, offColour);
		else
			nvgFillColor(vg, onColour);
		nvgFill(vg);

		nvgBeginPath(vg);
		nvgFontSize(vg, box.size.y);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, fontColour);
		nvgText(vg, box.size.y+4, box.size.y-2, text.c_str(), NULL);
		nvgFill(vg);
	}
};

//===================================================================
// Groupbox class
//===================================================================
struct CabbageGroupbox : Widget 
{
	NVGcolor colour, fontColour, outlineColour;
	std::shared_ptr<Font> font;
	string text;

 	CabbageGroupbox(Vec pos, Vec size, string groupText) 
	{
        box.size = size;
		box.pos = pos;
		text = groupText;
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
    }

	void setColours(NVGcolor col, NVGcolor outline, NVGcolor fontCol)       
	{
		colour = col;
		fontColour = fontCol;
		outlineColour = outline;
	}	

    void draw(NVGcontext *vg) override 
    {
		Widget::draw(vg);
		nvgBeginPath(vg);
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 4);
        nvgStrokeWidth(vg, 1);
		nvgStrokeColor(vg, outlineColour);
		nvgStroke(vg);
		nvgFillColor(vg, colour);
		nvgFill(vg);
		nvgClosePath(vg);
		nvgBeginPath(vg);
		nvgMoveTo(vg, 10, 20);
		nvgStrokeWidth(vg, 1);
		nvgStrokeColor(vg, colour);
		nvgLineTo(vg, box.size.x-10, 20);
		nvgStroke(vg);
		nvgClosePath(vg);
		nvgBeginPath(vg);
		nvgFontSize(vg, 12);
		nvgFontFaceId(vg, font->handle);
		nvgTextAlign(vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgFillColor(vg,fontColour);
		nvgTextBox(vg, 0, 5, box.size.x, text.c_str(), NULL);
		nvgClosePath(vg);
	}

};

//===================================================================
// Label class
//===================================================================
struct CabbageLabel : Widget 
{
	std::shared_ptr<Font> font;
	NVGcolor labelColour, backgroundColour;
	string text;
	int height = 14;

 	CabbageLabel(Vec pos, Vec size, string labelText) 
	{
		box.pos = pos;
		box.size = size;

		height = size.y;
		text = labelText;
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
	}

    void setColours(NVGcolor colour, NVGcolor fontColour)
    {
        backgroundColour = colour;
		labelColour = fontColour;
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
		nvgTextBox(vg, 0, 3, box.size.x, text.c_str(), NULL);
		
	}

};

//===================================================================
// Input / Output port
//===================================================================
struct CabbagePort : Port, FramebufferWidget 
{
	std::shared_ptr<Font> font;
	string text;
	CabbagePort()
	{
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		box.size.x = 30;
		box.size.y = 40;
	};

	void setText(string portText)
	{
		text = portText;
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
		nvgTextBox(vg, 0, 6, box.size.x, text.c_str(), NULL);
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