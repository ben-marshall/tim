--! ------------------------------------------------------------------------------------------------
--!
--! @file  bus_device_mux_arch.vhdl
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

--! Use the width of a single memory word as the default width of the bus.
use work.tim_common.memory_word_width;


--! Architecture of the bus controller multiplexer
architecture rtl of bus_device_mux is

    --! The current controller using the bus.
    signal current_user     : integer   := 0;

begin

    --! The master device always controls the read/write enable line.
    bus_write_enable <= req_write_enable(current_user);
    --! The bus_enable signal is always controlled by the current slave device.
    bus_enable       <= 'Z';

    --! Responsible for asigning the current controller of the bus. devices on lower port numbers have
    --! Priority.
    update_current_user : process(current_user, req_pending)
    begin

        if(req_pending'event and req_pending(current_user) = '0') then

            for i in users-1 to 0   loop
                if(req_pending(i) = '1') then
                    current_user <= i;
                end if;
            end loop;

        end if;

    end process;

    --! Responsibe for driving bus values through the MUX.
    update_outputs      : process(current_user, bus_data_lines, bus_data_valid, bus_enable,
                                  req_address_lines, req_data_lines, req_pending, req_write_enable)
    begin

        for i in users-1 downto 0 loop
        
            --! The req_pending signal is always controlled by the masters parent device.
            req_pending(i)      <= 'Z';
            --! Master always reads from request address lines.
            req_address_lines(i)   <= (others => 'Z');
            --! Master always reads from write_enable lines.
            req_write_enable(i)    <= 'Z';

            if(i = current_user) then

                bus_address_lines   <= req_address_lines(i);
                bus_address_valid   <= req_pending(i);
                req_complete     (i)   <= bus_enable;

                if(req_write_enable(i) = '1') then
                    bus_data_lines      <= req_data_lines(i);
                    req_data_lines   (i)   <= (others => 'Z');
                else
                    bus_data_lines      <= (others => 'Z');
                    req_data_lines   (i)   <= bus_data_lines;
                end if;

            else

                req_complete     (i)   <= '0';
                req_data_lines   (i)   <= req_data_lines(i);

            end if;

        end loop;

    end process update_outputs;


end architecture rtl;
