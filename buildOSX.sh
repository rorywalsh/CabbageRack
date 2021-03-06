echo "AGENT_WORKFOLDER is $AGENT_WORKFOLDER"
echo "AGENT_WORKFOLDER contents:"
ls -1 $AGENT_WORKFOLDER
echo "AGENT_BUILDDIRECTORY is $AGENT_BUILDDIRECTORY"
echo "AGENT_BUILDDIRECTORY contents:"
ls -1 $AGENT_BUILDDIRECTORY
echo "SYSTEM_DEFAULTWORKINGDIRECTORY is $SYSTEM_DEFAULTWORKINGDIRECTORY"
echo "SYSTEM_DEFAULTWORKINGDIRECTORY contents:"
ls -1 $SYSTEM_DEFAULTWORKINGDIRECTORY
echo "BUILD_ARTIFACTSTAGINGDIRECTORY is $BUILD_ARTIFACTSTAGINGDIRECTORY"
echo "BUILD_ARTIFACTSTAGINGDIRECTORY contents:"
ls -1 $BUILD_ARTIFACTSTAGINGDIRECTORY

#install deps for VCV Rack development
brew install git wget cmake autoconf automake libtool jq python

#install Csound
pwd
curl -L -o csound6.13.0-MacOS_x86_64.dmg 'https://github.com/csound/csound/releases/download/6.13.0/Csound6.13-MacOS_x86_64.dmg'
ls
hdiutil attach csound6.13.0-MacOS_x86_64.dmg
cp -R /Volumes/Csound6.13/ Csound
hdiutil detach /Volumes/Csound6.13/
cd Csound
sudo installer -pkg csound6.13-MacOS_x86_64.pkg -target /

#return to main working dir after installing Csound
cd $SYSTEM_DEFAULTWORKINGDIRECTORY
cd ..

curl -L -o Rack-SDK-1.1.6.zip 'https://vcvrack.com/downloads/Rack-SDK-1.1.6.zip'
unzip -q Rack-SDK-1.1.6.zip
ls
cp -rf s Rack-SDK/CabbageRack
cd Rack-SDK
echo "---------------------------_"
ls
echo "---------------------------_"
cd CabbageRack
ls
echo "---------------------------_"

make -f Makefile.osx clean
make dist -f Makefile.osx
echo "--------------------------"
ls
echo "--------------------------"
ls dist
cp -rf dist/CabbageRack-1.0.0-mac.zip $BUILD_ARTIFACTSTAGINGDIRECTORY/CabbageRack-1.0.0-mac.zip
