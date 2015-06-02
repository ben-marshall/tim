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

    --! The various state machine states of the bus master controller.
    type tim_bus_master_state is (BUS_RESET, IDLE, REQ, READ, WRITE);

end package;
