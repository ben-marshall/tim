--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_slave_arch.vhdl
--! @brief Source code for the architecture declaration of the slave bus controller.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_master_state;



--! The synthesisable architecture of the tim bus slave controller.
architecture tim_bus_slave_rtl of tim_bus_slave is
    
    --! The current state of the controller.
    signal current_state    : tim_bus_master_state := BUS_RESET;
    --! The next state of the controller.
    signal next_state       : tim_bus_master_state := IDLE;

    --! Internal storage for the address of a transaction.
    signal internal_address_lines : std_logic_vector(data_width-1 downto 0);
    --! Internal storage for the data of a transaction.
    signal internal_data_lines    : std_logic_vector(data_width-1 downto 0);

begin

    --! Responsible for the synchronous state transitions and asynchronous resets.
    state_machine_progress : process (clk, reset)
    begin
        if(reset='1') then
            current_state   <= BUS_RESET;
        elsif(clk'event and clk = '1') then
            current_state   <= next_state;
        end if;
    end process state_machine_progress;
    
    --! Responsible for determining the next state of the statemachine.
    state_machine_next_state : process(clk,bus_valid, bus_lines, bus_read_write)
    begin
        
        case current_state is
            
            when BUS_RESET =>
                next_state <= IDLE;
            
            when IDLE =>
                if(bus_valid = '1' and 
                   to_integer(unsigned(bus_lines)) >= address_range_bottom and
                   to_integer(unsigned(bus_lines)) <= address_range_top) then
                    next_state <= REQ;
                else
                    next_state <= IDLE;
                end if;

            when REQ =>
                if(bus_read_write = '1') then
                    next_state <= WRITE;
                else
                    next_state <= READ;
                end if;
            
            when READ =>
                if(req_done = '1') then
                    next_state <= IDLE;
                else
                    next_state <= READ;
                end if;
            
            when WRITE =>
                if(req_done = '1') then
                    next_state <= IDLE;
                else
                    next_state <= READ;
                end if;
        
        end case;

    end process state_machine_next_state;


    --! Responsible for driving the bus outputs based on the current state of the controller.
    bus_outputs : process(current_state, req_data_lines)
    begin
        
        case current_state is
            
            when BUS_RESET =>
                bus_lines   <= (others => 'Z');
                bus_enable  <= 'Z';
                internal_address_lines <= (others => '0');
            
            when IDLE =>
                bus_lines   <= (others => 'Z');
                bus_enable  <= 'Z';
                internal_address_lines <= internal_address_lines;

            when REQ =>
                bus_lines   <= (others => 'Z');
                bus_enable  <= '1';
                internal_address_lines <= bus_lines;
            
            when READ =>
                bus_lines   <= req_data_lines;
                bus_enable  <= '0';
                internal_address_lines <= internal_address_lines;
            
            when WRITE =>
                bus_lines   <= (others => 'Z');
                bus_enable  <= '1';
                internal_address_lines <= internal_address_lines;
        
        end case;

    end process bus_outputs;

    --! Responsible for driving the bus device outputs based on the current state of the controller.
    device_outputs: process(current_state, bus_lines, internal_address_lines)
    begin
        
        case current_state is
            
            when BUS_RESET =>
                req_read_write      <= '0';
                req_address_lines   <= (others => '0');
                req_data_lines      <= (others => 'Z');
                req_pending         <= '0';
            
            when IDLE =>
                req_read_write      <= '0';
                req_address_lines   <= (others => '0');
                req_data_lines      <= (others => 'Z');
                req_pending         <= '0';

            when REQ =>
                req_read_write      <= bus_read_write;
                req_address_lines   <= internal_address_lines;
                req_data_lines      <= (others => 'Z');
                req_pending         <= '1';
            
            when READ =>
                req_read_write      <= bus_read_write;
                req_address_lines   <= internal_address_lines;
                req_data_lines      <= (others => 'Z');
                req_pending         <= '1';
            
            when WRITE =>
                req_read_write      <= bus_read_write;
                req_address_lines   <= internal_address_lines;
                req_data_lines      <= internal_data_lines;
                req_pending         <= '1';
        
        end case;

    end process device_outputs;

end architecture tim_bus_slave_rtl;
