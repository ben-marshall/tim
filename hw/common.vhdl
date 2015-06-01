
--! ------------------------------------------------------------------------------------------------
--!
--! @file common.vhdl
--! @brief Contains all constants, types and functions used across the architecture.
--!
--! ------------------------------------------------------------------------------------------------


--! Package header for all constants, functions and types used in TIM.
package tim_common_package is

    --! @brief The number of bits used to represent an instruction opcode.
    constant opcode_length : integer;

    --! @brief The number of bits used to represent a single word in memory.
    constant word_length   : integer;

    --! @brief The number of bits used to represent an instruction's condition code.
    constant condition_length: integer;

end package;
