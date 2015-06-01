--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_master.vhdl
--! @brief Source code for the entity declaration of the master bus controller.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

use work.tim_bus.tim_bus_data_width;
use work.tim_bus.tim_bus_burst_width;

--! The bus master controller module which arbitrates bus requests and responses.
entity tim_bus_master is
    generic(
        --! The number of data and address lines.
        data_width  : integer := tim_bus_data_width
    );
    port(
        --! The lines which carry data and addresses;
        bus_lines       : inout std_logic_vector(data_width-1 downto 0);
        
        --! Used to assert data written by the bus master to bus_lines is valid.
        bus_valid       : out   std_logic;
        
        --! Used to assert that the slave has read the bus lines and they can be updated.
        bus_enable      : in    std_logic;
        
        --! High if this transaction is a write, low if it is a read.
        bus_read_write  : out   std_logic;

        --! The burst length of the current transaction where 0 is transacting a single data word.
        bus_burst       : out   std_logic_vector(tim_bus_burst_width-1 downto 0)
    );
end entity tim_bus_master;
