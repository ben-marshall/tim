--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_master.vhdl
--! @brief Source code for the entity declaration of the master bus controller.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;

--! The bus master controller module which arbitrates bus requests and responses.
entity tim_bus_master is
    generic(
        --! The number of data and address lines.
        data_width  : integer := tim_bus_data_width
    );
    port(
        --! The main system clock.
        clk             : in    std_logic;
        --! Asynchonous reset signal.
        reset           : in    std_logic;

        --! The lines which carry data and addresses;
        bus_lines       : inout std_logic_vector(data_width-1 downto 0);
        
        --! Used to assert data written by the bus master to bus_lines is valid.
        bus_valid       : out   std_logic;
        
        --! Used to assert that the slave has read the bus lines and they can be updated.
        bus_enable      : in    std_logic;
        
        --! High if this transaction is a write, low if it is a read.
        bus_read_write  : out   std_logic;

        --! Request write data is placed on these lines.
        req_data_lines      : inout std_logic_vector(data_width-1 downto 0);

        --! Tells the controller if this is a read or write transaction. High == write, low == read.
        req_read_write      : in    std_logic;

        --! Addresses for read and write operations are placed on these lines.
        req_address_lines   : in    std_logic_vector(data_width-1 downto 0);

        --! This is put high to tell the controller a request is pending.
        req_pending         : in    std_logic;

        --! This is put high to tell the requestor that the request has been accepted. When it goes low again,
        --! This signals that the response is available
        req_acknowledge     : out   std_logic
    );
end entity tim_bus_master;
