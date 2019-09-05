cd ~/sourcecode/Rack-SDK/CabbageRack
make clean
make -j4
make install
cp CabbageRack.csd ~/.Rack/plugins-v1/CabbageRack/CabbageRack.csd
cd ~/sourcecode/Rack
./Rack
# if [ $? -eq 0 ] ; then
#     cd ~/sourcecode/Rack
#     make run
# fin