; Produced by drl2gcode.py originally by Franco Lanza

; select absolute coordinate system
G90
; metric
G21
; G61 exact path mode was requested but not implemented
; start spindle
M3 S10000.0


; T3 Diameter: 0.70mm
G1 F1200.0 Z4.0
G1 F1200.0 X-1.84 Y-44.35
G1 F1200.0 Z0.2
G1 F100.0 Z-3.0
G1 F1200.0 Z4.0
G1 F1200.0 X3.79 Y0.85
G1 F1200.0 Z0.2
G1 F100.0 Z-3.0
G1 F1200.0 Z4.0
G1 F1200.0 X62.88 Y-0.2
G1 F1200.0 Z0.2
G1 F100.0 Z-3.0
G1 F1200.0 Z4.0
G1 F1200.0 X63.09 Y-44.45
G1 F1200.0 Z0.2
G1 F100.0 Z-3.0

; stop spindle
M5
; go to safe height
G1 Z4.0 F30000
; program ends
M2
