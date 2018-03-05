
<Cabbage>
form size(330, 380), colour(255, 255, 255)

image bounds(246, 6, 80, 238), colour(40,40,40,255)
label bounds(252, 10, 80, 16),  fontcolour(255, 255, 255, 255), text("Gate 1")
label bounds(252, 40, 80, 16),  fontcolour(255, 255, 255, 255), text("Gate 2")
label bounds(252, 70, 80, 16),  fontcolour(255, 255, 255, 255), text("Gate 3")
label bounds(252, 100, 80, 16), fontcolour(255, 255, 255, 255), text("Gate 4")
label bounds(252, 130, 80, 16), fontcolour(255, 255, 255, 255), text("Gate 5")
label bounds(252, 160, 80, 16), fontcolour(255, 255, 255, 255), text("Gate 6")
label bounds(252, 190, 80, 16), fontcolour(255, 255, 255, 255), text("Gate 7")
label bounds(252, 220, 80, 16), fontcolour(255, 255, 255, 255), text("Gate 8")

cvoutput bounds(250, 0, 20, 40), channel("gate1")
cvoutput bounds(250, 30, 20, 40), channel("gate2")
cvoutput bounds(250, 60, 20, 40), channel("gate3")
cvoutput bounds(250, 90, 20, 40), channel("gate4")
cvoutput bounds(250, 120, 20, 40), channel("gate5")
cvoutput bounds(250, 150, 20, 40), channel("gate6")
cvoutput bounds(250, 180, 20, 40), channel("gate7")
cvoutput bounds(250, 210, 20, 40), channel("gate8")


checkbox bounds(10,  10,  20, 20), channel("row1_1")
checkbox bounds(40,  10,  20, 20), channel("row1_2")
checkbox bounds(70,  10,  20, 20), channel("row1_3")
checkbox bounds(100, 10, 20, 20), channel("row1_4")
checkbox bounds(130, 10, 20, 20), channel("row1_5")
checkbox bounds(160, 10, 20, 20), channel("row1_6")
checkbox bounds(190, 10, 20, 20), channel("row1_7")
checkbox bounds(220, 10, 20, 20), channel("row1_8")

checkbox bounds(10,  40,  20, 20), channel("row2_1")
checkbox bounds(40,  40,  20, 20), channel("row2_2")
checkbox bounds(70,  40,  20, 20), channel("row2_3")
checkbox bounds(100,  40, 20, 20), channel("row2_4")
checkbox bounds(130,  40, 20, 20), channel("row2_5")
checkbox bounds(160,  40, 20, 20), channel("row2_6")
checkbox bounds(190,  40, 20, 20), channel("row2_7")
checkbox bounds(220,  40, 20, 20), channel("row2_8")



checkbox bounds(10,  70,  20, 20), channel("row3_1")
checkbox bounds(40,  70,  20, 20), channel("row3_2")
checkbox bounds(70,  70,  20, 20), channel("row3_3")
checkbox bounds(100, 70, 20, 20), channel("row3_4")
checkbox bounds(130, 70, 20, 20), channel("row3_5")
checkbox bounds(160, 70, 20, 20), channel("row3_6")
checkbox bounds(190, 70, 20, 20), channel("row3_7")
checkbox bounds(220, 70, 20, 20), channel("row3_8")

checkbox bounds(10,  100,  20, 20), channel("row4_1")
checkbox bounds(40,  100,  20, 20), channel("row4_2")
checkbox bounds(70,  100,  20, 20), channel("row4_3")
checkbox bounds(100, 100, 20, 20), channel("row4_4")
checkbox bounds(130, 100, 20, 20), channel("row4_5")
checkbox bounds(160, 100, 20, 20), channel("row4_6")
checkbox bounds(190, 100, 20, 20), channel("row4_7")
checkbox bounds(220, 100, 20, 20), channel("row4_8")

checkbox bounds(10,  130,  20, 20), channel("row5_1")
checkbox bounds(40,  130,  20, 20), channel("row5_2")
checkbox bounds(70,  130,  20, 20), channel("row5_3")
checkbox bounds(100, 130, 20, 20), channel("row5_4")
checkbox bounds(130, 130, 20, 20), channel("row5_5")
checkbox bounds(160, 130, 20, 20), channel("row5_6")
checkbox bounds(190, 130, 20, 20), channel("row5_7")
checkbox bounds(220, 130, 20, 20), channel("row5_8")

checkbox bounds(10,  160,  20, 20), channel("row6_1")
checkbox bounds(40,  160,  20, 20), channel("row6_2")
checkbox bounds(70,  160,  20, 20), channel("row6_3")
checkbox bounds(100, 160, 20, 20), channel("row6_4")
checkbox bounds(130, 160, 20, 20), channel("row6_5")
checkbox bounds(160, 160, 20, 20), channel("row6_6")
checkbox bounds(190, 160, 20, 20), channel("row6_7")
checkbox bounds(220, 160, 20, 20), channel("row6_8")

checkbox bounds(10,  190,  20, 20), channel("row7_1")
checkbox bounds(40,  190,  20, 20), channel("row7_2")
checkbox bounds(70,  190,  20, 20), channel("row7_3")
checkbox bounds(100, 190, 20, 20), channel("row7_4")
checkbox bounds(130, 190, 20, 20), channel("row7_5")
checkbox bounds(160, 190, 20, 20), channel("row7_6")
checkbox bounds(190, 190, 20, 20), channel("row7_7")
checkbox bounds(220, 190, 20, 20), channel("row7_8")

checkbox bounds(10,  220,  20, 20), channel("row8_1")
checkbox bounds(40,  220,  20, 20), channel("row8_2")
checkbox bounds(70,  220,  20, 20), channel("row8_3")
checkbox bounds(100, 220, 20, 20), channel("row8_4")
checkbox bounds(130, 220, 20, 20), channel("row8_5")
checkbox bounds(160, 220, 20, 20), channel("row8_6")
checkbox bounds(190, 220, 20, 20), channel("row8_7")
checkbox bounds(220, 220, 20, 20), channel("row8_8")

rslider bounds(162, 332, 50, 46) textcolour(0,0,0,255), channel("tempo"), range(0, 10, 1, 1, 0.001), text("Tempo")
rslider bounds(112, 332, 50, 46) textcolour(0,0,0,255), channel("gain"), range(0, 10, 6, 1, 0.001), text("Gain")

light bounds( 15, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light1")
light bounds( 45, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light2")
light bounds( 75, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light3")
light bounds(105, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light4")
light bounds(135, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light5")
light bounds(165, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light6")
light bounds(195, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light7")
light bounds(225, 242, 10, 10), colour(255, 0, 0), corners(7), channel("light8")

groupbox bounds(184, 256, 142, 70), colour(30, 30, 30), linethickness(0), text("Filter") outlinecolour(20, 20, 20), fontcolour(255,255,255,255)
rslider bounds(192, 266, 52, 53) range(1, 15000, 10000, 1, .01), text("Freq") channel("cutoff") textcolour(255,255,255,255), colour(255, 255, 255, 255) outlinecolour(0,0,0,0)
rslider bounds(232, 270, 52, 53) range(0, 1, 0, 1, .01), text("Res."), channel("res") textcolour(255,255,255,255), colour(255,255,255,255) outlinecolour(0,0,0,0)
rslider bounds(270, 266, 52, 53) range(0, 4, 0, 1, .01) text("Dist."), channel("dist") colour(255,255,255,255) textcolour(255,255,255,255), outlinecolour(0,0,0,0)

groupbox bounds(10, 256, 171, 70), colour(30, 30, 30), linethickness(0), text("ADSR") outlinecolour(20, 20, 20), fontcolour(255,255,255,255)
rslider bounds(12, 266, 52, 53) range(0.01, 1, .01, 1, .01), text("Att.") channel("att") colour(255,255,255,255) textcolour(255,255,255,255), outlinecolour(0,0,0,0)
rslider bounds(52, 270, 52, 53) range(0, 1, .1, 1, .01), text("Dec."), channel("dec") colour(255,255,255,255) textcolour(255,255,255,255), outlinecolour(0,0,0,0)
rslider bounds(94, 270, 52, 53) range(0, 1, .5, 1, .01) text("Sus."), channel("sus") colour(255,255,255,255) textcolour(255,255,255,255), outlinecolour(0,0,0,0)
rslider bounds(132, 266, 52, 53) range(0, 1, .5, 1, .01) text("Rel."), channel("rel") colour(255,255,255,255) textcolour(255,255,255,255), outlinecolour(0,0,0,0)

combobox bounds(16, 340, 92, 25), value(1), text("Saw", "Lo-Fi", "Wtf?"), channel("table")
cvoutput bounds(245, 346, 20, 20), channel("sigOutput")
label bounds(246, 346, 80, 16), text("Output"), fontcolour(0, 0, 0, 255) fontcolour(0, 0, 0, 255)




</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d
</CsOptions>
<CsInstruments>
ksmps   = 16
nchnls  = 2     
0dbfs   = 1
 
;k1 downsamp aInFreq
;kFreq = 261*pow(2.0, k1*10)

instr 1 
kNotes[] fillarray 60, 62, 64, 65, 67, 69, 71, 72

kRow = 1
kCol init 1
kTempo chnget "tempo"
kLightCnt = 1
if metro(kTempo) == 1 then
    while kRow < 9 do
        event "i", 2, 0, .1, kRow, kCol, kNotes[8-kRow]
        kRow += 1
    od 

    while kLightCnt < 9 do
        SLightChannel sprintfk "light%d", kLightCnt
        chnset kCol==kLightCnt ? 1 : 0, SLightChannel 
        kLightCnt += 1
    od

    kCol = kCol < 8 ? kCol+1 : 1
endif
endin


instr 2
    aOut init 0
    SChannel sprintfk "row%d_%d", p4, p5
    SChannelGate sprintfk "gate%d", p4
    SChannelVOct sprintfk "freq%d", p4
    iShouldPlay chnget SChannel
    chnset iShouldPlay == 1 ? a(1) : a(0), SChannelGate
    if iShouldPlay == 1 then
        aEnv madsr chnget:i("att"), chnget:i("dec"), chnget:i("sus"), chnget:i("rel")
        if chnget:i("table") == 1 then
            aOut vco2 1, cpsmidinn(p6)
        elseif chnget:i("table") == 2 then
            aOut oscili 1, cpsmidinn(p6), 2
        else
            aOut oscili 1, cpsmidinn(p6), 3
        endif
        ;Out*chnget:k("gain"), aOut*chnget:k("gain")
        chnmix aEnv*aOut*chnget:k("gain"), "signal"
    endif       
    
endin

instr 1000
;aL, aR monitor
   aSig chnget "signal"
   outs aSig, aSig
   aFlt lpf18 aSig, chnget:k("cutoff"), chnget:k("res"), chnget:k("dist")
   chnset aFlt, "sigOutput"
   chnclear "signal"   
endin

</CsInstruments>
<CsScore>
f2 0 16 10 1
f3 0 4096 10 1 0 1 .1 .5 .2 .7 .1 .2
i1 0 z
i1000 0 z
</CsScore>
</CsoundSynthesizer>
