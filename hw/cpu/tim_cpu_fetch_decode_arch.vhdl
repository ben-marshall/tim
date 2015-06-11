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
use work.tim_instructions.all;

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


    signal  current_decode      : std_logic_vector(opcode_width-1 downto 0) := (others => '0');

begin
    
    --! This module will never perform writes, so tie this line to zero.
    req_write_enable    <= '0';
    --! This module will always ask for the current value of the program counter.
    req_address_lines   <= program_counter;

    word_load_complete  <= req_complete;

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

    --! Responsible for decoding the current length of an instruction.
    instruction_length_decode : process(current_decode, clk)
    begin
    if(clk = '1' and clk'event) then
    case (current_decode) is
        when opcode_LOADR =>decoded_instruction<=LOADR;decoded_instruction_size<=opcode_width_LOADR;
        when opcode_LOADI =>decoded_instruction<=LOADI;decoded_instruction_size<=opcode_width_LOADI;
        when opcode_STORI =>decoded_instruction<=STORI;decoded_instruction_size<=opcode_width_STORI;
        when opcode_STORR =>decoded_instruction<=STORR;decoded_instruction_size<=opcode_width_STORR;
        when opcode_PUSH  =>decoded_instruction<=PUSH ;decoded_instruction_size<=opcode_width_PUSH ;
        when opcode_POP   =>decoded_instruction<=POP  ;decoded_instruction_size<=opcode_width_POP  ;
        when opcode_MOVR  =>decoded_instruction<=MOVR ;decoded_instruction_size<=opcode_width_MOVR ;
        when opcode_MOVI  =>decoded_instruction<=MOVI ;decoded_instruction_size<=opcode_width_MOVI ;
        when opcode_JUMPR =>decoded_instruction<=JUMPR;decoded_instruction_size<=opcode_width_JUMPR;
        when opcode_JUMPI =>decoded_instruction<=JUMPI;decoded_instruction_size<=opcode_width_JUMPI;
        when opcode_CALLR =>decoded_instruction<=CALLR;decoded_instruction_size<=opcode_width_CALLR;
        when opcode_CALLI =>decoded_instruction<=CALLI;decoded_instruction_size<=opcode_width_CALLI;
        when opcode_RETURN=>decoded_instruction<=RET  ;decoded_instruction_size<=opcode_width_RETURN;
        when opcode_TEST  =>decoded_instruction<=TEST ;decoded_instruction_size<=opcode_width_TEST ;
        when opcode_HALT  =>decoded_instruction<=HALT ;decoded_instruction_size<=opcode_width_HALT ;
        when opcode_ANDR  =>decoded_instruction<=ANDR ;decoded_instruction_size<=opcode_width_ANDR ;
        when opcode_NANDR =>decoded_instruction<=NANDR;decoded_instruction_size<=opcode_width_NANDR;
        when opcode_ORR   =>decoded_instruction<=ORR  ;decoded_instruction_size<=opcode_width_ORR  ;
        when opcode_NORR  =>decoded_instruction<=NORR ;decoded_instruction_size<=opcode_width_NORR ;
        when opcode_XORR  =>decoded_instruction<=XORR ;decoded_instruction_size<=opcode_width_XORR ;
        when opcode_LSLR  =>decoded_instruction<=LSLR ;decoded_instruction_size<=opcode_width_LSLR ;
        when opcode_LSRR  =>decoded_instruction<=LSRR ;decoded_instruction_size<=opcode_width_LSRR ;
        when opcode_NOTR  =>decoded_instruction<=NOTR ;decoded_instruction_size<=opcode_width_NOTR ;
        when opcode_ANDI  =>decoded_instruction<=ANDI ;decoded_instruction_size<=opcode_width_ANDI ;
        when opcode_NANDI =>decoded_instruction<=NANDI;decoded_instruction_size<=opcode_width_NANDI;
        when opcode_ORI   =>decoded_instruction<=ORI  ;decoded_instruction_size<=opcode_width_ORI  ;
        when opcode_NORI  =>decoded_instruction<=NORI ;decoded_instruction_size<=opcode_width_NORI ;
        when opcode_XORI  =>decoded_instruction<=XORI ;decoded_instruction_size<=opcode_width_XORI ;
        when opcode_LSLI  =>decoded_instruction<=LSLI ;decoded_instruction_size<=opcode_width_LSLI ;
        when opcode_LSRI  =>decoded_instruction<=LSRI ;decoded_instruction_size<=opcode_width_LSRI ;
        when opcode_IADDI =>decoded_instruction<=IADDI;decoded_instruction_size<=opcode_width_IADDI;
        when opcode_ISUBI =>decoded_instruction<=ISUBI;decoded_instruction_size<=opcode_width_ISUBI;
        when opcode_IMULI =>decoded_instruction<=IMULI;decoded_instruction_size<=opcode_width_IMULI;
        when opcode_IDIVI =>decoded_instruction<=IDIVI;decoded_instruction_size<=opcode_width_IDIVI;
        when opcode_IASRI =>decoded_instruction<=IASRI;decoded_instruction_size<=opcode_width_IASRI;
        when opcode_IADDR =>decoded_instruction<=IADDR;decoded_instruction_size<=opcode_width_IADDR;
        when opcode_ISUBR =>decoded_instruction<=ISUBR;decoded_instruction_size<=opcode_width_ISUBR;
        when opcode_IMULR =>decoded_instruction<=IMULR;decoded_instruction_size<=opcode_width_IMULR;
        when opcode_IDIVR =>decoded_instruction<=IDIVR;decoded_instruction_size<=opcode_width_IDIVR;
        when opcode_IASRR =>decoded_instruction<=IASRR;decoded_instruction_size<=opcode_width_IASRR;
        when opcode_FADDI =>decoded_instruction<=FADDI;decoded_instruction_size<=opcode_width_FADDI;
        when opcode_FSUBI =>decoded_instruction<=FSUBI;decoded_instruction_size<=opcode_width_FSUBI;
        when opcode_FMULI =>decoded_instruction<=FMULI;decoded_instruction_size<=opcode_width_FMULI;
        when opcode_FDIVI =>decoded_instruction<=FDIVI;decoded_instruction_size<=opcode_width_FDIVI;
        when opcode_FASRI =>decoded_instruction<=FASRI;decoded_instruction_size<=opcode_width_FASRI;
        when opcode_FADDR =>decoded_instruction<=FADDR;decoded_instruction_size<=opcode_width_FADDR;
        when opcode_FSUBR =>decoded_instruction<=FSUBR;decoded_instruction_size<=opcode_width_FSUBR;
        when opcode_FMULR =>decoded_instruction<=FMULR;decoded_instruction_size<=opcode_width_FMULR;
        when opcode_FDIVR =>decoded_instruction<=FDIVR;decoded_instruction_size<=opcode_width_FDIVR;
        when opcode_FASRR =>decoded_instruction<=FASRR;decoded_instruction_size<=opcode_width_FASRR;
        when others =>
        -- Default to a NOP.
        decoded_instruction <= ANDR;
        decoded_instruction_size <= 1;
        end case;
    end if;

    end process instruction_length_decode;


end architecture rtl;
