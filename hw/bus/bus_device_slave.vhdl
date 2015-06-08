--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device_slave.vhdl
--! @brief Contains the architecture declaration of the slave bus device. This architecture belongs
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


--! slave bus controller architecture.
architecture slave of bus_device is

    --! Defines the state of the master bus device.
    type slave_state is (BUS_RESET, BUS_IDLE, BUS_READ, BUS_WRITE);

    --! Current state of the bus slave.
    signal current_state    : slave_state  := BUS_RESET;
    --! Next state of the bus slave.
    signal next_state       : slave_state  := BUS_IDLE;

    --! Is there a bus transaction pending for this device?
    signal bus_pending      : std_logic := '0';

begin

    --! The master device always controls the read/write enable line.
    req_write_enable    <=  bus_write_enable;
    --! The req_complete signal is always controlled by the masters parent device.
    req_complete        <= 'Z';
    --! The bus_address_lines signal is always controlled by the master.
    bus_address_lines   <= (others => 'Z');

    --! Responsible for advancing the current state of the bus slave.
    state_machine_progress  : process(clk, reset)
    begin
        if(reset = '1') then
            current_state <= BUS_RESET;
        elsif(clk = '1' and clk'event) then
            current_state <= next_state;
        end if;
    end process state_machine_progress;

    
    --! Responsible for updating the signal to tell if the current transaction is targeted
    --! At this device.
    address_response_logic  : process(bus_address_valid, bus_address_lines)
    begin

        if(bus_address_valid = '1')
            if(bus_address_lines <= address_top and bus_address_lines >= address_bottom) then
                bus_pending <= '1';
            else
                bus_pending <= '0';
            end if;
        else
            bus_pending <= '0';
        end if;

    end

    --! Responsible for determining the next state of the state machine.
    next_state_logic        : process(current_state, req_complete, bus_write_enable, 
                                      bus_pending, bus_address_valid)
    begin
        case (current_state) is
            
            when BUS_RESET  =>
                next_state <= BUS_IDLE;

            when BUS_IDLE   =>
                if(bus_pending = '1' and bus_write_enable = '1') then
                    next_state <= BUS_WRITE;
                elsif(bus_pending = '1' and bus_write_enable = '0') then
                    next_state <= BUS_READ;
                else
                    next_state <= BUS_IDLE;
                end if;

            when BUS_READ   =>
                if(req_complete = '1' and bus_address_valid = '0') then
                    next_state <= BUS_IDLE;
                else
                    next_state <= BUS_READ;
                end if;

            when BUS_WRITE  => 
                if(req_complete = '1' and bus_address_valid = '0') then
                    next_state <= BUS_IDLE;
                else
                    next_state <= BUS_READ;
                end if;
        
        end case;
    end process;


    --! Responsible for setting the correct IO signal levels for requestor and bus lines.
    signal_control           : process(current_state, next_state, req_complete, req_data_lines,
                                       bus_data_lines)
    begin
        case (current_state) is
            
            when BUS_RESET  =>
                bus_data_lines  <= (others => 'Z');
                bus_data_valid  <= 'Z';
                bus_enable      <= 'Z';
                req_data_lines  <= (others => 'Z');
                req_pending     <= '0';

            when BUS_IDLE   =>
                if(next_state = BUS_IDLE) then
                    bus_data_lines  <= (others => 'Z');
                    bus_data_valid  <= 'Z';
                    bus_enable      <= 'Z';
                    req_data_lines  <= (others => 'Z');
                    req_pending     <= '0';

                elsif(next_state = BUS_READ) then
                    bus_data_lines  <= req_data_lines;
                    bus_data_valid  <= req_complete;
                    bus_enable      <= req_complete;
                    req_data_lines  <= (others => 'Z');
                    req_pending     <= '1';

                elsif(next_state = BUS_WRITE) then
                    bus_data_lines  <= (others => 'Z');
                    bus_data_valid  <= req_complete;
                    bus_enable      <= req_complete;
                    req_data_lines  <= bus_data_lines;
                    req_pending     <= '1';

                end if;

            when BUS_READ   =>
                    bus_data_lines  <= req_data_lines;
                    bus_data_valid  <= req_complete;
                    bus_enable      <= req_complete;
                    req_data_lines  <= (others => 'Z');
                    req_pending     <= '1';

            when BUS_WRITE  => 
                    bus_data_lines  <= (others => 'Z');
                    bus_data_valid  <= req_complete;
                    bus_enable      <= req_complete;
                    req_data_lines  <= bus_data_lines;
                    req_pending     <= '1';
        
        end case;
    end process;


end architecture slave;
