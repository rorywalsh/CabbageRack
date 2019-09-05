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

brew install git wget cmake autoconf automake libtool jq python
cd ..

curl -L -o Rack-SDK-1.1.4.zip 'https://vcvrack.com/downloads/Rack-SDK-1.1.4.zip'
unzip -q Rack-SDK-1.1.4.zip
ls
cp -rf s Rack-SDK/CabbageRack
cd Rack-SDK
ls
cd CabbageRack
ls
make clean
make -f Makefile.osx
cp dist $BUILD_ARTIFACTSTAGINGDIRECTORY/OSXDist
