#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

static string createRGBAString(int r, int g, int b, int a=255)
{   
    int rgba = (r<<16) | (g<<8) | b;
    std::stringstream stream;
    stream << std::hex << rgba;
    std::string result( stream.str() );
    std::stringstream output;
    output << "fill=\"#" << result << "\" fill-opacity=\"" << float(a)/255.f << "\"";
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
        // <circle cx="20" cy="20" r="18" stroke="black" stroke-width="1" fill="#ffffff" fill-opacity="0" />
        // <circle cx="20" cy="20" r="12.4" fill="#0" fill-opacity="1" />
        // <rect width="10" height="11.6" 	x="15" 	y="0" fill="#ffffff" fill-opacity="1" />
        // <rect width="8" 	height="12" 	x="16" 		y="0" fill="#93d200" fill-opacity="1" />

    std::ostringstream svgText;
    int innerSize = width*.9f;
    svgText << "<svg height=\"" << width << "\" width=\"" << height << "\">\n";
    svgText << "  <circle cx=\"" << width/2 << "\" cy=\"" << width/2 << "\" r=\"" << innerSize/2 << "\" " << outline << " />\n";
    svgText << "  <circle cx=\"" << width/2 << "\" cy=\"" << width/2 << "\" r=\"" << (innerSize/2) * float(4.f/5.f) << "\" " << colour << " />\n";
    svgText << "  <rect width=\"" << width*.24 << "\" height=\"" << height*.5 << "\" x=\"" << width*.38 << "\" y=\"" << 0 << "\" " << outline << " />\n";
    svgText << "  <rect width=\"" << width*.20 << "\" height=\"" << height*.5 << "\" x=\"" << width*.40 << "\" y=\"" << 0 << "\" " << tracker << " />\n";
    svgText << "</svg> ";
    cout << "==================\n" << svgText.str() << "\n=======================";

	ofstream imgFile;
  	imgFile.open (path + "/res/rslider.svg");
	imgFile << svgText.str();
	imgFile.close(); 
    
}
