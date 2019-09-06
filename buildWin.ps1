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

C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Syu --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy git --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy wget --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy make --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy tar --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy unzip --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-gcc --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy zip --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-gdb --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-cmake --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy autoconf --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy automake --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-libtool --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy mingw-w64-x86_64-jq --noconfirm"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Sy python --noconfirm"

cd D:/a/Rack-SDK/CabbageRack
dir
C:\tools\msys64\usr\bin\bash.exe -l -c "make -f Makefile.win"



