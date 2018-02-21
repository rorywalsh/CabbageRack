#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

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
//   <circle cx="50" cy="50" r="45" stroke-width="1" stroke="#000000" fill-opacity="0" />
//   <circle cx="50" cy="50" r="36" fill="#000000" />
//   <rect width="16" height="17" x="42" y="1" fill="#000000" />
//   <rect width="14" height="15" x="43" y="2" fill="#93d200" />


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
