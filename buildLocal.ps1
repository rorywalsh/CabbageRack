C:\tools\msys64\msys2_shell.cmd -defterm -mingw64 -no-start -full-path -here -c "make -f Makefile.win dist"
Copy-Item "C:/Users/rory/sourcecode/Rack-SDK/CabbageRack/CabbageRack.csd" -Destination "C:/Users/rory/sourcecode/Rack-SDK/CabbageRack/dist/CabbageRack/CabbageRack.csd"
Copy-Item "C:/Users/rory/sourcecode/Rack-SDK/CabbageRack/dist/CabbageRack" -Destination "C:/Users/rory/sourcecode/Rack/plugins/CabbageRack" -Recurse 
