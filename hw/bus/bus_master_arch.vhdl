--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_master_arch.vhdl
--! @brief Source code for the architecture declaration of the master bus controller.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_burst_width;

--! RTL & synthesisable architecture of the bus master controller.
architecture tim_bus_master_rtl of tim_bus_master is

    --! The current state of the controller.
    signal current_state    : tim_bus_master_state := RESET;
    --! The next state of the controller.
    signal next_state       : tim_bus_master_state := IDLE;

begin

    --! Responsible for the synchronous state transitions and asynchronous resets.
    state_machine_progress : process (clk, reset)
    begin
        if(reset='1') then
            current_state   <= RESET;
        elsif(clk'event and clk = '1') then
            current_state   <= next_state;
        end if;
    end process state_machine_progress;

end architecture tim_bus_master_rtl;
