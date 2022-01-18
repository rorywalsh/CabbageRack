# Cabbage for VCV RACK 


![sample screenshot](https://github.com/rorywalsh/CabbageRack/blob/master/example.gif)

## Building

This section outlines the steps necessary to build this project.

### Install VCV Rack SDK
First, download and extract the latest [Rack SDK](https://vcvrack.com/downloads/). 

### Compile and build template plugin

Once you have extracted the Rack SDK, follow the [instructions to compile and build the template plugin](https://vcvrack.com/manual/PluginDevelopmentTutorial#Creating-the-template-plugin). 

### Make CabbageRack within the Rack SDK

After you have built the template plugin, change directory (`cd`) in to the Rack SDK directory and run 

```
git clone https://github.com/rorywalsh/CabbageRack
cd CabbageRack
```

From within the `CabbageRack` directory, call `make` with the appropriate makefile for your platform by using the `-f <filename>` command line option. E.g. for Windows:

```
make -f Makefile.win
```

On Linux, you can politely call `make` without any arguments.

**Note:** The build script uses the locations for the Csound headers and libraries. If you install Csound to a custom location, update the makefiles.  

## Running

CabbageRack comes with a simple example file. In order to create an new module simply copy the CabbageRack folder and rename it. The name you choose will be the title of your new module. The .csd file contained within the folder must have the same name as the folder. For example, if the module is called CabbageSoup, the .csd file contained within must be call CabbageSoup.csd.  

#### IMPORTANT: Please read before this interface:
- Of the typical set of Cabbage widgets, only `rslider`, `label` , `groupbox`, `checkbox`, `button`, `combobox` and `image` widgets are supported, with reduced functionality (and the combobox is more like a switch than a drop-down combo). Only the most basic identifiers are supported, so please keep things simple and keep testing your modules to make sure you are not using unsupported identifiers.  
- CabbageRack also introduces 3 Rack specific widgets, `cvinput` and `cvoutput`, (see below) and `light`, which can be triggreed from a Csound instrument to give users visual feedback their modules.
- Unlike the other audio plugin interfaces for Cabbage, CabbageRack does not use the `in` and `out` family of opcodes to access inputs and outputs. They channels instead. Each `cvinput` and `cvoutput` widget is linked to a set of a-rate `chnget` and `chnset` opcodes, see the sample .csd file for usage. The reason for this is allow maximum configurability of inputs and outputs.  
- Slider skew values are not valid. The increment identifier can be set to a number equal or greater than one to allow snapping to integer value, but it will only snap to multiple of 1.
- Modules are named by their folder, which should contain a .csd file of the same name, therefore, no whitespaces can be used. Use `_` or `-` instead.  
- Colours are `RGBA` only, there is no support for CSS colour names. 
- This interface uses a much simpler parser than the one used in Cabbage plugins, spaces between identifiers and opening brackets will cause issues.  
- Only the `light` widget can be updated from Csound. 
- VCV Rack auto-saves sessions. If you make some changes to your Cabbage module and reload, VCV Rack might try to load parameters that are no longer valid. This might cause VCV Rack to crash. If so, you will need to edit or delete the auto-saved file.  
