#pragma once
#include <rack.hpp>
#include "CabbageParser.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include "csound.hpp"

using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin *pluginInstance;

// Declare each Model, defined in each module source file
extern Model *modelCabbageRack;