-------------------------------------------------------------------------------
-- @file alu.vhdl
-- @brief Arithmetic and Boolean Logic Unit
-------------------------------------------------------------------------------

--! Use the default IEEE libraries.
library IEEE;
--! Use the default IEEE Logic libraries.
use IEEE.std_logic_1164.all;
--! Use the default IEEE arithmetic libraries.
use IEEE.numeric_std.all;

--! Used to include all of the project constants etc.
use work.tim_common.all;

--!
--! Arithmetic Logic Unit Entity.
--!
entity alu is
	port(
        --! Global Clock
        clk              : in  std_logic;
        --! Global Reset
        reset            : in  std_logic;

        --! Boolean operand one. Goes on the left of the operation.
        bool_operand_1 : in  std_logic_vector(word_width-1 downto 0);
        --! Boolean operand two. Goes on the right of the operation.
        bool_operand_2 : in  std_logic_vector(word_width-1 downto 0);
        --! The operation to perform on the two boolean operands.
        bool_operation : in  tim_alu_bool_op;
        --! The result of the boolean operation.
        bool_result    : out std_logic_vector(word_width-1 downto 0);
        
        --! Arithmetic operand one. Goes on the left of the operation.
        arith_operand_1 : in  unsigned        (word_width-1 downto 0);
        --! Arithmetic operand two. Goes on the right of the operation.
        arith_operand_2 : in  unsigned        (word_width-1 downto 0);
        --! The operation to perform on the two arithmetic operands.
        arith_operation : in  tim_alu_arith_op;
        --! The result of the arithmetic operation.
        arith_result    : out unsigned        (word_width-1 downto 0)
	);
end entity alu;

--!
--! Synthesisable architecture of the arithmetic logic unit.
--!
architecture rtl of alu is

    --! Internal result of the arithmetic operation. Pushed to arith_result every pos clock edge.
    signal internal_arith_result : unsigned        ((word_width*2)-1   downto 0) := (others => '0'); 
    --! Internal result of the boolean operation. Pushed to bool_result every pos clock edge.
    signal internal_bool_result  : std_logic_vector( word_width   -1   downto 0) := (others => '0');

begin

    --!
    --! Responsible for making sure that the tape register is updated properly each clock cycle.
    --!
    output_values_update: process(clk, reset, internal_arith_result, internal_bool_result)
    begin
        if (reset = '1') then
            
            arith_result <= (others => '0'); 
            bool_result  <= (others => '0'); 

        else
            
            arith_result <= internal_arith_result (word_width-1 downto 0);
            bool_result  <= internal_bool_result; 

        end if;
    end process output_values_update;

    --!
    --! Handles all boolean operations
    --!
    boolean_operations : process (bool_operand_1, bool_operand_2, bool_operation)
    begin
        case bool_operation is
            when alu_bool_and  => internal_bool_result <= bool_operand_1 and  bool_operand_2;
            when alu_bool_nand => internal_bool_result <= bool_operand_1 nand bool_operand_2;
            when alu_bool_or   => internal_bool_result <= bool_operand_1 or   bool_operand_2;
            when alu_bool_nor  => internal_bool_result <= bool_operand_1 nor  bool_operand_2;
            when alu_bool_xor  => internal_bool_result <= bool_operand_1 xor  bool_operand_2;
            when alu_bool_not  => internal_bool_result <= not  bool_operand_1;
            when alu_bool_sl   => internal_bool_result <= std_logic_vector(unsigned(bool_operand_1) sll to_integer(unsigned(bool_operand_2)));
            when alu_bool_sr   => internal_bool_result <= std_logic_vector(unsigned(bool_operand_1) srl to_integer(unsigned(bool_operand_2)));
            when others       => internal_bool_result <= bool_operand_1;
        end case;
    end process boolean_operations;

    --!
    --! Handles all arithmetic operations
    --!
    arithmetic_operations: process (arith_operation, arith_operand_1, arith_operand_2)
    begin
        case arith_operation is
            when alu_arith_add => internal_arith_result(word_width-1 downto 0) <= arith_operand_1 + arith_operand_2;
            when alu_arith_sub => internal_arith_result(word_width-1 downto 0) <= arith_operand_1 - arith_operand_2;
            when alu_arith_mul => internal_arith_result <= arith_operand_1 * arith_operand_2;
            when alu_arith_div => internal_arith_result(word_width-1 downto 0) <= arith_operand_1 / arith_operand_2;
            when others       => internal_arith_result(word_width-1 downto 0) <= arith_operand_1;
        end case;
    end process arithmetic_operations;

end architecture rtl;
