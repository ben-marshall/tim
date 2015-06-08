--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu.vhdl
--! @brief Top entity declaration file for the CPU module entity.
--!
--! ------------------------------------------------------------------------------------------------


--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;


--! The top module of the tim CPU core.
entity tim_cpu is
    port(
        --! The main system clock.
        clk                     : in    std_logic; 
        --! Asynchonous reset signal.
        reset                   : in    std_logic;
        --! Set high IFF the CPU has halted processing.
        halted                  : out   std_logic;

        --
        -- Memory and data bus interface. These will be connected to a master bus controller
        -- architecture inside the cpu core.
        --
        
        --! The current address of the thing being accessed on the bus.
        mem_bus_address_lines   : inout std_logic_vector(address_width-1 downto 0);
        --! The data being carried on the bus.
        mem_bus_data_lines      : inout std_logic_vector(data_width-1 downto 0);
        --! Signal to tell the rest of the bus that the address lines are valid, initiating a transaction.
        mem_bus_address_valid   : inout std_logic;
        --! Signal to tell the rest of the bus that the data lines are valid.
        mem_bus_data_valid      : inout std_logic;
        --! Enable signal to let slaves tell the master they are finished with the data on the bus.
        mem_bus_enable          : inout std_logic;
        --! high = write, low = read operation.
        mem_bus_write_enable    : inout std_logic
    );
end entity tim_cpu;

