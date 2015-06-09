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
    type fetch_state    is      (FETCH_RESET, IDLE, LOAD_WORD, FILL_BUFFER);
    --! Current state of the instruction fetcher.
    signal current_state        : fetch_state   := FETCH_RESET;
    --! Next state of the instruction_fetcher.
    signal next_state           : fetch_state   := IDLE;
    
    --! The size of the memory buffer in bits.
    constant buf_size           : integer := data_bus_width * 2;
    --! The instruction memory buffer that will store instructions to be decoded.
    signal instruction_buffer   : std_logic_vector(buf_size-1 downto 0);
    --! The new and updated value of the instruction buffer.
    signal next_buffer          : std_logic_vector(buf_size-1 downto 0) := (others => '0');
    --! The number of currently stored bytes in the buffer.
    signal stored_bytes         : integer := 0;
    --! The next value for stored bytes after the buffer is updated.
    signal next_stored_bytes    : integer := 0;

    --! Communicates with the bus controller to initiate an instruction request.
    signal  word_load_pending   : std_logic := '0';
    --! Tells the module an instruction has been fetched from memory.
    signal  word_load_complete  : std_logic := '0';
    --! The most recently fetched word from memory.
    signal  fetched_word        : std_logic_vector(data_bus_width-1, downto 0);

begin
    
    --! This module will never perform writes, so tie this line to zero.
    req_write_enable    <= '0';
    --! This module will always ask for the current value of the program counter.
    req_address_lines   <= program_counter;

    --! Responsible for advancing the current state of the fetcher and buffer.
    state_machine_progress  : process(clk, reset)
    begin
        if(reset = '1') then
            current_state       <= BUS_RESET;
            instruction_buffer  <= (others => '0');
        elsif(clk = '1' and clk'event) then
            current_state       <= next_state;
            instruction_buffer  <= next_buffer
        end if;
    end process state_machine_progress;


    --! Responsible for computing the next value of the fetch state machine.
    fetch_state_machine_next    : process(current_state, stored_bytes, word_load_complete)
    begin
        case(current_state) is

            when FETCH_RESET    =>
                next_state <= IDLE;

            when IDLE           =>
                if(stored_bytes <= 4) then
                    next_state <= LOAD_WORD;
                else
                    next_state <= IDLE;
                end if;

            when LOAD_WORD      =>
                if(word_load_complete = '1') then
                    next_state <= FILL_BUFFER;
                else
                    next_state <= IDLE;
                end if;

            when FILL_BUFFER    =>
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

            when IDLE           =>
               req_pending          <= '0';
               fetched_word         <= fetched_word;

            when LOAD_WORD      =>
               req_pending          <= '1';
               fetched_word         <= req_data_lines;

            when FILL_BUFFER    =>
               req_pending          <= '0';
               fetched_word         <= fetched_word;

        end case;

    end process fetch_io_control;

    --! Responsible for computing what the next value of the buffer should be.
    buffer_control              : process(current_state, instruction_recieved, instruction_buffer)
    begin
        if(instruction_recieved = '1') then
            
            case(decoded_instruction_size) is
                when 0 =>
                    next_buffer         <= instruction_buffer;
                    next_stored_bytes   <= stored_bytes;

                when 1 =>
                    next_buffer(buf_size-1 downto 8)    <= instruction_buffer(buf_size-8 downto 0);
                    next_buffer(7          downto 0)    <= instruction_buffer(7          downto 0);
                    next_stored_bytes   <= stored_bytes - 1;

                when 2 =>
                    next_buffer(buf_size-1 downto 16)    <= instruction_buffer(buf_size-16 downto 0);
                    next_buffer(15          downto 0)    <= instruction_buffer(15          downto 0);
                    next_stored_bytes   <= stored_bytes - 2;

                when 3 =>
                    next_buffer(buf_size-1 downto 24)    <= instruction_buffer(buf_size-24 downto 0);
                    next_buffer(23          downto 0)    <= instruction_buffer(23          downto 0);
                    next_stored_bytes   <= stored_bytes - 3;

                when 4 =>
                    next_buffer(buf_size-1 downto 32)    <= instruction_buffer(buf_size-32 downto 0);
                    next_buffer(31          downto 0)    <= instruction_buffer(31          downto 0);
                    next_stored_bytes   <= stored_bytes - 4;

                when others =>
                    report "Invalid instruction size!" severity failure;

            end case;

        else(current_state = FILL_BUFFER) then
                    
            next_stored_bytes   <= stored_bytes + 4;
            next_buffer(buf_size-1 downto buf_size-stored_bytes*8) <= instruction_buffer(buf_size-1 downto buf_size-stored_bytes*8)
            next_buffer(buf_size-stored_bytes*8 -1 downto buf_size-stored_bytes*16) <= fetched_word;

        else
            next_buffer         <= instruction_buffer;
            next_stored_bytes   <= stored_bytes;
        end if;

    end process buffer_control;

end architecture rtl;
