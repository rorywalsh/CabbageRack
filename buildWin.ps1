dir  
# cd c:/
# Invoke-WebRequest -Uri "https://github.com/rorywalsh/cabbage/releases/download/v2.0.00/csound-windows_x64-6.13.0.zip" -OutFile "C:\csound-windows_x64-6.12.0.zip" 
# 7z.exe x csound-windows_x64-6.12.0.zip -o"C:/Program Files"
# cd "C:/Program Files/Csound6_x64"
# dir

Invoke-WebRequest -Uri "https://vcvrack.com/downloads/Rack-SDK-1.1.4.zip" -OutFile "D:/a/Rack-SDK-1.1.4.zip" 
cd D:/a
7z.exe x D:/a/Rack-SDK-1.1.4.zip -o"D:/a"
dir
cd D:/a/1/

dir

$env:Path += ";C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin"
