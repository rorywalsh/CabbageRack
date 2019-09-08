<Cabbage>
form caption("CabbageModule") size(100, 380), colour(255, 255, 255), pluginid("def1")
cvinput bounds(40, 60, 20, 20), channel("cvInput1")
label bounds(0, 90, 100, 40), fontcolour(0,0,0) text("Input")
cvoutput bounds(40, 300, 20, 20), channel("cvOutput1")
label bounds(0, 330, 100, 40), fontcolour(0,0,0) text("Output")
rslider bounds(0, 150, 100, 100), channel("gain"), textcolour(0, 0, 0), range(0, 1, 0, 1, .01)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1
    aInput chnget "cvInput1"
    
    aOut = aInput*chnget:k("gain")

    chnset a1, "cvOutput1"
endin

</CsInstruments>
<CsScore>
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
