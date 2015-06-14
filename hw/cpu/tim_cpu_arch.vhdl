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
        
    signal req_bus_address_lines    : unsigned(address_bus_width-1 downto 0);
    signal req_bus_data_lines       : std_logic_vector(data_bus_width-1 downto 0);
    signal req_bus_pending          : std_logic;
    signal req_bus_complete         : std_logic;
    signal req_bus_write_enable     : std_logic;

    --! The most recently fetched and decoded instruction.
    signal decoded_instruction      : tim_instruction;

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

    --! The fetch decode module.
    fetch_module    : entity work.tim_cpu_fetch_decode
    port map(
        clk                   => clk, 
        reset                 => reset,
        program_counter       => program_counter,
        
        instruction_recieved  => '1',
        instruction_valid     => instruction_valid,
        
        decoded_instruction_size => decoded_instruction_size,
        decoded_instruction      => decoded_instruction,

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


end architecture testbench;
