
--! ------------------------------------------------------------------------------------------------
--!
--! @file common.vhdl
--! @brief Contains all constants, types and functions used across the architecture.
--!
--! ------------------------------------------------------------------------------------------------

--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! Package header for all constants, functions and types used in TIM.
package tim_common is
    
    --! @brief The number of bits used to represent a single word in memory.
    constant memory_word_width         : integer    := 32;

end package;
