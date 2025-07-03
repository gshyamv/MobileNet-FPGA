open_project unidepth
set_top unidw
add_files "unidw/unidw.cpp"
add_files "unidw/unidwtb.cpp" -tb
open_solution -reset "solution1"
set_part {xczu7ev-ffvc1156-2-e}   ;# Example target part, adjust as necessary
create_clock -period 10 -name default   ;# Example clock period
csim_design   ;# Optional: Run C simulation to verify the testbench and design
csynth_design
cosim_design   ;# Optional: Run co-simulation to verify RTL against C model
