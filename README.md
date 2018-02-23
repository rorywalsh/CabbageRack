#Cabbage for VCV RACK 
(work in progress - not ready for public consumption!)


##Building 

To build, follow the instructions [here](https://vcvrack.com/manual/PluginDevelopmentTutorial.html) to compile and build the template plugin. Then cd to `Rack/plugins/` and run 

```
git clone https://github.com/rorywalsh/CabbageRack
cd CabbageRack
./build.sh
```

The build script uses the locations for the Csound headers and libraries. if you install to custom location, update the makefiles.  

##Running
CabbageRack comes with a simple example file. In order to create an new module simply copy the CabbageRack folder and rename it. The name you choose will be the title of your new module. The .csd file contained within the folder must have the same name as the folder. For example, if the module is called CabbageSoup, the .csd file contained within must be call CabbageSoup.csd.  

####Notes:
- Modules have two inputs and two outputs. All Csound instrument must use `nchnls=2`.  
- Currently only 'rsliders' and 'labels' are supported.
- Slider only take min, max and value parameters, increment, and skew factor are not yet supported
- Modules are named by their folder, which should contain a .csd file of the same name, therefore, no whitespaces can be used. Use `_` or `-` instead.  
- Colours are `RGBA` only, there is no support for colour names. 