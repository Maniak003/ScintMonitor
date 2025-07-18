; Produced by drl2gcode.py originally by Franco Lanza

; select absolute coordinate system
G90
; metric
G21
; G61 exact path mode was requested but not implemented
; start spindle
M3 S10000.0


; T6 Diameter: 1.63mm
G1 F1200.0 Z4.0
G1 F1200.0 X10.776 Y-2.036
G1 F1200.0 Z0.2
G1 F100.0 Z-3.0
G1 F1200.0 Z4.0
G1 F1200.0 X10.776 Y-17.526
G1 F1200.0 Z0.2
G1 F100.0 Z-3.0

; stop spindle
M5
; go to safe height
G1 Z4.0 F30000
; program ends
M2
