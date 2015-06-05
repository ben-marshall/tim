--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_arch_basic.vhdl
--! @brief Contains the first and most basic architecture implementation of the TIM CPU.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--pragma translate_off
library std;
use std.env.all;
--pragma translate_on

--! Imported from tim_common package,
use work.tim_common.word_width;
use work.tim_common.opcode_length;
use work.tim_common.reg_pc;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_instructions package,
use work.tim_instructions.tim_instruction;


--! The basic architecture of the tim CPU with little or no optimisation.
architecture  tim_cpu_rtl_basic of tim_cpu is

    --
    -- Imported component declarations.
    --

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
            decoded_instruction_size: out   integer;
            decoded_arguments       : out   std_logic_vector(word_width-opcode_length downto 0);
            instruction_valid       : out   std_logic;
            instruction_enable      : in    std_logic

        );
    end component tim_cpu_fetch;

    --
    -- Internal Signal Declarations.
    --

    signal internal_halted                  : std_logic := '0';

    --! Typedef for the register file.
    type tim_cpu_reg_file   is array (0 to 31) of std_logic_vector(word_width-1 downto 0);

    --! The register file for all special, temporary and general purpose registers.
    signal internal_register_file           : tim_cpu_reg_file;

    --! The decoded instruction from the fetch module.
    signal decoded_instruction_size         : integer;
    signal decoded_instruction              : tim_instruction;
    signal decoded_instruction_valid        : std_logic;

begin

    --
    -- Continuous Assignments.
    --

    halted                      <= internal_halted;

    --
    -- Processes
    --

    --! Responsible for updating the value of the program counter.
    program_counter_next_value  : process(reset, clk, decoded_instruction_valid)
    begin

        if(reset = '1') then
            internal_register_file(reg_pc) <= (others => '0');
        elsif(decoded_instruction_valid = '1' and decoded_instruction_valid'event) then
            internal_register_file(reg_pc) <= std_logic_vector(unsigned(internal_register_file(reg_pc))+decoded_instruction_size);
        else 
            internal_register_file(reg_pc) <= internal_register_file(reg_pc);
        end if;

    end process program_counter_next_value;


    --pragma translate_off
    simulation_halt : process(decoded_instruction)
    begin
        if(decoded_instruction = HALT) then
            report "HALT instruction encountered. Simulation stopping.";
            finish(2);
        end if;
    end process;
    --pragma translate_on

    
    --
    -- Component Instances
    --
    
    --! Fetches and decodes instructions from main memory.
    module_fetch : tim_cpu_fetch port map(
        clk                     => clk,
        reset                   => reset,
        halted                  => internal_halted,
        memory_bus_lines        => memory_bus_lines,
        memory_bus_valid        => memory_bus_valid,
        memory_bus_enable       => memory_bus_enable,
        memory_bus_read_write   => memory_bus_read_write,
        program_counter         => internal_register_file(reg_pc),
        decoded_instruction     => decoded_instruction,
        decoded_instruction_size=> decoded_instruction_size,
        instruction_valid       => decoded_instruction_valid,
        instruction_enable      => '1' -- Permenantly tied high for now.
    );

end architecture tim_cpu_rtl_basic;
