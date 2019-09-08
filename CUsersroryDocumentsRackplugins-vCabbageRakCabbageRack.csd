<Cabbage>
form caption("CabbageModule") size(60, 380), colour(255, 255, 255), pluginid("def1")
cvinput bounds(20, 60, 20, 40), channel("cvInput1")
cvoutput bounds(20, 360, 20, 40), channel("cvOutput1")
rslider bounds(20, 200, 100, 100), channel("gain"), range(0, 1, 0, 1, .01)
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
