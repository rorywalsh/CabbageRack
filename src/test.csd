<Cabbage>
form size(120, 380), colour(40, 40, 40)
rslider bounds(40, 42, 50, 50) channel("freq1") range(0, 1000, 56, 1, 0.001), colour(25, 25, 25), trackercolour(147, 210, 0), outlinecolour(20, 20, 20), 
rslider bounds(40, 140, 50, 50) channel("freq2") range(0, 1000, 120, 1, 0.001), colour(25, 25, 25)), trackercolour(147, 210, 0), outlinecolour(20, 20, 20)
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
a1 oscili 1, chnget:k("freq1")
a2 oscili 1, chnget:k("freq2")
outs (a1*a2)/2, (a1*a2)/2
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
