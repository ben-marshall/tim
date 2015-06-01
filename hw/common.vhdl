
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
    constant opcode_length : integer    := 6;

    --! @brief The number of bits used to represent a single word in memory.
    constant word_length   : integer    := 32;

    --! @brief The number of bits used to represent an instruction's condition code.
    constant condition_length: integer  := 2; 

end package;
