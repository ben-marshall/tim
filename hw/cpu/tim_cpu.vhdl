--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu.vhdl
--! @brief Top file for the CPU module entity.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_common package,
use work.tim_common.word_width;

--! The top module of the tim CPU core.
entity tim_cpu is
    port(
        --! The main system clock.
        clk                     : in    std_logic; 
        --! Asynchonous reset signal.
        reset                   : in    std_logic;
        --! Set high IFF the CPU has halted processing.
        halted                  : out   std_logic;
        
        --! The lines which carry data and addresses;
        memory_bus_lines        : inout std_logic_vector(word_width-1 downto 0);
        --! Used to assert data written by the bus master to bus_lines is valid.
        memory_bus_valid        : out   std_logic;
        --! Used to assert that the slave has read the bus lines and they can be updated.
        memory_bus_enable       : in    std_logic;
        --! High if this transaction is a write, low if it is a read.
        memory_bus_read_write   : out   std_logic;

        --! The lines which carry data and addresses;
        device_bus_lines        : inout std_logic_vector(word_width-1 downto 0);
        --! Used to assert data written by the bus master to bus_lines is valid.
        device_bus_valid        : out   std_logic;
        --! Used to assert that the slave has read the bus lines and they can be updated.
        device_bus_enable       : in    std_logic;
        --! High if this transaction is a write, low if it is a read.
        device_bus_read_write   : out   std_logic
    );
end entity tim_cpu;

