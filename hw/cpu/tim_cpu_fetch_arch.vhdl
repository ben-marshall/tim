--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_fetch_arch.vhdl
--! @brief Contains the first and most basic architecture implementation of the TIM CPU.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;

--! Architecture of the instruction fetch module.
architecture tim_cpu_fetch_arch of tim_cpu_fetch is

    --
    -- Imported component declarations.
    --

    --! The bus master controller module which arbitrates bus requests and responses.
    component tim_bus_master is
        generic(
            --! The number of data and address lines.
            data_width  : integer := tim_bus_data_width
        );
        port(
            clk             : in    std_logic;
            reset           : in    std_logic;
            bus_lines       : inout std_logic_vector(data_width-1 downto 0);
            bus_valid       : out   std_logic;
            bus_enable      : in    std_logic;
            bus_read_write  : out   std_logic;
            req_data_lines      : inout std_logic_vector(data_width-1 downto 0);
            req_read_write      : in    std_logic;
            req_address_lines   : in    std_logic_vector(data_width-1 downto 0);
            req_pending         : in    std_logic;
            req_acknowledge     : out   std_logic
        );
    end component tim_bus_master;

begin


end architecture tim_cpu_fetch_arch;
