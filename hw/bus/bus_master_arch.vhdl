--! ------------------------------------------------------------------------------------------------
--!
--! @file bus_master_arch.vhdl
--! @brief Source code for the architecture declaration of the master bus controller.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_bus package,
use work.tim_bus.tim_bus_burst_width;

--! RTL & synthesisable architecture of the bus master controller.
architecture tim_bus_master_rtl of tim_bus_master is

    --! The current state of the controller.
    signal current_state    : tim_bus_master_state := RESET;
    --! The next state of the controller.
    signal next_state       : tim_bus_master_state := IDLE;

begin

    --! Responsible for the synchronous state transitions and asynchronous resets.
    state_machine_progress : process (clk, reset)
    begin
        if(reset='1') then
            current_state   <= RESET;
        elsif(clk'event and clk = '1') then
            current_state   <= next_state;
        end if;
    end process state_machine_progress;


    --! Responsible for determining the next state of the statemachine.
    state_machine_next_state : process(bus_valid, bus_enable, req_read_write, req_pending)
    begin
        
        case current_state is
            
            when RESET =>
                next_state <= IDLE;
            
            when IDLE =>
                if(req_pending = '1') then
                    next_state <= REQ;
                else
                    next_state <= IDLE;
                end if;

            when REQ =>
                if(bus_enable = '1' and req_read_write = '0') then
                    next_state <= READ;
                elsif(bus_enable = '1' and req_read_write = '1') then
                    next_state <= WRITE;
                else
                    next_state <= REQ;
                end if;
            
            when READ =>
                if(bus_enable = '0') then
                    next_state <= IDLE;
                else
                    next_state <= READ;
                end if;
            
            when WRITE =>
                if(bus_enable = '0') then
                    next_state <= IDLE;
                else
                    next_state <= READ;
                end if;
        
        end case;

    end process state_machine_next_state;


    --! Responsible for driving the bus outputs based on the current state of the controller.
    bus_outputs : process(current_state, req_data_lines, req_address_lines, req_read_write)

    begin
        
        case current_state is
            
            when RESET =>
                bus_lines       <= (others => 'Z');
                bus_valid       <= '0';
                bus_read_write  <= '0';
            
            when IDLE =>
                bus_lines       <= (others => 'Z');
                bus_valid       <= '0';
                bus_read_write  <= '0';

            when REQ =>
                bus_lines       <= req_address_lines;
                bus_valid       <= '1';
                bus_read_write  <= req_read_write;
            
            when READ =>
                bus_lines       <= 'Z';
                bus_valid       <= '1';
                bus_read_write  <= req_read_write;
            
            when WRITE =>
                bus_lines       <= 'req_data_lines';
                bus_valid       <= '0';
                bus_read_write  <= req_read_write;
        
        end case;

    end process bus_outputs;

    --! Responsible for driving the requestor outputs based on the current state of the controller.
    requestor_outputs: process(current_state, bus_lines)

    begin
        
        case current_state is
            
            when RESET =>
               req_data_lines   <= (others => 'Z'); 
               req_acknowledge  <= '0';
            
            when IDLE =>
               req_data_lines   <= (others => 'Z'); 
               req_acknowledge  <= '0';

            when REQ =>
               req_data_lines   <= (others => 'Z'); 
               req_acknowledge  <= '1';
            
            when READ =>
               req_data_lines   <= bus_lines;
               req_acknowledge  <= '1';
            
            when WRITE =>
               req_data_lines   <= (others => 'Z'); 
               req_acknowledge  <= '1';
        
        end case;

    end process bus_outputs;

end architecture tim_bus_master_rtl;
