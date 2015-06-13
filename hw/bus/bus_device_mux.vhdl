--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device_mux.vhdl
--! @brief Acts as a multiplexer for bus controllers. Allows multiple modules to use the same
--!        bus device master controller using an interrupt system. No extra logic within
--!        the bus controller, or the module it communicates with, is neded to attatch it to this
--!        multiplexer. It is a simple dropin replacement.
--!
--! ------------------------------------------------------------------------------------------------

--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! import the tim_common package contents.
use work.tim_common.all;

--! Allows multiple hardware modules to connect to the same bus controller with no extra logic on
--! their part. Each device connects to the mux through a port. Each port is numbered, with lower
--! ports given a higher requestor priority.
entity  bus_device_mux  is
    generic(
        --! The number of address bits on the bus.
        address_width   : integer   := memory_word_width;
        --! The number of data bits on the bus.
        data_width      : integer   := memory_word_width;
        --! The bottom of the address range to which this device will respond.
        address_bottom  : unsigned  := to_unsigned(0, memory_word_width);
        --! The top of the address range to which this device will respond.
        address_top     : unsigned  := to_unsigned(0, memory_word_width);
        --! The number of devices that will be multiplexed onto this bus.
        users           : integer   := 2
    );
    port(
        --! The main system clock.
        clk                 : in    std_logic;
        --! System reset signal.
        reset               : in    std_logic;

        --! The current address of the thing being accessed on the bus.
        bus_address_lines   : inout unsigned(address_width-1 downto 0);
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
        
        -- Need to make these signals into arrays.

        --! The current address of the thing being accessed on the bus.
        req_address_lines   : inout bus_mux_address_lines(users-1 downto 0);
        --! The data being carried on the bus.
        req_data_lines      : inout bus_mux_data_lines(users-1 downto 0);
        --! Signal to tell the controller/device that a request is pending and needs attention.
        req_pending         : inout bus_mux_pending(users-1 downto 0);
        --! Asserted high to tell the device/bus controller that the request has been completed and that the response
        --! is available on the data lines.
        req_complete        : inout bus_mux_complete(users-1 downto 0);
        --! high = write, low = read operation.
        req_write_enable    : inout bus_mux_write_enable(users-1 downto 0)
    );
end entity bus_device_mux;
