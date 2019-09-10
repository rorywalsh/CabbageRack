<Cabbage>
form caption("CabbageModule") size(100, 380), colour(255, 255, 255), pluginid("def1")
screw bounds(5, 10, 15, 15)
screw bounds(80, 10, 15, 15)
screw bounds(5, 360, 15, 15)
screw bounds(80, 360, 15, 15)

cvinput bounds(34, 60, 30, 30), channel("cvInput1")
label bounds(0, 90, 100, 12), fontcolour(0,0,0) text("Input")
cvoutput bounds(34, 300, 30, 30), channel("cvOutput1")
label bounds(0, 330, 100, 12), fontcolour(0,0,0) text("Output")
rslider bounds(0, 150, 100, 100), channel("gain"), text("Gain"), textcolour(0, 0, 0, 255), range(0, 1, 0, 1, 0.01)
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
    printk2 chnget:k("gain")
    aOut = aInput*chnget:k("gain")

    chnset aOut, "cvOutput1"
endin

</CsInstruments>
<CsScore>
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
