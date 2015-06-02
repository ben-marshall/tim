--! ------------------------------------------------------------------------------------------------
--!
--! @file tb_bus.vhdl
--! @brief Testbench for the bus modules.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_burst_width;

--! Empty entity declaration of the bus testbench.
entity tb_bus is
end entity tb_bus;


--! Bus testbench architecture.
architecture testbench of tb_bus is


--! The bus master controller module which arbitrates bus requests and responses.
component tim_bus_master is
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
        --! The read response data.
        req_read_write      : in    std_logic;
        --! Addresses for read and write operations are placed on these lines.
        req_address_lines   : in    std_logic_vector(data_width-1 downto 0);
        --! This is put high to tell the controller a request is pending.
        req_pending         : in    std_logic;
        --! This is put high to tell the requestor that the response is valid and its transaction is complete.
        req_acknowledge     : out   std_logic
    );
end component tim_bus_master;


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

    --! Testbench clock signal.
    signal tb_clk   : std_logic := '0';
    --! Testbench reset signal.
    signal tb_reset : std_logic := '1';

    --
    -- These are all the system bus signals.
    --
    signal bus_lines       : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => 'Z');
    signal bus_valid       : std_logic := '0';
    signal bus_enable      : std_logic := '0';
    signal bus_read_write  : std_logic := '0';


    --
    -- These are the master device signals.
    -- 

    signal master_data          : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => '1');
    signal master_address       : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => '0');
    signal master_read_write    : std_logic := '0';
    signal master_pending       : std_logic := '0';
    signal master_acknowledge   : std_logic := '0';

    --
    -- These are the slave device signals.
    --

    signal slave_0_read_write   : std_logic := '0';
    signal slave_0_address      : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => 'Z');
    signal slave_0_data         : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => '0');
    signal slave_0_pending      : std_logic := '0';
    signal slave_0_done         : std_logic := '0';

    signal slave_1_read_write   : std_logic := '0';
    signal slave_1_address      : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => 'Z');
    signal slave_1_data         : std_logic_vector(tim_bus_data_width-1 downto 0) := (others => '1');
    signal slave_1_pending      : std_logic := '0';
    signal slave_1_done         : std_logic := '0';

    --
    -- Testbench signals.
    --
    
begin
    
    --
    -- Continuous assignments
    --

    tb_clk  <= not tb_clk after 20 ns;
    tb_reset <= '0' after 50 ns;

    --! Responsible for stimulating the master bus controller.
    master_control  : process(tb_clk, tb_reset, master_acknowledge)
    begin
        
        if(tb_reset = '1') then
            
            master_pending <= '0';

        elsif(tb_clk'event and master_acknowledge = '0' and master_pending = '0') then
            
            master_pending <= '1';
            master_address <= std_logic_vector(unsigned(master_address)+128);

        elsif(tb_clk'event and master_acknowledge = '1') then

            master_pending <= '0';

        end if;

    end process master_control;

    --! Responsible for stimulating the master bus controller.
    slave_0_control : process(tb_clk, tb_reset, slave_0_pending)
    begin
        
        if(tb_reset = '1') then
            
            slave_0_done <= '0';

        elsif(tb_clk'event and slave_0_pending = '0') then

            slave_0_done <= '0';

        elsif(tb_clk'event and slave_0_pending = '1') then

            if(slave_0_read_write = '0') then
                slave_0_data <= slave_0_address;
            end if;

            slave_0_done <= '1';

        end if;

    end process slave_0_control;

    --! Responsible for stimulating the master bus controller.
    slave_1_control : process(tb_clk, tb_reset, slave_1_pending)
    begin
        
        if(tb_reset = '1') then
            
            slave_1_done <= '0';

        elsif(tb_clk'event and slave_1_pending = '0') then
            
            slave_1_done <= '0';

        elsif(tb_clk'event and slave_1_pending = '1') then

            if(slave_1_read_write = '0') then
                slave_1_data <= slave_1_address;
            end if;

            slave_1_done <= '1';

        end if;

    end process slave_1_control;


    --
    -- Entity declarations.
    --

    --! The bus master object.
    entity_master : tim_bus_master port map(
        clk     => tb_clk,
        reset   => tb_reset,

        bus_lines   => bus_lines,
        bus_valid   => bus_valid,
        bus_enable  => bus_enable,
        bus_read_write => bus_read_write,

        req_data_lines    =>  master_data,
        req_address_lines =>  master_address,
        req_read_write    =>  master_read_write,
        req_pending       =>  master_pending,
        req_acknowledge   =>  master_acknowledge
    );

    --! The First bus device.
    entity_slave_0 : tim_bus_slave generic map(
        address_range_top       => 1023,
        address_range_bottom    => 0
    )
    port map(
        clk     => tb_clk,
        reset   => tb_reset,

        bus_lines   => bus_lines,
        bus_valid   => bus_valid,
        bus_enable  => bus_enable,
        bus_read_write => bus_read_write,
        
        req_read_write    =>  slave_0_read_write,
        req_address_lines =>  slave_0_address,
        req_data_lines    =>  slave_0_data,
        req_pending       =>  slave_0_pending,
        req_done          =>  slave_0_done
    );
    
    --! The second bus device.
    entity_slave_1 : tim_bus_slave generic map(
        address_range_top       => 2047,
        address_range_bottom    => 1024
    )
    port map(
        clk     => tb_clk,
        reset   => tb_reset,

        bus_lines   => bus_lines,
        bus_valid   => bus_valid,
        bus_enable  => bus_enable,
        bus_read_write => bus_read_write,
        
        req_read_write    =>  slave_1_read_write,
        req_address_lines =>  slave_1_address,
        req_data_lines    =>  slave_1_data,
        req_pending       =>  slave_1_pending,
        req_done          =>  slave_1_done
    );

end architecture testbench;


