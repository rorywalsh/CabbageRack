<Cabbage>
form size(160, 380), colour(40, 40, 40)
groupbox bounds(22, 8, 119, 232) text("Tones") colour(30, 30, 30, 255) fontcolour:0(255, 255, 255, 255) fontcolour:1(255, 255, 255, 255) 
image bounds(22, 248, 118, 49) corners(25) 
rslider bounds(52, 44, 60, 60) channel("freq1") range(0, 1000, 500, 1, 0.001) text("Freq. 1") 
rslider bounds(52, 108, 60, 60) channel("freq2") range(0, 1000, 120, 1, 0.001) text("Freq. 2") 
rslider bounds(52, 170, 60, 60) channel("freq3") range(0, 1000, 120, 1, 0.001) text("Freq. 2") 
checkbox bounds(24, 302, 59, 22) channel("mute") text("Mute") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps   = 32
nchnls  = 2     
0dbfs   = 1   

;checkbox bounds(52, 116, 60, 14)  channel("mute") value(1), text("Mute") colour:0(25, 25, 25, 255) colour:1(147, 210, 0, 255) fontcolour:0(20, 20, 20, 255) fontcolour:1(20, 20, 20, 255) 
;
 

instr   1   
a1 oscili 1*1-chnget:k("mute"), chnget:k("freq1")
a2 oscili 1*1-chnget:k("mute"), chnget:k("freq2")
a3 oscili 1*1-chnget:k("mute"), chnget:k("freq3")
outs (a1+a3)/2, (a2+a3)/2
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
