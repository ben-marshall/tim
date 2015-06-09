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

    --! Testbench clock.
    signal clk                  : std_Logic := '0';
    --! Testbench reset signal.
    signal reset                : std_Logic := '1';

    --! Dummy constant to set the width of the bus.
    constant bus_width          : integer := memory_word_width;

    --! Shared bus signal.
    signal bus_address_lines    : unsigned(bus_width-1 downto 0);
    --! Shared bus signal.
    signal bus_data_lines       : std_logic_vector(bus_width-1 downto 0);
    --! Shared bus signal.
    signal bus_address_valid    : std_logic;
    --! Shared bus signal.
    signal bus_data_valid       : std_logic;
    --! Shared bus signal.
    signal bus_enable           : std_logic;
    --! Shared bus signal.
    signal bus_write_enable     : std_logic;

    --! Master signal
    signal master_address_lines : unsigned(bus_width-1 downto 0) := (others => '0');
    --! Master signal
    signal master_data_lines    : std_logic_vector(bus_width-1 downto 0);
    --! Master signal
    signal master_pending       : std_logic := '0';
    --! Master signal
    signal master_complete      : std_logic := '0';
    --! Master signal
    signal master_write         : std_logic := '0';

    --! Slave signal
    signal slave_address_lines : unsigned(bus_width-1 downto 0);
    --! Slave signal
    signal slave_data_lines    : std_logic_vector(bus_width-1 downto 0);
    --! Slave signal
    signal slave_pending       : std_logic := '0';
    --! Slave signal
    signal slave_complete      : std_logic := '0';
    --! Slave signal
    signal slave_write         : std_logic := '0';

begin
    
    reset   <= '0' after 60 ns;
    clk     <= not clk after 50 ns;


    --! An instance of the bus master device.
    master_device   : entity work.bus_device(master)
    generic map(
        address_width   => bus_width,
        data_width      => bus_width,
        address_bottom  => to_unsigned(0, bus_width),
        address_top     => to_unsigned(0, bus_width)
    )
    port map(
        clk               => clk,
        reset             => reset,
        bus_address_lines => bus_address_lines,
        bus_data_lines    => bus_data_lines,
        bus_address_valid => bus_address_valid,
        bus_data_valid    => bus_data_valid,
        bus_enable        => bus_enable,
        bus_write_enable  => bus_write_enable,
        req_address_lines => master_address_lines,
        req_data_lines    => master_data_lines,
        req_pending       => master_pending,
        req_complete      => master_complete,
        req_write_enable  => master_write
    );
    
    --! An instance of a bus slave device.
    slave_device   : entity work.bus_device(slave)
    generic map(
        address_width   => bus_width,
        data_width      => bus_width,
        address_bottom  => to_unsigned(0, bus_width),
        address_top     => to_unsigned(1024, bus_width)
    )
    port map(
        clk               => clk,
        reset             => reset,
        bus_address_lines => bus_address_lines,
        bus_data_lines    => bus_data_lines,
        bus_address_valid => bus_address_valid,
        bus_data_valid    => bus_data_valid,
        bus_enable        => bus_enable,
        bus_write_enable  => bus_write_enable,
        req_address_lines => slave_address_lines,
        req_data_lines    => slave_data_lines,
        req_pending       => slave_pending,
        req_complete      => slave_complete,
        req_write_enable  => slave_write
    );

end architecture testbench;
