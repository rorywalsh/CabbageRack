<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
ksmps   = 32
nchnls  = 2     ;2 in + 2 out
0dbfs   = 1   

instr   1   
a1 oscili 1, 100
outs a1, a1
;printk 1, 1
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
