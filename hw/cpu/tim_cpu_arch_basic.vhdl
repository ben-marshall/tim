--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_arch_basic.vhdl
--! @brief Contains the first and most basic architecture implementation of the TIM CPU.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_common package,
use work.tim_common.word_width;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_instructions package,
use work.tim_instructions.tim_instruction;


--! The basic architecture of the tim CPU with little or no optimisation.
architecture  tim_cpu_rtl_basic of tim_cpu is

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

    --! The module responsible for fetching the next instruction from memory and making it available
    --! To the next pipeline stage.
    component tim_cpu_fetch is
        port(
            clk                     : in    std_logic; 
            reset                   : in    std_logic;
            halted                  : in    std_logic;
            memory_bus_lines        : inout std_logic_vector(word_width-1 downto 0);
            memory_bus_valid        : out   std_logic;
            memory_bus_enable       : in    std_logic;
            memory_bus_read_write   : out   std_logic;
            program_counter         : in    std_logic_vector(word_width-1 downto 0);
            decoded_instruction     : out   tim_instruction;
            instruction_valid       : out   std_logic;
            instruction_enable      : in    std_logic

        );
    end component tim_cpu_fetch;

    --
    -- Internal Signal Declarations.
    --

begin



end architecture tim_cpu_rtl_basic;
