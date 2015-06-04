
--! ------------------------------------------------------------------------------------------------
--!
--! @file common.vhdl
--! @brief Contains all constants, types and functions used across the architecture.
--!
--! ------------------------------------------------------------------------------------------------

library ieee;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;

--! Package header for all constants, functions and types used in TIM.
package tim_common is

    --! @brief The number of bits used to represent an instruction opcode.
    constant opcode_length      : integer    := 6;

    --! @brief The number of bits used to represent a single word in memory.
    constant word_width         : integer    := 32;

    --! @brief The number of bits used to represent an instruction's condition code.
    constant condition_length   : integer  := 2; 
    
    --! The kinds of operation the ALU can perform on arithmetic operands.
    type tim_alu_arith_op   is (alu_arith_add, alu_arith_sub, alu_arith_mul, alu_arith_div);

    --! The kinds of operation the ALU can perform on boolean operands.
    type tim_alu_bool_op   is (alu_bool_and, alu_bool_nand, alu_bool_or, alu_bool_nor, 
                               alu_bool_xor, alu_bool_not, alu_bool_sl, alu_bool_sr);

    --! Index of the program counter in the register file.
    constant  reg_pc            : integer   := 16;

end package;
