--! ------------------------------------------------------------------------------------------------
--!
--! @file tim_cpu_fetch_arch.vhdl
--! @brief Contains the first and most basic architecture implementation of the TIM CPU.
--!
--! ------------------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

--! Imported from tim_bus package,
use work.tim_bus.tim_bus_data_width;
--! Imported from tim_common package
use work.tim_common.opcode_length;
use work.tim_common.word_width;
--! Import the whole instructions package.
use work.tim_instructions.all;

--! Architecture of the instruction fetch module.
architecture tim_cpu_fetch_arch of tim_cpu_fetch is

    --
    -- Imported component declarations.
    --

    --! The bus master controller module which arbitrates bus requests and responses.
    component tim_bus_master is
        generic(
            --! The number of data and address lines.
            data_width  : integer := tim_bus_data_width
        );
        port(
            clk             : in    std_logic;
            reset           : in    std_logic;
            bus_lines       : inout std_logic_vector(data_width-1 downto 0);
            bus_valid       : out   std_logic;
            bus_enable      : in    std_logic;
            bus_read_write  : out   std_logic;
            req_data_lines      : inout std_logic_vector(data_width-1 downto 0);
            req_read_write      : in    std_logic;
            req_address_lines   : in    std_logic_vector(data_width-1 downto 0);
            req_pending         : in    std_logic;
            req_acknowledge     : out   std_logic
        );
    end component tim_bus_master;

    --! Typedef for the state of the fetch module.
    type fetch_state is (FETCH_RESET, IDLE, IDLE_LOOP, REQUEST);
    --! Current state of the fetch module.
    signal current_state    : fetch_state := FETCH_RESET;
    --! Next state of the fetch module.
    signal next_state       : fetch_state := IDLE;


    --
    -- Signals for communicating with the bus master.

    signal returned_instruction     : std_logic_vector(tim_bus_data_width-1 downto 0);
    signal internal_address_lines   : std_logic_vector(tim_bus_data_width-1 downto 0);
    signal internal_mem_pending     : std_logic;
    signal internal_mem_acknowledge : std_logic;

begin
    
    --! Always ask for the program counter.
    internal_address_lines  <= program_counter;

    --! Responsible for the synchronous state transitions and asynchronous resets.
    state_machine_progress : process (clk, reset)
    begin
        if(reset='1') then
            current_state   <= FETCH_RESET;
        elsif(clk'event and clk = '1') then
            current_state   <= next_state;
        end if;
    end process state_machine_progress;


    --! Responsible for decoding a returned instruction and putting them onto the output ports.
    instruction_decode  : process(reset,returned_instruction, internal_mem_acknowledge, memory_bus_enable)
    begin
        
        if(reset = '1') then
            decoded_instruction_size    <= 0;
            decoded_instruction         <= ANDR;
            decoded_arguments           <= (others => '0');

        elsif(internal_mem_acknowledge = '1' and memory_bus_enable = '0') then
            decoded_arguments <= returned_instruction(word_width-opcode_length downto 0);

            case (returned_instruction(word_width-1 downto word_width-opcode_length)) is
                when opcode_LOADR  => decoded_instruction <= LOADR ; decoded_instruction_size <= opcode_length_LOADR;
                when opcode_LOADI  => decoded_instruction <= LOADI ; decoded_instruction_size <= opcode_length_LOADI  ;
                when opcode_STORI  => decoded_instruction <= STORI ; decoded_instruction_size <= opcode_length_STORI  ;
                when opcode_STORR  => decoded_instruction <= STORR ; decoded_instruction_size <= opcode_length_STORR  ;
                when opcode_PUSH   => decoded_instruction <= PUSH  ; decoded_instruction_size <= opcode_length_PUSH   ;
                when opcode_POP    => decoded_instruction <= POP   ; decoded_instruction_size <= opcode_length_POP    ;
                when opcode_MOVR   => decoded_instruction <= MOVR  ; decoded_instruction_size <= opcode_length_MOVR   ;
                when opcode_MOVI   => decoded_instruction <= MOVI  ; decoded_instruction_size <= opcode_length_MOVI   ;
                when opcode_JUMPR  => decoded_instruction <= JUMPR ; decoded_instruction_size <= opcode_length_JUMPR  ;
                when opcode_JUMPI  => decoded_instruction <= JUMPI ; decoded_instruction_size <= opcode_length_JUMPI  ;
                when opcode_CALLR  => decoded_instruction <= CALLR ; decoded_instruction_size <= opcode_length_CALLR  ;
                when opcode_CALLI  => decoded_instruction <= CALLI ; decoded_instruction_size <= opcode_length_CALLI  ;
                when opcode_RETURN => decoded_instruction <= RET   ; decoded_instruction_size <= opcode_length_RETURN ;
                when opcode_TEST   => decoded_instruction <= TEST  ; decoded_instruction_size <= opcode_length_TEST   ;
                when opcode_HALT   => decoded_instruction <= HALT  ; decoded_instruction_size <= opcode_length_HALT   ;
                when opcode_ANDR   => decoded_instruction <= ANDR  ; decoded_instruction_size <= opcode_length_ANDR   ;
                when opcode_NANDR  => decoded_instruction <= NANDR ; decoded_instruction_size <= opcode_length_NANDR  ;
                when opcode_ORR    => decoded_instruction <= ORR   ; decoded_instruction_size <= opcode_length_ORR    ;
                when opcode_NORR   => decoded_instruction <= NORR  ; decoded_instruction_size <= opcode_length_NORR   ;
                when opcode_XORR   => decoded_instruction <= XORR  ; decoded_instruction_size <= opcode_length_XORR   ;
                when opcode_LSLR   => decoded_instruction <= LSLR  ; decoded_instruction_size <= opcode_length_LSLR   ;
                when opcode_LSRR   => decoded_instruction <= LSRR  ; decoded_instruction_size <= opcode_length_LSRR   ;
                when opcode_NOTR   => decoded_instruction <= NOTR  ; decoded_instruction_size <= opcode_length_NOTR   ;
                when opcode_ANDI   => decoded_instruction <= ANDI  ; decoded_instruction_size <= opcode_length_ANDI   ;
                when opcode_NANDI  => decoded_instruction <= NANDI ; decoded_instruction_size <= opcode_length_NANDI  ;
                when opcode_ORI    => decoded_instruction <= ORI   ; decoded_instruction_size <= opcode_length_ORI    ;
                when opcode_NORI   => decoded_instruction <= NORI  ; decoded_instruction_size <= opcode_length_NORI   ;
                when opcode_XORI   => decoded_instruction <= XORI  ; decoded_instruction_size <= opcode_length_XORI   ;
                when opcode_LSLI   => decoded_instruction <= LSLI  ; decoded_instruction_size <= opcode_length_LSLI   ;
                when opcode_LSRI   => decoded_instruction <= LSRI  ; decoded_instruction_size <= opcode_length_LSRI   ;
                when opcode_IADDI  => decoded_instruction <= IADDI ; decoded_instruction_size <= opcode_length_IADDI  ;
                when opcode_ISUBI  => decoded_instruction <= ISUBI ; decoded_instruction_size <= opcode_length_ISUBI  ;
                when opcode_IMULI  => decoded_instruction <= IMULI ; decoded_instruction_size <= opcode_length_IMULI  ;
                when opcode_IDIVI  => decoded_instruction <= IDIVI ; decoded_instruction_size <= opcode_length_IDIVI  ;
                when opcode_IASRI  => decoded_instruction <= IASRI ; decoded_instruction_size <= opcode_length_IASRI  ;
                when opcode_IADDR  => decoded_instruction <= IADDR ; decoded_instruction_size <= opcode_length_IADDR  ;
                when opcode_ISUBR  => decoded_instruction <= ISUBR ; decoded_instruction_size <= opcode_length_ISUBR  ;
                when opcode_IMULR  => decoded_instruction <= IMULR ; decoded_instruction_size <= opcode_length_IMULR  ;
                when opcode_IDIVR  => decoded_instruction <= IDIVR ; decoded_instruction_size <= opcode_length_IDIVR  ;
                when opcode_IASRR  => decoded_instruction <= IASRR ; decoded_instruction_size <= opcode_length_IASRR  ;
                when opcode_FADDI  => decoded_instruction <= FADDI ; decoded_instruction_size <= opcode_length_FADDI  ;
                when opcode_FSUBI  => decoded_instruction <= FSUBI ; decoded_instruction_size <= opcode_length_FSUBI  ;
                when opcode_FMULI  => decoded_instruction <= FMULI ; decoded_instruction_size <= opcode_length_FMULI  ;
                when opcode_FDIVI  => decoded_instruction <= FDIVI ; decoded_instruction_size <= opcode_length_FDIVI  ;
                when opcode_FASRI  => decoded_instruction <= FASRI ; decoded_instruction_size <= opcode_length_FASRI  ;
                when opcode_FADDR  => decoded_instruction <= FADDR ; decoded_instruction_size <= opcode_length_FADDR  ;
                when opcode_FSUBR  => decoded_instruction <= FSUBR ; decoded_instruction_size <= opcode_length_FSUBR  ;
                when opcode_FMULR  => decoded_instruction <= FMULR ; decoded_instruction_size <= opcode_length_FMULR  ;
                when opcode_FDIVR  => decoded_instruction <= FDIVR ; decoded_instruction_size <= opcode_length_FDIVR  ;
                when opcode_FASRR  => decoded_instruction <= FASRR ; decoded_instruction_size <= opcode_length_FASRR  ;
                when others =>
                    -- Default to a NOP.
                    decoded_instruction <= ANDR;
                    decoded_arguments   <= (others => '0');
            end case;
        end if;
    end process instruction_decode;


    --! Responsible for determining the next state machine state.
    state_machine_next  : process(clk, internal_mem_acknowledge, instruction_enable)
    begin
        case(current_state) is
            when FETCH_RESET =>
            
                next_state <= IDLE;
            
            when IDLE   =>
            
                if(instruction_enable = '1') then
                    next_state <= REQUEST;
                else
                    next_state <= IDLE;
                end if;
            
            when IDLE_LOOP   =>
            
                if(instruction_enable = '1') then
                    next_state <= REQUEST;
                else
                    next_state <= IDLE_LOOP;
                end if;

            when REQUEST =>

                if(internal_mem_acknowledge = '0') then
                    next_state <= IDLE_LOOP;
                else
                    next_state <= REQUEST;
                end if;

            end case;

    end process state_machine_next;

    --! Responsible for setting the control and IO signals of the module.
    control_signals : process(current_state)
    begin

        case (current_state) is
            when FETCH_RESET =>
                instruction_valid       <= '0';
                internal_mem_pending    <= '0';
            
            when IDLE =>
                instruction_valid       <= '0';
                internal_mem_pending    <= '0';

            when IDLE_LOOP  =>
                instruction_valid       <= '1';
                internal_mem_pending    <= '0';

            when REQUEST =>
                instruction_valid       <= '0';
                internal_mem_pending    <= '1';

        end case;

    end process control_signals;


    --! The fetch modules internal bus controller for making instruction requests.
    bus_controller : tim_bus_master generic map(
        data_width  => tim_bus_data_width
    ) port map(
        clk               =>  clk,
        reset             =>  reset,
        bus_lines         =>  memory_bus_lines,
        bus_valid         =>  memory_bus_valid,
        bus_enable        =>  memory_bus_enable,
        bus_read_write    =>  memory_bus_read_write,
        req_data_lines    =>  returned_instruction,
        req_read_write    =>  '0', -- Always read requests.
        req_address_lines =>  internal_address_lines,
        req_pending       =>  internal_mem_pending,
        req_acknowledge   =>  internal_mem_acknowledge
    );


end architecture tim_cpu_fetch_arch;
