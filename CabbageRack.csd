<Cabbage>
form size(160, 380), colour(40, 40, 40)
groupbox bounds(22, 8, 119, 279) text("Freqky") colour(100, 100, 100, 255) fontcolour:0(255, 255, 255, 255) fontcolour:1(255, 255, 255, 255) 
rslider bounds(52, 44, 60, 60) channel("freq1") range(0, 1000, 56, 1, 0.001) text("Freq. 1") 
rslider bounds(52, 140, 60, 60) channel("freq2")  range(0, 1000, 120, 1, 0.001) text("Freq. 2") 
checkbox bounds(52, 116, 60, 14)  channel("mute") text("Freq. 2") colour:0(25, 25, 25, 255) colour:1(147, 210, 0, 255) fontcolour:0(20, 20, 20, 255) fontcolour:1(20, 20, 20, 255) 

 
button bounds(44, 218, 40, 20) colour:0(30, 30, 30, 255), colour:1(147, 210, 0, 255), channel("buttonchan6") text("Push me", "Push me") 
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps   = 32
nchnls  = 2     
0dbfs   = 1   

instr   1   
a1 inch 1;
a2 inch 2;
a1 oscili 1, chnget:k("freq1")
a2 oscili 1, chnget:k("freq2")
;outs (a1*a2)/2, (a1*a2)/2
outs a1*chnget:k("mute"), a2*chnget:k("mute")
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
