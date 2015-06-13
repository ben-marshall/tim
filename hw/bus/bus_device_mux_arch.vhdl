--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device_mux_arch.vhdl
--! @brief Acts as a multiplexer for bus controllers. Allows multiple modules to use the same
--!        bus device master controller using an interrupt system. No extra logic within
--!        the bus controller, or the module it communicates with, is neded to attatch it to this
--!        multiplexer. It is a simple dropin replacement.
--!
--! ------------------------------------------------------------------------------------------------

--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! Use the width of a single memory word as the default width of the bus.
use work.tim_common.memory_word_width;


--! Architecture of the bus controller multiplexer
architecture rtl of bus_device_mux is



begin



end architecture rtl;
