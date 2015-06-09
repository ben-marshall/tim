--! ------------------------------------------------------------------------------------------------
--!
--! @file  top.vhdl
--! @brief Top structural file for the whole FPGA design. Doubles as a testbench scafold.
--!
--! ------------------------------------------------------------------------------------------------


--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

use work.tim_common.address_bus_width;
use work.tim_common.data_bus_width;
use work.tim_common.memory_word_width;

--! Top level entity declaration along with all IO signals to the FPGA.
entity top is
    port(
        --! The main system clock.
        clk                     : in    std_logic; 
        --! Asynchonous reset signal.
        reset                   : in    std_logic;
        --! Set high IFF the CPU has halted processing.
        halted                  : out   std_logic
    );
end entity top;

--! Architecture declaration for the top level module.
architecture rtl of top is

    --! A bram memory module connected to the system bus.
    component mem_bus_bram    is
        generic(
            address_bottom  : unsigned  := to_unsigned(0, memory_word_width);
            address_top     : unsigned  := to_unsigned(0, memory_word_width)
        );
        port(
            clk                 : in    std_logic;
            reset               : in    std_logic;
            bus_address_lines   : inout unsigned(31 downto 0);
            bus_data_lines      : inout std_logic_vector(31 downto 0);
            bus_address_valid   : inout std_logic;
            bus_data_valid      : inout std_logic;
            bus_enable          : inout std_logic;
            bus_write_enable    : inout std_logic
        );
    end component mem_bus_bram;
            
    --
    -- Main system bus signals.
    --

    signal system_bus_address_lines   : unsigned(address_bus_width-1 downto 0);
    signal system_bus_data_lines      : std_logic_vector(data_bus_width-1 downto 0);
    signal system_bus_address_valid   : std_logic;
    signal system_bus_data_valid      : std_logic;
    signal system_bus_enable          : std_logic;
    signal system_bus_write_enable    : std_logic;

begin


    --
    -- Main RAM memory block declaration.
    --

    system_memory   : mem_bus_bram 
    generic map(
        address_bottom    => to_unsigned(0,address_bus_width),
        address_top       => to_unsigned(511,address_bus_width)
    )
    port map(
        clk               => clk,
        reset             => reset,
        bus_address_lines => system_bus_address_lines,
        bus_data_lines    => system_bus_data_lines,
        bus_address_valid => system_bus_address_valid,
        bus_data_valid    => system_bus_data_valid,
        bus_enable        => system_bus_enable,
        bus_write_enable  => system_bus_write_enable
    );



end architecture rtl;
