--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_arch.vhdl
--! @brief Contains the architecture declaration/defintion for the TIM CPU module.
--!
--! ------------------------------------------------------------------------------------------------


--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! Architecture declaration for the cpu core.
architecture rtl of tim_cpu is
        
    --! Address lines for connecting to the master bus controller.
    signal req_bus_address_lines        : unsigned(address_bus_width-1 downto 0);
    --! Data lines for connecting to the master bus controller.
    signal req_bus_data_lines           : std_logic_vector(data_bus_width-1 downto 0);
    --! Request pending lines for connecting to the master bus controller.
    signal req_bus_pending              : std_logic;
    --! Request complete lines for connecting to the master bus controller.
    signal req_bus_complete             : std_logic;
    --! Write enable lines for connecting to the master bus controller.
    signal req_bus_write_enable         : std_logic;
    
    --! Global program counter, temporary until the register file module is finished.
    signal program_counter              : unsigned(address_bus_width-1 downto 0) := (others => '0');

    --! is the currently decoded instruction valid?
    signal decoded_instruction_valid    : std_logic;
    --! The size in bytes of the decoded instruction,
    signal decoded_instruction_size     : integer := 0;
    --! The most recently fetched and decoded instruction.
    signal decoded_instruction          : tim_instruction;
    --! The condition code of the decoded instruction.
    signal decoded_condition            : tim_instruction_condition;
    --! The immediate of the decoded instruction, aligned to the LSB.
    signal decoded_immediate            : std_logic_vector(immediate_width-1 downto 0);
    --! Decoded Register One
    signal decoded_reg_1                : tim_register;
    --! Decoded Register Two 
    signal decoded_reg_2                : tim_register;
    --! Decoded Register Three
    signal decoded_reg_3                : tim_register;

begin
    
    program_counter_update  : process(clk, reset, instruction_valid)
    begin
        if(clk = '1' and clk'event) then
            if(instruction_valid = '1') then
                program_counter <= program_counter + to_unsigned(decoded_instruction_size, 32);
                fetched_instruction <= 
            end if;
        end if;
    end process program_counter_update;

    --! The fetch decode module. Currently this is just hooked directly into the master bus controller
    --! eventually it will attatch via a controller multiplexer so other modules can access the bus.
    fetch_module    : entity work.tim_cpu_fetch_decode
    port map(
        clk                   => clk, 
        reset                 => reset,
        program_counter       => program_counter,
        
        instruction_recieved  => '1', -- tied high for now to constantly fetch instructions.
        instruction_valid     => decoded_instruction_valid,
        
        decoded_instruction_size => decoded_instruction_size,
        decoded_instruction      => decoded_instruction,
        decoded_condition        => decoded_condition,
        decoded_immediate        => decoded_immediate,
        decoded_reg_1            => decoded_reg_1,
        decoded_reg_2            => decoded_reg_2,
        decoded_reg_3            => decoded_reg_3,

        req_address_lines     => req_bus_address_lines,
        req_data_lines        => req_bus_data_lines,
        req_pending           => req_bus_pending,   
        req_complete          => req_bus_complete,
        req_write_enable      => req_bus_write_enable
    );


    --! Bus master controller.
    bus_master_controller   : entity work.bus_device(master)
    generic map(
        address_width   =>  memory_word_width,
        data_width      =>  memory_word_width,
        address_bottom  =>  to_unsigned(0, memory_word_width),
        address_top     =>  to_unsigned(0, memory_word_width)
    )
    port map(
        clk                => clk,
        reset              => reset,

        bus_address_lines  => system_bus_address_lines, 
        bus_data_lines     => system_bus_data_lines,    
        bus_address_valid  => system_bus_address_valid, 
        bus_data_valid     => system_bus_data_valid,    
        bus_enable         => system_bus_enable,        
        bus_write_enable   => system_bus_write_enable,  
        
        req_address_lines  => req_bus_address_lines,
        req_data_lines     => req_bus_data_lines,
        req_pending        => req_bus_pending,
        req_complete       => req_bus_complete,
        req_write_enable   => req_bus_write_enable
    );


end architecture rtl;
