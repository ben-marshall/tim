--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device_master.vhdl
--! @brief Contains the architecture declaration of the master bus device. This architecture belongs
--!        to the bus_device entity.
--! @todo  Write this architecture definition.
--!
--! ------------------------------------------------------------------------------------------------


--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;


--! Master bus controller architecture.
architecture master of bus_device is

    --! Defines the state of the master bus device.
    type master_state is (BUS_RESET, BUS_IDLE, BUS_READ, BUS_WRITE);

    --! Current state of the master bus device.
    signal current_state    : master_state  := BUS_RESET;
    --! Next state of the master bus device.
    signal next_state       : master_state  := BUS_IDLE;

begin

    --! The master device always controls the read/write enable line.
    bus_write_enable <= req_write_enable;
    --! The bus_enable signal is always controlled by the current slave device.
    bus_enable       <= 'Z';
    --! The req_pending signal is always controlled by the masters parent device.
    req_pending      <= 'Z';

    --! Responsible for advancing the current state of the bus master.
    state_machine_progress  : process(clk, reset)
    begin
        if(reset = '1') then
            current_state <= BUS_RESET;
        elsif(clk = '1' and clk'event) then
            current_state <= next_state;
        end if;
    end process state_machine_progress;


    --! Responsible for determining the next state of the state machine.
    next_state_logic        : process(current_state, bus_enable, req_pending, bus_data_valid,
                                      req_write_enable)
    begin
        case (current_state) is
            
            when BUS_RESET  =>
                next_state <= BUS_IDLE;

            when BUS_IDLE   =>
                if(req_pending = '1' and req_write_enable = '1') then
                    next_state <= BUS_WRITE;
                elsif(req_pending = '1' and req_write_enable = '0') then
                    next_state <= BUS_READ;
                else
                    next_state <= BUS_IDLE;
                end if;

            when BUS_READ   =>
                if(bus_data_valid = '1' and bus_enable='1' and req_pending='0') then
                    next_state <= BUS_IDLE;
                else
                    next_state <= BUS_READ;
                end if;

            when BUS_WRITE  => 
                if(bus_enable = '1' and req_pending = '0') then
                    next_state <= BUS_IDLE;
                else
                    next_state <= BUS_WRITE;
                end if;
        
        end case;
    end process;


    --! Responsible for setting the correct IO signal levels for requestor and bus lines.
    signal_control           : process(current_state, next_state)
    begin
        case (current_state) is
            
            when BUS_RESET  =>
                bus_address_valid   <= '0';
                bus_data_valid      <= '0';
                req_complete        <= '0';
                bus_address_lines   <= (others => 'Z');
                bus_data_lines      <= (others => 'Z');

            when BUS_IDLE   =>
                if(next_state = BUS_IDLE) then
                    bus_address_valid   <= '0';
                    bus_data_valid      <= '0';
                    req_complete        <= '0';
                    bus_address_lines   <= (others => 'Z');
                    bus_data_lines      <= (others => 'Z');

                elsif(next_state = BUS_READ) then
                    bus_address_valid   <= '1';
                    bus_data_valid      <= 'Z';
                    req_complete        <= '0';
                    bus_address_lines   <= req_address_lines
                    bus_data_lines      <= (others => 'Z');

                elsif(next_state = BUS_WRITE) then
                    bus_address_valid   <= '1';
                    bus_data_valid      <= '1';
                    req_complete        <= '0';
                    bus_address_lines   <= req_address_lines;
                    bus_data_lines      <= req_data_lines;

                end if;

            when BUS_READ   =>
                bus_address_valid   <= '1';
                bus_data_valid      <= 'Z';
                bus_address_lines   <= req_address_lines;
                bus_data_lines      <= (others => 'Z');
                req_data_lines      <= bus_data_lines;
                req_complete        <= bus_enable;

            when BUS_WRITE  => 
                bus_address_valid   <= '1';
                bus_data_valid      <= '1';
                bus_address_lines   <= req_address_lines;
                bus_data_lines      <= req_data_lines;
                req_complete        <= bus_enable;
        
        end case;
    end process;


end architecture master;
