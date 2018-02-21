#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

static string createRGBAString(int r, int g, int b, int a)
{   
    int rgba = (r<<16) | (g<<8) | b;
    std::stringstream stream;
    stream << std::hex << rgba;
    std::string result( stream.str() );
    std::stringstream output;
    output << "fill=#" << result << " fill-opacity=\"" << float(a)/255.f << "\"";
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
    svgText << "<svg height=\"" << width << "\" width=\"" << height << "\">\n";
    svgText << "  <circle cx=\"" << width/2 << "\" cy=\"" << width/2 << "\" r=\"" << width/2 << "\" " << outline << " />\n";
    svgText << "  <circle cx=\"" << width/2 << "\" cy=\"" << width/2 << "\" r=\"" << width/2 * int(4.f/5.f) << "\" " << colour << " />\n";
    svgText << "  <rect width=\"" << width*.24 << "\" height=\"" << height*.44 << "\" x=\"" << width*.38 << "\" y=\"" << width*.5 << "\" " << outline << " />\n";
    svgText << "  <rect width=\"" << width*.20 << "\" height=\"" << height*.40 << "\" x=\"" << width*.40 << "\" y=\"" << width*.7 << "\" " << tracker << " />\n";
    svgText << "</svg> ";
    cout << "==================\n" << svgText.str() << "\n=======================";

	ofstream imgFile;
  	imgFile.open (path + "/res/rslider.svg");
	imgFile << svgText.str();
	imgFile.close(); 
    
}
