--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device.vhdl
--! @brief Contains the entity declaration for the bus device module. This module is responsible for
--!        Arbitrating all bus communication and providing a standard interface between a device
--!        connected to the bus and the bus itself.
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

--! All devices which connect to the system bus will use this entity. Any such device is either
--! A bus master or bus slave. This is determined by the architecture of the bus_device entity that
--! they use.
entity  bus_device  is
    generic(
        --! The number of address bits on the bus.
        address_width   : integer   := memory_word_width;
        --! The number of data bits on the bus.
        data_width      : integer   := memory_word_width;
        --! The bottom of the address range to which this device will respond.
        address_bottom  : unsigned  := to_unsigned(0);
        --! The top of the address range to which this device will respond.
        address_top     : unsigned  := to_unsigned(0)
    );
    port(
        --! The main system clock.
        clk                 : in    std_logic;
        --! System reset signal.
        reset               : in    std_logic;

        --! The current address of the thing being accessed on the bus.
        bus_address_lines   : inout std_logic_vector(address_width-1 downto 0);
        --! The data being carried on the bus.
        bus_data_lines      : inout std_logic_vector(data_width-1 downto 0);
        --! Signal to tell the rest of the bus that the address lines are valid, initiating a transaction.
        bus_address_valid   : inout std_logic;
        --! Signal to tell the rest of the bus that the data lines are valid.
        bus_data_valid      : inout std_logic;
        --! Enable signal to let slaves tell the master they are finished with the data on the bus.
        bus_enable          : inout std_logic;
        --! high = write, low = read operation.
        bus_write_enable    : inout std_logic;
        
        --! The current address of the thing being accessed on the bus.
        req_address_lines   : inout std_logic_vector(address_width-1 downto 0);
        --! The data being carried on the bus.
        req_data_lines      : inout std_logic_vector(data_width-1 downto 0);
        --! Signal to tell the controller/device that a request is pending and needs attention.
        req_pending         : inout std_logic;
        --! Asserted high to tell the device/bus controller that the request has been completed and that the response
        --! is available on the data lines.
        req_complete        : inout std_logic;
        --! high = write, low = read operation.
        req_write_enable    : inout std_logic
    );
end entity bus_device;
