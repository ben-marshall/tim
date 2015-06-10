--! ------------------------------------------------------------------------------------------------
--!
--! @file  tb_cpu_fetch_decode.vhdl
--! @brief Testbench file for the cpu fetch decode module.
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

--! Testbench entity declaration.
entity fetch_decode_testbench is
end entity fetch_decode_testbench;

--! Architecture declaration for the testbench
architecture testbench of fetch_decode_testbench is

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
        
    --! The main system clock.
    signal  clk                     : std_logic   := '0'; 
    --! Asynchonous reset signal.
    signal  reset                   : std_logic   := '1';
            
    --
    -- Main system bus signals.
    --

    signal system_bus_address_lines   : unsigned(address_bus_width-1 downto 0);
    signal system_bus_data_lines      : std_logic_vector(data_bus_width-1 downto 0);
    signal system_bus_address_valid   : std_logic;
    signal system_bus_data_valid      : std_logic;
    signal system_bus_enable          : std_logic;
    signal system_bus_write_enable    : std_logic;
    
    signal req_bus_address_lines   : unsigned(address_bus_width-1 downto 0);
    signal req_bus_data_lines      : std_logic_vector(data_bus_width-1 downto 0);
    signal req_bus_pending         : std_logic;
    signal req_bus_complete        : std_logic;
    signal req_bus_write_enable    : std_logic;

begin
    
    reset   <= '0' after 50 ns;
    clk     <= not clk  after 20 ns;

    --! The fetch decode module.
    fetch_module    : entity work.tim_cpu_fetch_decode
    port map(
        clk                   => clk, 
        reset                 => reset,
        program_counter       => (others => '0'),
        instruction_recieved  => '1',
        req_address_lines     =>  req_bus_address_lines,
        req_data_lines        =>  req_bus_data_lines,
        req_pending           =>  req_bus_pending,   
        req_complete          =>  req_bus_complete,
        req_write_enable      =>  req_bus_write_enable
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


end architecture testbench;
