--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_fetch_decode_arch.vhdl
--! @brief Contains the architecture declaration/defintion for the fetch & decode module.
--!
--! ------------------------------------------------------------------------------------------------

--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! Imported from tim_common package.
use work.tim_common.data_bus_width;

--! Architecture for the instruction fetch and decode module.
architecture rtl of tim_cpu_fetch_decode is

    --! The state of the instruction fetcher.
    type fetch_state    is      (FETCH_RESET, IDLE, LOAD_WORD, FILL_BUFFER, EMPTY_BUFFER);
    --! Current state of the instruction fetcher.
    signal current_state        : fetch_state   := FETCH_RESET;
    --! Next state of the instruction_fetcher.
    signal next_state           : fetch_state   := IDLE;
    
    signal stored_bytes         : integer   := 0;

    --! Tells the module an instruction has been fetched from memory.
    signal  word_load_complete  : std_logic := '0';
    --! The most recently fetched word from memory.
    signal  fetched_word        : std_logic_vector(data_bus_width-1 downto 0);

begin
    
    --! This module will never perform writes, so tie this line to zero.
    req_write_enable    <= '0';
    --! This module will always ask for the current value of the program counter.
    req_address_lines   <= program_counter;

    word_load_complete  <= req_complete;

    decoded_instruction_size    <= 4;

    --! Responsible for advancing the current state of the fetcher and buffer.
    state_machine_progress  : process(clk, reset)
    begin
        if(reset = '1') then
            current_state       <= FETCH_RESET;
        elsif(clk = '1' and clk'event) then
            current_state       <= next_state;
        end if;
    end process state_machine_progress;


    --! Responsible for computing the next value of the fetch state machine.
    fetch_state_machine_next    : process(current_state, stored_bytes, word_load_complete)
    begin
        case(current_state) is

            when FETCH_RESET    =>
                next_state <= LOAD_WORD;

            when IDLE           =>
                if(stored_bytes <= 4) then
                    next_state <= LOAD_WORD;
                elsif(instruction_recieved = '1') then
                    next_state <= EMPTY_BUFFER;
                else
                    next_state <= IDLE;
                end if;

            when LOAD_WORD      =>
                if(word_load_complete = '1') then
                    next_state <= FILL_BUFFER;
                else
                    next_state <= LOAD_WORD;
                end if;

            when FILL_BUFFER    =>
                next_state <= IDLE;
            
            when EMPTY_BUFFER   =>
                next_state <= IDLE;

        end case;
    end process;

    --! Responsible for controlling the IO signals for fetching a new word from memory.
    fetch_io_control            : process(current_state, req_data_lines)
    begin

        case(current_state) is

            when FETCH_RESET    =>
               req_pending          <= '0';
               fetched_word         <= (others => '0');
               instruction_valid    <= '0';

            when IDLE           =>
               req_pending          <= '0';
               fetched_word         <= fetched_word;
               instruction_valid    <= '1';

            when LOAD_WORD      =>
               req_pending          <= '1';
               fetched_word         <= req_data_lines;
               instruction_valid    <= '0';

            when FILL_BUFFER    =>
               req_pending          <= '0';
               fetched_word         <= fetched_word;
               instruction_valid    <= '0';

            when EMPTY_BUFFER    =>
               req_pending          <= '0';
               fetched_word         <= fetched_word;
               instruction_valid    <= '0';

        end case;

    end process fetch_io_control;


end architecture rtl;
