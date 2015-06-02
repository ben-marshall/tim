--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_slave.vhdl
--! @brief Source code for the slave bus controller entity declaration.
--!
--! ------------------------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;


--! The bus slave controller module which responds to requests from the master.
entity tim_bus_slave is
    generic(
        --! The number of data and address lines.
        data_width           : integer := tim_bus_data_width;

        --! The top of the range of addresses to which this slave controller will respond to requests.
        address_range_top    : integer   := 1023;

        --! The bottomof the range of addresses to which this slave controller will respond to requests.
        address_range_bottom : integer   := 0
    );
    port(
        --! The main system clock.
        clk             : in    std_logic;
        --! Asynchonous reset signal.
        reset           : in    std_logic;

        --! The lines which carry data and addresses;
        bus_lines       : inout std_logic_vector(data_width-1 downto 0);
        
        --! Used to assert data written by the bus master to bus_lines is valid.
        bus_valid       : in   std_logic;
        
        --! Used to assert that the slave has read the bus lines and they can be updated.
        bus_enable      : out    std_logic;
        
        --! High if this transaction is a write, low if it is a read.
        bus_read_write  : in   std_logic;

        --! Whether this is a read or write transaction the device must respond to.
        req_read_write  : out  std_logic;

        --! The address the bus transaction is targeting within the device.
        req_address_lines : out   std_logic_vector(data_width-1 downto 0);
        
        --! The data the transaction needs. Either read data is put onto this or write data is taken off it.
        req_data_lines  : inout std_logic_vector(data_width-1 downto 0);

        --! Tells the host device a bus request needs dealing with.
        req_pending     : out std_logic;

        --! Tells the controller the device has written/read all values from the address and data lines and can finish the request.
        req_done        : in std_logic
    );
end entity tim_bus_slave;
