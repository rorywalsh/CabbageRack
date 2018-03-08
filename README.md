# Cabbage for VCV RACK 
(work in progress - not ready for public consumption!)

![sample screenshot](https://github.com/rorywalsh/CabbageRack/blob/master/example.gif)

## Building 

To build, follow the instructions [here](https://vcvrack.com/manual/PluginDevelopmentTutorial.html)(for v0.5) to compile and build the template plugin. Then cd to `Rack/plugins/` and run 

```
git clone https://github.com/rorywalsh/CabbageRack
cd CabbageRack
```

Then call the appropriate makefile for your platform by using the -f filename command line option to make:
```
make -f Makefile.win
```

> just call ```make``` on its own to build for Linux. 

The build script uses the locations for the Csound headers and libraries. if you install to custom location, update the makefiles.  

## Running
CabbageRack comes with a simple example file. In order to create an new module simply copy the CabbageRack folder and rename it. The name you choose will be the title of your new module. The .csd file contained within the folder must have the same name as the folder. For example, if the module is called CabbageSoup, the .csd file contained within must be call CabbageSoup.csd.  

#### Notes:
- Unlike the plugin interfaces for Cabbage, VCV Rack Modules don't use the `in` and `out` family of opcodes to access inputs and outputs. Instead, you should use `cvinput` and `cvoutput` widgets coupled with a set of `chnget` and `chnset` opcodes, see the sample .csd file for usage. 
- Currently only `rslider`, `label` , `groupbox`, `checkbox`, `button` and `image` widgets are supported, with reduced functionality. Only the most basic identifiers are supported `image` does support loading os SVG files using the `file()` identifier.  
- Slider only take min, max and value parameters, increment, and skew factor are not yet supported
- Modules are named by their folder, which should contain a .csd file of the same name, therefore, no whitespaces can be used. Use `_` or `-` instead.  
- Colours are `RGBA` only, there is no support for colour names. 
- This interface uses a much simpler parser than the one used in Cabbage plugins, spaces between identifiers and opening brackets will cause issues.  
- Widgets cannot be updated from Csound. 
