<Cabbage>
form size(150, 380), colour(90, 90,90)
groupbox bounds(16, 28, 119, 216) text("De-tuners") colour(30, 30, 30, 255)  255, 255, 255, 255)  255, 255, 255, 255)
rslider bounds(46, 48, 60, 60) channel("detune1") range(-1, 1, 0, 1, 0.001) text("Detune 1") 
rslider bounds(46, 112, 60, 60) channel("detune2") range(-1, 1, 0, 1, 0.001) text("Detune 2")
rslider bounds(46, 174, 60, 60) channel("detune3") range(-1, 1, 0, 1, 0.001) text("Detune 3")
cvinput bounds(24, 280, 40, 40) channel("amp") text("Amp-In") 
cvinput bounds(90, 280, 40, 40) channel("freq") text("Pitch-In") 
cvoutput bounds(24, 320, 40, 40) channel("out1") text("Out-1") 
cvoutput bounds(90, 320, 40, 40) channel("out2") text("Out-2") 
 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps   = 16
nchnls  = 2     
0dbfs   = 1   
 
instr   1   
aInAmp chnget "amp"
aInFreq chnget "freq"
 
k1 downsamp aInFreq
kFreq = 261*pow(2.0, k1*10)
 
kAmp downsamp aInAmp
 
if kAmp > 0 then
    a1 oscil kAmp, kFreq+kFreq*chnget:k("detune1")
    a2 oscil kAmp, kFreq+kFreq*chnget:k("detune2")
    a3 oscil kAmp, kFreq+kFreq*chnget:k("detune3")
 
    chnset (a1+a2+a3)/3, "out1"
    chnset (a1+a2+a3)/3, "out2"
else
    chnset a(0), "out1"
    chnset a(0), "out2"
 
endif
 
endin
 
</CsInstruments>
<CsScore>
f1 0 4096 10 1
i1 0 z
</CsScore>
</CsoundSynthesizer>
