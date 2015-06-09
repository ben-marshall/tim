--! ------------------------------------------------------------------------------------------------
--!
--! @file  mem_bus_bram.vhdl
--! @brief Contains the entity and architecture declarations for the module connecting a bram
--!        module to the memory bus.
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

--! A bram memory module connected to the system bus.
entity  mem_bus_bram    is
    generic(
        --! The bottom of the address range to which this device will respond.
        address_bottom  : unsigned  := to_unsigned(0, memory_word_width);
        --! The top of the address range to which this device will respond.
        address_top     : unsigned  := to_unsigned(0, memory_word_width)
    );
    port(
        --! The main system clock.
        clk                 : in    std_logic;
        --! System reset signal.
        reset               : in    std_logic;

        --! The current address of the thing being accessed on the bus.
        bus_address_lines   : inout unsigned(31 downto 0);
        --! The data being carried on the bus.
        bus_data_lines      : inout std_logic_vector(31 downto 0);
        --! Signal to tell the rest of the bus that the address lines are valid, initiating a transaction.
        bus_address_valid   : inout std_logic;
        --! Signal to tell the rest of the bus that the data lines are valid.
        bus_data_valid      : inout std_logic;
        --! Enable signal to let slaves tell the master they are finished with the data on the bus.
        bus_enable          : inout std_logic;
        --! high = write, low = read operation.
        bus_write_enable    : inout std_logic
    );
end entity mem_bus_bram;

--! Architecture of the bram memory bus module.
architecture rtl of mem_bus_bram is

    --! Forward declaration of the generated BRAM controller.
    COMPONENT mem_bram
      PORT (
        clka : IN STD_LOGIC;
        ena : IN STD_LOGIC;
        wea : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
        addra : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        dina : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        douta : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
      );
    END COMPONENT;
    
    signal internal_address_lines : unsigned(31 downto 0);
    signal internal_data_lines    : std_logic_vector(31 downto 0);
    signal internal_write_data_lines    : std_logic_vector(31 downto 0);
    signal internal_read_data_lines    : std_logic_vector(31 downto 0);
    signal internal_pending       : std_logic := '0';
    signal internal_complete      : std_logic := '0';
    signal internal_write_enable  : std_logic := '0';

    signal write_enable_vector    : std_logic_vector(3 downto 0);

    type mem_bram_state is (BRAM_RESET, BRAM_IDLE, BRAM_DONE);

    signal current_state    : mem_bram_state := BRAM_RESET;
    signal next_state       : mem_bram_state := BRAM_IDLE;

begin
    --! Expand the write enable to fill the internal write vector.
    write_enable_vector <= (others => internal_write_enable);

    --! Handles progression from one state to the next, along with async reset.
    state_progresssion  : process(clk, reset)
    begin
        if(reset = '1') then
            current_state <= BRAM_RESET;
        elsif(clk = '1' and clk'event) then
            current_state <= next_state;
        end if;
    end process;

    read_write_connect  : process(internal_write_data_lines, internal_read_data_lines, internal_write_enable)
    begin
        
        if(internal_write_enable = '1') then
            internal_data_lines <= internal_write_data_lines;
        else
            internal_data_lines <= internal_read_data_lines;
        end if;
        
    end process read_write_connect;

    --! Handles next state logic and request complete signal.
    state_logic :   process(current_state, internal_pending)
    begin
        case(current_state) is
            when BRAM_RESET =>
                next_state <= BRAM_IDLE;
                internal_complete <= '0';

            when BRAM_IDLE =>
                internal_complete <= '0';
                if(internal_pending = '1') then
                    next_state <= BRAM_DONE;
                else
                    next_state <= BRAM_IDLE;
                end if;

            when BRAM_DONE =>
                next_state <= BRAM_IDLE;
                internal_complete <= '1';

        end case;
    end process state_logic;


    --! An instance of the generated BRAM controller.
    bram_instance : mem_bram
    port map (
      clka  => clk,
      ena   => internal_pending,
      wea   => write_enable_vector,
      addra => std_logic_vector(internal_address_lines),
      dina  => internal_write_data_lines,
      douta => internal_read_data_lines
    );

    --! An instance of a bus slave device.
    slave_device   : entity work.bus_device(slave)
    generic map(
        address_width   => 32,
        data_width      => 32,
        address_bottom  => address_bottom,
        address_top     => address_top
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
        req_address_lines => internal_address_lines,
        req_data_lines    => internal_data_lines,
        req_pending       => internal_pending,
        req_complete      => internal_complete,
        req_write_enable  => internal_write_enable
    );


end architecture rtl;
