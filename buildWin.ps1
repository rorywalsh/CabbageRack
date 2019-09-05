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
cd D:/a/1/

dir
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Syu"
C:\tools\msys64\usr\bin\bash.exe -l -c "pacman -Syu git wget make tar unzip zip mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-cmake autoconf automake mingw-w64-x86_64-libtool mingw-w64-x86_64-jq python"
