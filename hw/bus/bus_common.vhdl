--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_common.vhdl
--! @brief File containing package declarations for the bus logic.
--!
--! ------------------------------------------------------------------------------------------------

use work.tim_common.word_width;

--! This package contains entity declarations and shared constant values for the bus logic modules.
package tim_bus is

    --! The number of multiplexed data and address lines on the bus.
    constant tim_bus_data_width     : integer := 32;

    --! The number of bits used to represent the length of a burst.
    constant tim_bus_burst_width    : integer := 2;

end package;
