--! ------------------------------------------------------------------------------------------------
--!
--! @file mem_dummy.vhdl
--! @brief Source code for a dummy memory module used in simulations.
--!
--! ------------------------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_master_state;

--! A dummy RAM memory module to be used for simulations.
entity mem_dummy is
    generic(
        --! The size of the memory in words.
        mem_size             : integer   := 1023;

        --! The width in bits of the data bus.
        data_width           : integer   := tim_bus_data_width;
        
        --! The top of the range of addresses to which this memory block will respond to requests.
        address_range_top    : integer   := 1023;

        --! The bottomof the range of addresses to which this memory block will respond to requests.
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
        bus_read_write  : in   std_logic
    );
end entity mem_dummy;


--! The architecture of the dummy simulation memory block.
architecture mem_dummy_sim of mem_dummy is

    --
    -- Component declarations
    --
    
    --! The bus slave controller module which responds to requests from the master.
    component tim_bus_slave is
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
    end component tim_bus_slave;

    --
    -- Internal signals.
    --

    --! Whether or not the memory controller is responding to a read (low) or write (high).
    signal request_read_write      : std_logic;
    --! THe address of the memory request.
    signal request_address_lines   : std_logic_vector(data_width-1 downto 0);
    --! The write data of the memory request.
    signal request_data_lines      : std_logic_vector(data_width-1 downto 0);
    --! High if the data, address and read_write request lines are valid and should be responded too.
    signal request_pending         : std_logic;
    --! Used to signal to the memory controller than the request has been dealt with and it can read back requst_data_lines.
    signal request_done            : std_logic;

    type memory_state is (MEM_RESET, IDLE, READ, WRITE);

    --! Current state of the dummy memory.
    signal current_state        : memory_state := MEM_RESET;
    --! Next state of the dummy memory.
    signal next_state           : memory_state := IDLE;

    --! Datatype that pretends to be an array of memory words.
    type memory_array is array (0 to mem_size) of std_logic_vector(data_width-1 downto 0);

    --! The memory array variable that stores all of the values.
    shared variable memory               : memory_array;

begin

    --! Responsible for the synchronous state transitions and asynchronous resets.
    state_machine_progress : process (clk, reset)
    begin
        if(reset='1') then
            current_state   <= MEM_RESET;
        elsif(clk'event and clk = '1') then
            current_state   <= next_state;
        end if;
    end process state_machine_progress;

    --! Responsible for assigning the next state of the memory module.
    state_machine_next_state : process(clk, request_pending, request_read_write)
    begin
        case current_state is
            when MEM_RESET => 
                next_state <= IDLE;
            
            when IDLE     =>
                if(request_pending <= '0' and request_read_write = '0') then
                    next_state <= READ;
                elsif(request_pending <= '0' and request_read_write = '0') then
                    next_state <= WRITE;
                else
                    next_state <= IDLE;
                end if;

            when READ =>
                next_state <= IDLE;
            
            when WRITE =>
                next_state <= IDLE;

        end case;
    end process state_machine_next_state;

    --! Responsible for reading and writing the requestor interface to the memory controller and memory array.
    requestor_values    : process(current_state, clk, request_pending, request_read_write)
    begin

        case current_state is
            when MEM_RESET => 
               request_data_lines   <= (others => 'Z');
               request_done         <= '0';
            
            when IDLE     =>
               request_data_lines   <= (others => 'Z');
               request_done         <= '0';

            when READ =>
               request_data_lines   <= memory(to_integer(unsigned(request_address_lines)));
               request_done         <= '1';
            
            when WRITE =>
               memory(to_integer(unsigned(request_address_lines))) := request_data_lines;
               request_done         <= '1';

        end case;

    end process requestor_values;


    --! The instance of the bus controller that will interface between the system bus and the memory controller.
    bus_controller  : tim_bus_slave generic map(
        data_width              => tim_bus_data_width,
        address_range_top       => address_range_top,
        address_range_bottom    => address_range_bottom
    ) port map(
        clk                 =>  clk,
        reset               =>  reset,
        bus_lines           =>  bus_lines ,     
        bus_valid           =>  bus_valid,      
        bus_enable          =>  bus_enable,     
        bus_read_write      =>  bus_read_write,
        req_read_write      =>  request_read_write,
        req_address_lines   =>  request_address_lines,
        req_data_lines      =>  request_data_lines,
        req_pending         =>  request_pending,  
        req_done            =>  request_done         

    );


end architecture mem_dummy_sim;