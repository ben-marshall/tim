--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_fetch_decode.vhdl
--! @brief Entity declaration for the fetch & decode module of the CPU.
--!
--! ------------------------------------------------------------------------------------------------

--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! Imported from tim_common package.
use work.tim_common.address_bus_width;
--! Imported from tim_common package.
use work.tim_common.data_bus_width;
--! Imported from tim_instructions package,
use work.tim_instructions.immediate_width;
--! Imported from tim_instructions package,
use work.tim_instructions.tim_instruction;
--! Imported from tim_instructions package,
use work.tim_instructions.tim_instruction_condition;

--! Entity of the instruction fetch & decode module.
entity tim_cpu_fetch_decode is
    port(
        --! The main system clock.
        clk                     : in    std_logic; 
        --! Asynchonous reset signal.
        reset                   : in    std_logic;

        --! The address of the next instruction to fetch.
        program_counter         : in    unsigned(address_bus_width-1 downto 0);

        --! The currently fetched & available instruction.
        decoded_instruction     : out   tim_instruction;
        --! The condition code of the decoded instruction.
        decoded_condition       : out   tim_instruction_condition;
        --! The immediate of the decoded instruction, aligned to the LSB.
        decoded_immediate       : out   std_logic_vector(immediate_width-1 downto 0);
        --! Decoded Register One
        decoded_reg_1           : out   tim_register;
        --! Decoded Register Two 
        decoded_reg_2           : out   tim_register;
        --! Decoded Register Three
        decoded_reg_3           : out   tim_register;
        --! The size in bytes of the decoded instruction.
        decoded_instruction_size: buffer integer;

        --! Signals that an instruction has been fetched, decoded and made available.
        instruction_valid       : out   std_logic;
        --! Signals that the decoded instruction has been recieved and that the module can fetch the next one.
        instruction_recieved    : in    std_logic;

        --
        -- Memory and data bus interface. These will be connected to a master bus controller
        -- architecture inside the cpu core and multiplexed to the fetch_decode module and
        -- Anything else that does memory bus transactions.
        --
        
        --! The current address of the thing being accessed on the bus.
        req_address_lines   : out unsigned(address_bus_width-1 downto 0);
        --! The data being carried on the bus.
        req_data_lines      : in std_logic_vector(data_bus_width-1 downto 0);
        --! Signal to tell the controller/device that a request is pending and needs attention.
        req_pending         : out std_logic;
        --! Asserted high to tell the device/bus controller that the request has been completed and that the response
        --! is available on the data lines.
        req_complete        : in std_logic;
        --! high = write, low = read operation.
        req_write_enable    : out std_logic

    );
end entity tim_cpu_fetch_decode;
