--! ------------------------------------------------------------------------------------------------
--!
--! @file tb_tim_cpu.vhdl
--! @brief Testbench file for the CPU.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_common package,
use work.tim_common.word_width;

--! Top testbench entity for the CPU.
entity tb_tim_cpu is
end entity tb_tim_cpu;

--! Testbench architecture of the CPU.
architecture testbench of tb_tim_cpu is

    --
    -- Component Declarations.
    --

    --! The top module of the tim CPU core.
    component tim_cpu is
        port(
            clk                     : in    std_logic; 
            reset                   : in    std_logic;
            halted                  : out   std_logic;
            
            memory_bus_lines        : inout std_logic_vector(word_width-1 downto 0);
            memory_bus_valid        : out   std_logic;
            memory_bus_enable       : in    std_logic;
            memory_bus_read_write   : out   std_logic;

            device_bus_lines        : inout std_logic_vector(word_width-1 downto 0);
            device_bus_valid        : out   std_logic;
            device_bus_enable       : in    std_logic;
            device_bus_read_write   : out   std_logic
        );
    end component tim_cpu;

    --! A dummy RAM memory module to be used for simulations.
    component mem_dummy is
        generic(
            mem_size             : integer   := 1023;
            data_width           : integer   := word_width;
            address_range_top    : integer   := 1023;
            address_range_bottom : integer   := 0
        );
        port(
            clk             : in    std_logic;
            reset           : in    std_logic;
            bus_lines       : inout std_logic_vector(data_width-1 downto 0);
            bus_valid       : in   std_logic;
            bus_enable      : out    std_logic;
            bus_read_write  : in   std_logic
        );
    end component mem_dummy;

    -- ------------------------------------------------------------------------
    -- Signal Declarations
    --

    --! Testbench clock
    signal tb_clk               : std_logic := '0';
    --! Testbench reset signal.
    signal tb_reset             : std_logic := '1';
    --! Tells the testbench that the CPU has finished and we can stop the simulation.
    signal dut_halted           : std_logic;

    --
    -- Memory and Device Bus signals.
    --

    signal tb_memory_bus_lines        : std_logic_vector(word_width-1 downto 0);
    signal tb_memory_bus_valid        : std_logic;
    signal tb_memory_bus_enable       : std_logic;
    signal tb_memory_bus_read_write   : std_logic;

    signal tb_device_bus_lines        : std_logic_vector(word_width-1 downto 0);
    signal tb_device_bus_valid        : std_logic;
    signal tb_device_bus_enable       : std_logic;
    signal tb_device_bus_read_write   : std_logic;
    

begin

    tb_reset    <= '0' after 70 ns;
    tb_clk      <= not tb_clk after 20 ns;


    --! The CPU core instance.
    dut : tim_cpu port map(
        clk                    => tb_clk, 
        reset                  => tb_reset,
        halted                 => dut_halted,

        memory_bus_lines       =>  tb_memory_bus_lines,
        memory_bus_valid       =>  tb_memory_bus_valid,
        memory_bus_enable      =>  tb_memory_bus_enable,
        memory_bus_read_write  =>  tb_memory_bus_read_write,
 
        device_bus_lines       =>  tb_device_bus_lines,
        device_bus_valid       =>  tb_device_bus_valid,
        device_bus_enable      =>  tb_device_bus_enable,
        device_bus_read_write  =>  tb_device_bus_read_write 
    );

    --! Serves as a dummy device on the device bus.
    device_memory   : mem_dummy generic map(
        mem_size             => 1023,
        data_width           => word_width,
        address_range_top    => 1023,
        address_range_bottom => 0
    ) port map(
        clk                  =>  tb_clk,
        reset                =>  tb_reset,         
        bus_lines            =>  tb_device_bus_lines,     
        bus_valid            =>  tb_device_bus_valid,     
        bus_enable           =>  tb_device_bus_enable,    
        bus_read_write       =>  tb_device_bus_read_write
    );

    --! Serves as a dummy device on the memory bus.
    system_memory   : mem_dummy generic map(
        mem_size             => 1023,
        data_width           => word_width,
        address_range_top    => 1023,
        address_range_bottom => 0
    ) port map(
        clk                  =>  tb_clk,
        reset                =>  tb_reset,         
        bus_lines            =>  tb_memory_bus_lines,     
        bus_valid            =>  tb_memory_bus_valid,     
        bus_enable           =>  tb_memory_bus_enable,    
        bus_read_write       =>  tb_memory_bus_read_write
    );

end architecture testbench;
