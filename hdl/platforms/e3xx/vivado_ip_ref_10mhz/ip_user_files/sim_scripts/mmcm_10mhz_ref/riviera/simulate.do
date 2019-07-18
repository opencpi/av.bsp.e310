onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+mmcm_10mhz_ref -L xil_defaultlib -L xpm -L unisims_ver -L unimacro_ver -L secureip -O5 xil_defaultlib.mmcm_10mhz_ref xil_defaultlib.glbl

do {wave.do}

view wave
view structure

do {mmcm_10mhz_ref.udo}

run -all

endsim

quit -force
