dir  
# cd c:/
# Invoke-WebRequest -Uri "https://github.com/rorywalsh/cabbage/releases/download/v2.0.00/csound-windows_x64-6.13.0.zip" -OutFile "C:\csound-windows_x64-6.12.0.zip" 
# 7z.exe x csound-windows_x64-6.12.0.zip -o"C:/Program Files"
# cd "C:/Program Files/Csound6_x64"
# dir
choco install -y msys2 --params "/InstallDir:C:\tools\msys64"
Invoke-WebRequest -Uri "https://vcvrack.com/downloads/Rack-SDK-1.1.4.zip" -OutFile "D:/a/Rack-SDK-1.1.4.zip" 
cd D:/a
7z.exe x D:/a/Rack-SDK-1.1.4.zip -o"D:/a"
dir
Get-ChildItem -Path "D:/a/1" | Copy-Item -Destination "D:/a/Rack-SDK/CabbageRack" -Recurse -Container
cd D:/a/Rack-SDK
dir

C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Syu"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy git"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy wget"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy make"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy tar"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy unzip"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-gcc"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy zip"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-gdb"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-cmake"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy autoconf"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy automake"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-libtool"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-jq"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy python"

cd D:/a/Rack-SDK/CabbageRack
C:\tools\msys64\usr\bin\bash.exe -l -c "make -f Makefile.win"



