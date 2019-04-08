onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib mmcm_10mhz_ref_opt

do {wave.do}

view wave
view structure
view signals

do {mmcm_10mhz_ref.udo}

run -all

quit -force
