<Cabbage>
form caption("Untitled") size(300, 380), colour(255, 255, 255), pluginid("def1")
rslider bounds(120, 8, 100, 100), channel("freq"), range(0, 2, 0, 1, 1), text("Gain"), colour(0, 0, 0, 255), trackercolour(0, 100, 0, 255)
cvoutput bounds(40, 360, 20, 40), channel("signalOutput1"), colour(255, 0, 0)
image bounds(10, 158, 200, 200), file("choice.svg")
button bounds(10, 112, 101, 41), text("On", "Off"), fontcolour(0, 0, 0)
checkbox bounds(10, 264, 100, 30), text("Check"), fontcolour(0,0,0)
combobox bounds(10, 300, 100, 30), fontcolour(255, 255, 255) text("One", "Two", "Three"), channel("combo1") fontcolour(0,0,0)

light bounds(100, 10, 20, 20), channel("light1"), colour(0, 255, 0), fontcolour(0,0,0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


instr 1
printk2 chnget:k("testChan")
kFreq chnget "freq"
printk2 kFreq
a1 oscili 1, 1000
printk2 chnget:k("choice")
kLight init 0
chnset a1, "signalOutput1"

if metro(1) == 1 then    
    chnset kLight, "light1"
    kLight = (kLight == 0 ? 1 : 0)
endif

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
