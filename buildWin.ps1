dir  
cd c:/
Invoke-WebRequest -Uri "https://github.com/rorywalsh/cabbage/releases/download/v2.0.00/csound-windows_x64-6.13.0.zip" -OutFile "C:\csound-windows_x64-6.12.0.zip" 
7z.exe x csound-windows_x64-6.12.0.zip -o"C:/Program Files"
cd "C:/Program Files/Csound6_x64"
dir
choco install -y msys2 --params "/InstallDir:C:\tools\msys64"
Invoke-WebRequest -Uri "https://vcvrack.com/downloads/Rack-SDK-1.1.4.zip" -OutFile "D:/a/Rack-SDK-1.1.4.zip" 
cd D:/a
7z.exe x D:/a/Rack-SDK-1.1.4.zip -o"D:/a"
dir
Get-ChildItem -Path "D:/a/1/s/" | Copy-Item -Destination "D:/a/Rack-SDK/CabbageRack" -force -Recurse -Container
cd D:/a/Rack-SDK
dir
cd D:/a/Rack-SDK/CabbageRack

C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Syu --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy git --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy wget --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy make --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy tar --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy unzip --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy mingw-w64-x86_64-gcc --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy zip --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy mingw-w64-x86_64-gdb --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy mingw-w64-x86_64-cmake --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy autoconf --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy automake --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy mingw-w64-x86_64-libtool --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Sy mingw-w64-x86_64-jq --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -lc "echo ++++++++++++++++++++++"
C:\tools\msys64\usr\bin\bash.exe -lc "ls"
C:\tools\msys64\msys2_shell.cmd -defterm -mingw64 -no-start -full-path -here -c "make -f Makefile.win"
Copy-Item "D:/a/Rack-SDK/CabbageRack/dist/CabbageRack-1.0.0-win.zip" -Destination "$BUILD_ARTIFACTSTAGINGDIRECTORY/CabbageRack-1.0.0-mac.zip" -Recurse 



