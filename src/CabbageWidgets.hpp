#include "plugin.hpp"


extern Plugin *cabbageRackPlugin;

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
        box.size = mm2px(Vec(control.width, 128.5));
        colour = control.colour;
    }
	
	void draw (const DrawArgs &args) override
    {
		nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
        nvgFillColor(args.vg, colour);
        nvgFill(args.vg);
	}

};

//===================================================================
// Rotary Slider class
//===================================================================
struct CabbageRotarySlider : virtual Knob, FramebufferWidget
{
	NVGcolor outline, inner, tracker, textColour;
	std::shared_ptr<Font> font;
	float minValue, maxValue;
	std::string text;
	int textHeight = 12;
	float currentValue = 0;

	CabbageRotarySlider(CabbageControl control, Module *mod, int id)
	{
		
		if (mod) 
		{
			paramQuantity = mod->paramQuantities[id];
		}
		
		// FramebufferWidget::box = math::Rect(math::Vec(10, 10), math::Vec(100, 100));y

		Knob::box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		Knob::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		FramebufferWidget::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		
		minValue = control.range[Range::min];
		maxValue = control.range[Range::max];
		
		
		if(control.range[Range::increment] >= 1)
			snap = true;

		
		// setDefaultValue(control.range[Range::value]);
		currentValue = dirtyValue;

		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
		outline = control.outlineColour;
		inner = control.colour;
		tracker = control.trackerColour;
		textColour = control.textColour;

		text = control.text[0];
		
		std::cout << "++++++++++++++++++++++++++++++\n";
		std::cout << text;
		std::cout << "++++++++++++++++++++++++++++++\n";
		
		FramebufferWidget::dirty = false;
	}

	void draw (const DrawArgs &args) override 
	{		
		const int centerx = FramebufferWidget::box.size.x/2.65f + FramebufferWidget::box.size.x/7.5f;
		const int centery = FramebufferWidget::box.size.y/2.65f;
		const int innerSize = FramebufferWidget::box.size.x;
		const int radius = innerSize/2;
		const float sliderValue = (dirtyValue+abs(min))/(abs(maxValue));
		// const int x = radius * cos((NVG_PI* 1.67) * sliderValue);  
		// const int y = radius * sin((NVG_PI* 1.67) * sliderValue);
		const float angle = (NVG_PI / 1.5) + (NVG_PI* 1.67) * sliderValue;
		const float x = (radius-radius*.15)*.5*cos( (NVG_PI / 1.5) + (NVG_PI* 1.67) * sliderValue) + centerx;
		const float y = (radius-radius*.15)*.5*sin( (NVG_PI / 1.5) + (NVG_PI* 1.67) * sliderValue) + centery;

		//tracker background
		nvgBeginPath(args.vg);
		nvgMoveTo(args.vg, centerx, centery);
		nvgArc(args.vg, centerx, centery, radius, (NVG_PI / 1.5), (NVG_PI / 1.5) + (NVG_PI* 1.67), NVG_CW);
		nvgLineTo(args.vg, centerx, centery);
		nvgFillColor(args.vg, nvgRGBA(0,0,0, 255));
		nvgFill(args.vg);


		
		//pie segment
		nvgBeginPath(args.vg);
		nvgMoveTo(args.vg, centerx, centery);
		nvgArc(args.vg, centerx, centery, radius, (NVG_PI / 1.5), (NVG_PI / 1.5) + ((NVG_PI* 1.67) *sliderValue), NVG_CW);
		nvgLineTo(args.vg, centerx, centery);
		nvgFillColor(args.vg, tracker);
		nvgFill(args.vg);
		//outer circle
		nvgBeginPath(args.vg);					
		// nvgCircle(args.vg, centerx, centery, radius);
		nvgMoveTo(args.vg, centerx, centery);
		nvgArc(args.vg, centerx, centery, radius, (NVG_PI / 1.5), (NVG_PI / 1.5) + (NVG_PI* 1.67), NVG_CW);
		nvgLineTo(args.vg, centerx, centery);
		nvgStrokeWidth(args.vg, 1);
		nvgStrokeColor(args.vg, outline);
		nvgStroke(args.vg);
		//innerCircle
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, centerx, centery, radius * .6f);
		nvgFillColor(args.vg, outline);
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, centerx, centery, (radius * .6f)-1);
		nvgFillColor(args.vg, inner);
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		//text
		nvgBeginPath(args.vg);
		nvgFontSize(args.vg, textHeight);
		nvgFontFaceId(args.vg, font->handle);
		
		nvgTextLetterSpacing(args.vg, -2);
		nvgFillColor(args.vg, textColour);
		nvgTextAlign(args.vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgTextBox(args.vg, 0, FramebufferWidget::box.size.y-20, FramebufferWidget::box.size.x, text.c_str(), NULL);
		nvgFill(args.vg);

		
		
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, x, y, radius*.15);
		// nvgRoundedRect(args.vg, x, y, 10, 10, 1);
		nvgRotate(args.vg, angle);
		nvgFillColor(args.vg, nvgRGBA(80, 80, 80, 255));
		nvgFill(args.vg);
		nvgClosePath(args.vg);

	}
};

//===================================================================
// Button class
//===================================================================
struct CabbageButton : virtual Switch, FramebufferWidget {

	std::shared_ptr<Font> font;
	NVGcolor onColour, offColour, fontColour;
	std::string text[2];

	CabbageButton(CabbageControl control, Module *mod, int id)
	{
		
		if (mod) 
			paramQuantity = mod->paramQuantities[id];



		Switch::box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		Switch::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		FramebufferWidget::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);

		onColour = control.colour1;
		offColour = control.colour0;
		fontColour = control.fontColour;

		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
		text[0] = control.text[0];
		text[1] = control.text[1];
		
	}

	void draw (const DrawArgs &args) override 
	{
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0, 0, FramebufferWidget::box.size.x-int(value), FramebufferWidget::box.size.y-int(value), 4);
		nvgFillColor(args.vg, nvgRGBA(80, 80, 80, 255));
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 1, 1, FramebufferWidget::box.size.x-2-int(value), FramebufferWidget::box.size.y-1-int(value), 4);
		nvgFillColor(args.vg, nvgRGBA(170, 170, 170, 255));
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 2, 2, FramebufferWidget::box.size.x-3-int(value), FramebufferWidget::box.size.y-2-int(value), 4);
		if (dirtyValue==0)
			nvgFillColor(args.vg, offColour);
		else
			nvgFillColor(args.vg, onColour);
		nvgFill(args.vg);

		nvgBeginPath(args.vg);
		nvgFontSize(args.vg, 11);//box.size.y*(numChars/box.size.y));
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgFillColor(args.vg, fontColour);
		nvgTextAlign(args.vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgTextBox(args.vg, 0, (FramebufferWidget::box.size.y/2)-(11.f/2.f)-int(value), FramebufferWidget::box.size.x-int(dirtyValue), text[int(dirtyValue)].c_str(), NULL);
		nvgFill(args.vg);
	}
};

//===================================================================
// Combobox class
//===================================================================
struct CabbageCombobox : virtual Switch {

	std::shared_ptr<Font> font;
	NVGcolor colour, fontColour;
	std::vector<std::string> text;
	int max, min, value;

	CabbageCombobox(CabbageControl control, Module *mod, int id)
	{
		if (mod) 
			paramQuantity = mod->paramQuantities[id];


		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		// setLimits(0.f, control.text.size());
		max = control.text.size();
		min = 1;
		dirtyValue = control.value;
		colour = control.colour;
		fontColour = control.fontColour;
		font = font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
		text = control.text;
	}


	void draw (const DrawArgs &args) override 
	{
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 4);
		nvgFillColor(args.vg, nvgRGBA(80, 80, 80, 255));
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 1, 1, box.size.x-2, box.size.y-1, 4);
		nvgFillColor(args.vg, nvgRGBA(170, 170, 170, 255));
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 2, 2, box.size.x-3, box.size.y-2, 4);
		nvgFillColor(args.vg, colour);
		nvgFill(args.vg);
		nvgBeginPath(args.vg);
		nvgFontSize(args.vg, 13);//box.size.y*(numChars/box.size.y));
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgFillColor(args.vg, fontColour);
		nvgTextAlign(args.vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgTextBox(args.vg, 0, (box.size.y/2)-(11.f/2.f), box.size.x, text[(int(dirtyValue-1) < 0 ? 0 : int(dirtyValue-1))].c_str(), NULL);
		nvgFill(args.vg);
	}
};

//===================================================================
// Checkbox class
//===================================================================
struct CabbageCheckbox : virtual Switch, FramebufferWidget {

	std::shared_ptr<Font> font;
	NVGcolor onColour, offColour, fontColour;
	std::string text;

	CabbageCheckbox(CabbageControl control, Module *mod, int id)
	{
		if (mod) 
		{
			paramQuantity = mod->paramQuantities[id];
		}

		Switch::box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);

		//rescaling here to keep things in line with Cabbage look and feel for checkboxes
		Switch::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		
		FramebufferWidget::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);

		
		onColour = control.colour1;
		offColour = control.colour0;
		fontColour = control.fontColour;
		font = font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
		text = control.text[0];
	}


	void draw (const DrawArgs &args) override 
	{
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0, 0, FramebufferWidget::box.size.y, FramebufferWidget::box.size.y, 2);
		nvgFillColor(args.vg, nvgRGBA(20, 20, 20, 255));
		nvgRoundedRect(args.vg, 1, 1, FramebufferWidget::box.size.y-1, FramebufferWidget::box.size.y-1, 2);
		nvgFillColor(args.vg, nvgRGBA(80, 80, 80, 255));

		nvgRoundedRect(args.vg, 2, 2, FramebufferWidget::box.size.y-4, FramebufferWidget::box.size.y-4, 2);
		if (dirtyValue==0)
			nvgFillColor(args.vg, offColour);
		else
			nvgFillColor(args.vg, onColour);
		nvgFill(args.vg);

		nvgBeginPath(args.vg);
		nvgFontSize(args.vg, FramebufferWidget::box.size.y*.8f);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgFillColor(args.vg, fontColour);
		nvgText(args.vg, FramebufferWidget::box.size.y+4, FramebufferWidget::box.size.y-2, text.c_str(), NULL);
		nvgFill(args.vg);
	}
};

//===================================================================
// Groupbox class
//===================================================================
struct CabbageGroupbox : FramebufferWidget 
{
	NVGcolor colour, fontColour, outlineColour;
	std::shared_ptr<Font> font;
	std::string text;

 	CabbageGroupbox(CabbageControl control) 
	{
        box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		text = control.text[0];
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
		colour = control.colour;
		fontColour = control.fontColour;
		outlineColour = control.outlineColour;
    }

    void draw (const DrawArgs &args) override 
    {
		nvgBeginPath(args.vg);
        nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 4);
        nvgStrokeWidth(args.vg, 1);
		nvgStrokeColor(args.vg, outlineColour);
		nvgStroke(args.vg);
		nvgFillColor(args.vg, colour);
		nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgMoveTo(args.vg, 10, 20);
		nvgStrokeWidth(args.vg, 1);
		nvgStrokeColor(args.vg, colour);
		nvgLineTo(args.vg, box.size.x-10, 20);
		nvgStroke(args.vg);
		nvgClosePath(args.vg);
		nvgBeginPath(args.vg);
		nvgFontSize(args.vg, 12);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextAlign(args.vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgFillColor(args.vg,fontColour);
		nvgTextBox(args.vg, 0, 5, box.size.x, text.c_str(), NULL);
		nvgClosePath(args.vg);
	}

};

//===================================================================
// Label class
//===================================================================
struct CabbageLabel : FramebufferWidget 
{
	std::shared_ptr<Font> font;
	NVGcolor labelColour, backgroundColour;
	std::string text;
	int height = 14;

 	CabbageLabel(CabbageControl control) 
	{
        box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		text = control.text[0];
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
		backgroundColour = control.colour;
		labelColour = control.fontColour;
	}

	void draw (const DrawArgs &args) override 
	{
		nvgBeginPath(args.vg);
        nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 3);
        nvgFillColor(args.vg, backgroundColour);
        nvgFill(args.vg);
		nvgClosePath(args.vg);
		nvgFontSize(args.vg, height*.9);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextLetterSpacing(args.vg, -2);
		nvgTextAlign(args.vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
		nvgFillColor(args.vg,labelColour);
		nvgTextBox(args.vg, 0, 3, box.size.x, text.c_str(), NULL);
		
	}
};

//===================================================================
// Image class
//===================================================================
struct CabbageImage : FramebufferWidget 
{
	NVGcolor outlineColour, backgroundColour;
	std::string text;
	NSVGimage* svgImage;
	int height = 14, corners = 3;
	std::string svgFile;

 	CabbageImage(CabbageControl control) 
	{
        box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		svgFile = control.file;
		corners = control.corners;
		backgroundColour = control.colour;
		outlineColour = control.outlineColour;
		svgImage = nsvgParseFromFile("/home/rory/sourcecode/Rack-SDK/CabbageRack/res/choice.svg", "px", 96.0f);
		std::cout << control.file.c_str();

	}

	void draw (const DrawArgs &args) override 
	{
		
		if(svgImage)//if users specifies valid svg file draw...
		{
			drawSVG(args.vg, svgImage);
		}
		else
		{
			nvgBeginPath(args.vg);
    		nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, corners);
        	nvgFillColor(args.vg, backgroundColour);
        	nvgFill(args.vg);	
		}	
	}
};

// struct CabbageLight : MultiLightWidget 
// {
// 	int corners = corners;


// 	CabbageLight(CabbageControl control, Module* mod, int lightId) 
// 	{
// 		box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
// 		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
//		addBaseColor(control.colour);
// 		addBaseColor(nvgRGBA(0, 225, 0, 255));
// 		// module = mod;
// 		// firstLightId = lightId;
// 		corners = control.corners;
// 	}
// };

struct CabbageLight : ModuleLightWidget 
{
	int corners = corners;
	NVGcolor outlineColour, backgroundColour;

	CabbageLight(CabbageControl control, Module* mod, int lightId) 
	{
		box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
		box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
		addBaseColor(control.colour);
		color =  nvgRGBA(0, 255, 0, 100);
		backgroundColour = control.colour;
		module = mod;
		firstLightId = lightId;
		corners = control.corners;
	}

	void draw(const DrawArgs& args) override
	{
		drawLight(args);
		drawHalo(args);
	}

	void drawLight(const DrawArgs& args) override
	{
	float radius = box.size.x / 2.0;

		nvgBeginPath(args.vg);
		nvgCircle(args.vg, radius, radius, radius);
		nvgFillColor(args.vg, nvgRGB(0,0,0));
		nvgFill(args.vg);

		// Background
		if (bgColor.a > 0.0) {
			nvgFillColor(args.vg, bgColor);
			nvgFill(args.vg);
		}

		// Foreground
		if (color.a > 0.0) {
			nvgFillColor(args.vg, color);
			nvgFill(args.vg);
		}

		// // Border
		// if (borderColor.a > 0.0) {
		 	nvgStrokeWidth(args.vg, 1);
		 	nvgStrokeColor(args.vg, nvgRGBA(0, 0, 0, 255));
		 	nvgStroke(args.vg);
		// }
	}

	void drawHalo(const DrawArgs& args) override
	{
		float radius = box.size.x / 2.0;
		float oradius = 4.0 * radius;

		nvgBeginPath(args.vg);
		nvgRect(args.vg, radius - oradius, radius - oradius, 2 * oradius, 2 * oradius);

		NVGpaint paint;
		NVGcolor icol = color::mult(color, 0.07);
		NVGcolor ocol = nvgRGB(0, 0, 0);
		paint = nvgRadialGradient(args.vg, radius, radius, radius, oradius, icol, ocol);
		nvgFillPaint(args.vg, paint);
		nvgGlobalCompositeOperation(args.vg, NVG_LIGHTER);
		nvgFill(args.vg);
	}
};
//===================================================================
// Can't get this one to work with API 1.0... see below for current implementation
//===================================================================
// struct CabbagePort : PortWidget, FramebufferWidget 
// {
// 	std::shared_ptr<Font> font;
// 	std::string text;  
// 	NVGcolor outlineColour, colour, fontcolour;

// 	CabbagePort(CabbageControl control,  Module *mod, int id)
// 	{
// 		PortWidget::box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);
//         FramebufferWidget::box.size = Vec(control.bounds[Bounds::width], control.bounds[Bounds::height]);
// 		PortWidget::box.pos = Vec(control.bounds[Bounds::x], control.bounds[Bounds::y]);;
		
// 		if(mod)
// 			module = mod;

// 		portId = id;
// 		PortWidget::type = control.type == "cvinput" ? PortWidget::INPUT : PortWidget::OUTPUT;
// 		outlineColour = control.outlineColour;
// 		fontcolour = control.fontColour;
// 		colour = control.colour;
// 		text = control.text[0];
// 		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/Vera-Bold.ttf"));
// 	};

// 	void draw(const DrawArgs& args) override
// 	{
// 		nvgBeginPath(args.vg);
// 		nvgFontSize(args.vg, 10);
// 		nvgFontFaceId(args.vg, font->handle);
// 		nvgTextLetterSpacing(args.vg, -2);
// 		nvgFillColor(args.vg, fontcolour);
// 		nvgTextAlign(args.vg, NVG_ALIGN_CENTER);
// 		nvgTextBox(args.vg, 0, FramebufferWidget::box.size.y+10, FramebufferWidget::box.size.x, text.c_str(), NULL);
// 		nvgBeginPath(args.vg);
//         nvgCircle(args.vg, FramebufferWidget::box.size.x/2, FramebufferWidget::box.size.y/2, 10);
//         nvgFillColor(args.vg, nvgRGBA(80, 80, 80, 255));
// 		nvgFill(args.vg);
// 		nvgBeginPath(args.vg);
//         nvgCircle(args.vg,FramebufferWidget::box.size.x/2, FramebufferWidget::box.size.y/2, 8);
//         nvgFillColor(args.vg, outlineColour);
// 		nvgFill(args.vg);
// 		nvgBeginPath(args.vg);
//         nvgCircle(args.vg, FramebufferWidget::box.size.x/2, FramebufferWidget::box.size.y/2, 7);
//         nvgFillColor(args.vg, colour);
//         nvgFill(args.vg);
// 	}
// };

struct CabbagePort : app::SvgPort {
	CabbagePort() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Port.svg")));
	}
};

struct CabbageScrew : app::SvgScrew {
	CabbageScrew() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Screw.svg")));
	}
};