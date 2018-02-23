<Cabbage>
form size(160, 380), colour(40, 40, 40)
rslider bounds(40, 42, 60, 60) channel("freq1") range(0, 1000, 56, 1, 0.001), text("Freq. 1"), colour(25, 25, 25), trackercolour(147, 210, 0), outlinecolour(20, 20, 20), 
rslider bounds(40, 140, 60, 60) channel("freq2") range(0, 1000, 120, 1, 0.001), text("Freq. 2"), colour(25, 25, 25), trackercolour(147, 210, 0), outlinecolour(20, 20, 20)
checkbox bounds(40, 200, 60, 14) channel("mute"), text("Freq. 2"), colour:0(25, 25, 25), colour:1(147, 210, 0), fontcolour(20, 20, 20)

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
