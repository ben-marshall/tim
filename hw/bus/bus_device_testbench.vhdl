--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device_testbench.vhdl
--! @brief Contains the testbench for all bus devices.
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

--! Testbench entity for bus devices.
entity bus_device_testbench is
end entity bus_device_testbench;

--! Testbench architecture for bus devices.
architecture testbench of bus_device_testbench is

    --! Forward declaration of the bus device component entity.
    component bus_device  is
        generic(
            address_width   : integer   := memory_word_width;
            data_width      : integer   := memory_word_width;
            address_bottom  : unsigned(memory_word_width-1 downto 0)  := to_unsigned(0,memory_word_width);
            address_top     : unsigned(memory_word_width-1 downto 0)  := to_unsigned(0,memory_word_width)
        );
        port(
            clk                 : in    std_logic;
            reset               : in    std_logic;
            bus_address_lines   : inout std_logic_vector(address_width-1 downto 0);
            bus_data_lines      : inout std_logic_vector(data_width-1 downto 0);
            bus_address_valid   : inout std_logic;
            bus_data_valid      : inout std_logic;
            bus_enable          : inout std_logic;
            bus_write_enable    : inout std_logic;
            req_address_lines   : inout std_logic_vector(address_width-1 downto 0);
            req_data_lines      : inout std_logic_vector(data_width-1 downto 0);
            req_pending         : inout std_logic;
            req_complete        : inout std_logic;
            req_write_enable    : inout std_logic
        );
    end component bus_device;

begin



end architecture testbench;
