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
#include "CabbageParser.hpp"

using namespace std;

//========================================================
// Utility functions for drawing SVGs
//========================================================
static NVGcolor getNVGColor(uint32_t color) {
	return nvgRGBA(
		(color >> 0) & 0xff,
		(color >> 8) & 0xff,
		(color >> 16) & 0xff,
		(color >> 24) & 0xff);
}

static void drawSVG(NVGcontext* vg, NSVGimage* svgImage)
{
	NSVGshape * shape;
	NSVGpath * path;
	int i;
	nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
	for (shape = svgImage->shapes; shape != NULL; shape = shape->next) {

		if (!(shape->flags & NSVG_FLAGS_VISIBLE))
			continue;

		nvgFillColor(vg, getNVGColor(shape->fill.color));
		nvgStrokeColor(vg, getNVGColor(shape->stroke.color));
		nvgStrokeWidth(vg, shape->strokeWidth);

		for (path = shape->paths; path != NULL; path = path->next) {
			nvgBeginPath(vg);
			nvgMoveTo(vg, path->pts[0], path->pts[1]);
			for (i = 0; i < path->npts-1; i += 3) {
				float* p = &path->pts[i*2];
				nvgBezierTo(vg, p[2], p[3], p[4], p[5], p[6], p[7]);
			}
			if (path->closed)
				nvgLineTo(vg, path->pts[0], path->pts[1]);

			if(shape->fill.type)
				nvgFill(vg);

			if(shape->stroke.type)
				nvgStroke(vg);
		}
	}
}

//===================================================================
// Form class
//===================================================================
struct CabbageForm : Widget 
{
	NVGcolor colour;

 	CabbageForm(CabbageControl control) 
	{
        box.size = Vec(control.width, control.height);
        colour = control.colour;
    }
	
    void draw(NVGcontext *vg) override 
    {
		Widget::draw(vg);
		nvgBeginPath(vg);
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
        nvgFillColor(vg, colour);
        nvgFill(vg);
	}

};

//===================================================================
// Rotary Slider class
//===================================================================
struct CabbageRotarySlider : virtual Knob, FramebufferWidget
{
	NVGcolor outline, inner, tracker, textColour;
	std::shared_ptr<Font> font;
	string text;
	int textHeight = 12;

	CabbageRotarySlider(CabbageControl control, Module *mod, int id)
	{
		paramId = id;
		module = mod;
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		setLimits(control.range[Range::min], control.range[Range::max]);
		setDefaultValue(control.range[Range::value]);

		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		outline = control.outlineColour;
		inner = control.colour;
		tracker = control.trackerColour;
		textColour = control.textColour;
		text = control.text[0];

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
		nvgFillColor(vg, outline);
		nvgFill(vg);
		nvgClosePath(vg);
		nvgBeginPath(vg);
		nvgCircle(vg, centerx, centery, (radius * .6f)-1);
		nvgFillColor(vg, inner);
		nvgFill(vg);
		nvgClosePath(vg);
		//text
		nvgBeginPath(vg);
		nvgFontSize(vg, textHeight);
		nvgFontFaceId(vg, font->handle);
		
		nvgTextLetterSpacing(vg, -2);
		nvgFillColor(vg, textColour);
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

	CabbageButton(CabbageControl control, Module *mod, int id)
	{
		paramId = id;
		module = mod;
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		setLimits(0.f, 1.f);
		setDefaultValue(control.value);
	
		onColour = control.colour1;
		offColour = control.colour0;
		fontColour = control.fontColour;

		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		text[0] = control.text[0];
		text[1] = control.text[1];
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

	CabbageCheckbox(CabbageControl control, Module *mod, int id)
	{
		paramId = id;
		module = mod;
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		//rescaling here to keep things in line with Cabbage look and feel for checkboxes
		box.size = Vec(control.bounds[Bounds::width]*.7f, control.bounds[Bounds::height]*.7f);
		setLimits(0.f, 1.f);
		setDefaultValue(control.value);
		
		onColour = control.colour1;
		offColour = control.colour0;
		fontColour = control.fontColour;
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		text = control.text[0];
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
		nvgFontSize(vg, box.size.y*.8f);
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

 	CabbageGroupbox(CabbageControl control) 
	{
        box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		text = control.text[0];
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		colour = control.colour;
		fontColour = control.fontColour;
		outlineColour = control.outlineColour;
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

 	CabbageLabel(CabbageControl control) 
	{
        box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		text = control.text[0];
		font = Font::load(assetPlugin(plugin, "res/Vera-Bold.ttf"));
		backgroundColour = control.colour;
		labelColour = control.fontColour;
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
// Image class
//===================================================================
struct CabbageImage : Widget 
{
	NVGcolor outlineColour, backgroundColour;
	string text;
	int height = 14, corners = 3;
	string svgFile;

 	CabbageImage(CabbageControl control) 
	{
        box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		svgFile = control.file;
		corners = control.corners;
		backgroundColour = control.colour;
		outlineColour = control.outlineColour;

	}

	void draw(NVGcontext *vg) override 
	{
		NSVGimage* svgImage = nsvgParseFromFile(assetPlugin(plugin, svgFile).c_str(), "px", 96.0f);

		if(svgImage)//if users specifies valid svg file draw...
		{
			drawSVG(vg, svgImage);
		}
		else
		{
			Widget::draw(vg);
			nvgBeginPath(vg);
    		nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, corners);
        	nvgFillColor(vg, backgroundColour);
        	nvgFill(vg);	
		}	
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