--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_fetch.vhdl
--! @brief Entity file for the CPU fetch pipeline stage.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_common package,
use work.tim_common.word_width;
use work.tim_common.opcode_length;
--! Imported from tim_instructions package,
use work.tim_instructions.tim_instruction;

--! The module responsible for fetching the next instruction from memory and making it available
--! To the next pipeline stage.
entity tim_cpu_fetch is
    port(
        --! The main system clock.
        clk                     : in    std_logic; 
        --! Asynchonous reset signal.
        reset                   : in    std_logic;
        --! Set high IFF the CPU has halted processing.
        halted                  : in    std_logic;
        
        -- It is these signals that will let the fetch module communicate with system memory.

        --! The lines which carry data and addresses;
        memory_bus_lines        : inout std_logic_vector(word_width-1 downto 0);
        --! Used to assert data written by the bus master to bus_lines is valid.
        memory_bus_valid        : out   std_logic;
        --! Used to assert that the slave has read the bus lines and they can be updated.
        memory_bus_enable       : in    std_logic;
        --! High if this transaction is a write, low if it is a read.
        memory_bus_read_write   : out   std_logic;

        --! The value of the program counter containing the NEXT instruction to fetch.
        program_counter         : in    std_logic_vector(word_width-1 downto 0);

        --! The most recently fetched instruction.
        decoded_instruction     : out   tim_instruction;
        --! The length of the instruction in bytes.
        decoded_instruction_size: out   integer;
        --! The arguments of the most recently fetched instruction.
        decoded_arguments       : out   std_logic_vector(word_width-opcode_length downto 0);
        --! Whether the fetched instruction is valid.
        instruction_valid       : out   std_logic;
        --! enable the next fetched instruction to be outputed.
        instruction_enable      : in    std_logic

    );
end entity tim_cpu_fetch;
