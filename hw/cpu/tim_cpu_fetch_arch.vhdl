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
    instruction_decode  : process(returned_instruction, internal_mem_acknowledge, memory_bus_enable)
    begin
        
        if(internal_mem_acknowledge = '1' and memory_bus_enable = '0') then
            decoded_arguments <= returned_instruction(word_width-opcode_length downto 0);

            case (returned_instruction(word_width-1 downto word_width-opcode_length)) is
                when opcode_LOADR  => decoded_instruction <= LOADR ;
                when opcode_LOADI  => decoded_instruction <= LOADI ;
                when opcode_STORI  => decoded_instruction <= STORI ;
                when opcode_STORR  => decoded_instruction <= STORR ;
                when opcode_PUSH   => decoded_instruction <= PUSH  ;
                when opcode_POP    => decoded_instruction <= POP   ;
                when opcode_MOVR   => decoded_instruction <= MOVR  ;
                when opcode_MOVI   => decoded_instruction <= MOVI  ;
                when opcode_JUMPR  => decoded_instruction <= JUMPR ;
                when opcode_JUMPI  => decoded_instruction <= JUMPI ;
                when opcode_CALLR  => decoded_instruction <= CALLR ;
                when opcode_CALLI  => decoded_instruction <= CALLI ;
                when opcode_RETURN => decoded_instruction <= RET   ;
                when opcode_TEST   => decoded_instruction <= TEST  ;
                when opcode_HALT   => decoded_instruction <= HALT  ;
                when opcode_ANDR   => decoded_instruction <= ANDR  ;
                when opcode_NANDR  => decoded_instruction <= NANDR ;
                when opcode_ORR    => decoded_instruction <= ORR   ;
                when opcode_NORR   => decoded_instruction <= NORR  ;
                when opcode_XORR   => decoded_instruction <= XORR  ;
                when opcode_LSLR   => decoded_instruction <= LSLR  ;
                when opcode_LSRR   => decoded_instruction <= LSRR  ;
                when opcode_NOTR   => decoded_instruction <= NOTR  ;
                when opcode_ANDI   => decoded_instruction <= ANDI  ;
                when opcode_NANDI  => decoded_instruction <= NANDI ;
                when opcode_ORI    => decoded_instruction <= ORI   ;
                when opcode_NORI   => decoded_instruction <= NORI  ;
                when opcode_XORI   => decoded_instruction <= XORI  ;
                when opcode_LSLI   => decoded_instruction <= LSLI  ;
                when opcode_LSRI   => decoded_instruction <= LSRI  ;
                when opcode_IADDI  => decoded_instruction <= IADDI ;
                when opcode_ISUBI  => decoded_instruction <= ISUBI ;
                when opcode_IMULI  => decoded_instruction <= IMULI ;
                when opcode_IDIVI  => decoded_instruction <= IDIVI ;
                when opcode_IASRI  => decoded_instruction <= IASRI ;
                when opcode_IADDR  => decoded_instruction <= IADDR ;
                when opcode_ISUBR  => decoded_instruction <= ISUBR ;
                when opcode_IMULR  => decoded_instruction <= IMULR ;
                when opcode_IDIVR  => decoded_instruction <= IDIVR ;
                when opcode_IASRR  => decoded_instruction <= IASRR ;
                when opcode_FADDI  => decoded_instruction <= FADDI ;
                when opcode_FSUBI  => decoded_instruction <= FSUBI ;
                when opcode_FMULI  => decoded_instruction <= FMULI ;
                when opcode_FDIVI  => decoded_instruction <= FDIVI ;
                when opcode_FASRI  => decoded_instruction <= FASRI ;
                when opcode_FADDR  => decoded_instruction <= FADDR ;
                when opcode_FSUBR  => decoded_instruction <= FSUBR ;
                when opcode_FMULR  => decoded_instruction <= FMULR ;
                when opcode_FDIVR  => decoded_instruction <= FDIVR ;
                when opcode_FASRR  => decoded_instruction <= FASRR ;
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
