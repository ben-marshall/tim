
--! ------------------------------------------------------------------------------------------------
--!
--! @file instructions.vhdl
--! @brief Contains definitions of all instruction opcodes and their byte size in memory.
--!
--! ------------------------------------------------------------------------------------------------

--! Use the standard IEEE libraries
library ieee;
--! Import standard logic interfaces.
use ieee.std_logic_1164.ALL;
--! Standard numeric operations and types.
use ieee.numeric_std.ALL;

--! Package that contains declarations and definitions for all instruction opcodes and their lengths.
package tim_instructions is

    --! @brief The number of bits used to represent an instruction opcode.
    constant opcode_width               : integer    := 6;
    
    --! @brief The number of bits used to represent an instruction's condition code.
    constant condition_width            : integer  := 2; 
    
    --! The maximum width of an instruction immediate.
    constant immediate_width            : integer   := 16;


    --! A simple way of encoding all of the instructions once they are decoded.
    type tim_instruction is ( LOADR ,LOADI ,STORI ,STORR ,PUSH  ,POP   ,MOVR  ,MOVI  ,JUMPR ,JUMPI,
                              CALLR ,CALLI ,RET   ,TEST  ,HALT  ,ANDR  ,NANDR ,ORR   ,NORR  ,XORR,
                              LSLR  ,LSRR  ,NOTR  ,ANDI  ,NANDI ,ORI   ,NORI  ,XORI  ,LSLI  ,LSRI,
                              IADDI ,ISUBI ,IMULI ,IDIVI ,IASRI ,IADDR ,ISUBR ,IMULR ,IDIVR ,IASRR,
                              FADDI ,FSUBI ,FMULI ,FDIVI ,FASRI ,FADDR ,FSUBR ,FMULR ,FDIVR ,FASRR);

    
    --! An easy way to encode the conditional execution bits of an instruction.
    type tim_instruction_condition is (ALWAYS, IF_TRUE, IF_FALSE, IF_ZERO);


    --! Load to register X from address in register Y with offset in register Z.     
    constant opcode_LOADR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(1, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_LOADR : integer := 3;
  
    --! Load to register X from address in register Y with immediate offset.         
    constant opcode_LOADI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(2, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_LOADI : integer := 4;
 
    --! Store register X to address in register Y with offset in register Z.         
    constant opcode_STORI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(3, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_STORI : integer := 4;
 
    --! Store register X to address in register Y with immediate offset.             
    constant opcode_STORR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(4, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_STORR : integer := 3;
 
    --! Push register X onto the top of the stack and decrement the stack pointer.   
    constant opcode_PUSH  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(5, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_PUSH  : integer := 2;
 
    --! Pop element at top of stack into register X and increment the stack pointer. 
    constant opcode_POP   : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(6, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_POP   : integer := 2;
 
    --! Move the content of register X into register Y                               
    constant opcode_MOVR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(7, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_MOVR  : integer := 3;
 
    --! Move immediate I into register X                                             
    constant opcode_MOVI  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(8, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_MOVI  : integer := 4;
 
    --! Jump to address contained within register X                                  
    constant opcode_JUMPR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(9, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_JUMPR : integer := 2;
 
    --! Jump to address contained within instruction immediate.                      
    constant opcode_JUMPI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(10,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_JUMPI : integer := 4;
 
    --! Call to function who's address is contained within register X                
    constant opcode_CALLR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(11,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_CALLR : integer := 2;
 
    --! Call to function who's address is contained within instruction immediate.    
    constant opcode_CALLI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(12,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_CALLI : integer := 4;
 
    --! Return from the last function call.                                          
    constant opcode_RETURN: std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(13,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_RETURN : integer := 1;
    
    --! Test two general or special registers and set comparison bits.               
    constant opcode_TEST  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(14,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_TEST  : integer := 4;
 
    --! Stop processing and wait to be reset.                                        
    constant opcode_HALT  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(15,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_HALT  : integer := 1;
 
    --! Bitwise AND two registers together.                                          
    constant opcode_ANDR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(0, opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_ANDR  : integer := 3;
 
    --! Bitwise NAND two registers together.                                         
    constant opcode_NANDR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(16,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_NANDR : integer := 3;
 
    --! Bitwise OR two registers together.                                           
    constant opcode_ORR   : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(17,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_ORR   : integer := 3;
 
    --! Bitwise NOR two registers together.                                          
    constant opcode_NORR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(18,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_NORR  : integer := 3;
 
    --! Bitwise XOR two registers together.                                          
    constant opcode_XORR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(19,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_XORR  : integer := 3;
 
    --! Logical shift left the bits in register X by the value in register Y.        
    constant opcode_LSLR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(20,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_LSLR  : integer := 3;
 
    --! Logical shift right the bits in register X by the value in register Y.       
    constant opcode_LSRR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(21,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_LSRR  : integer := 3;
 
    --! Bitwise invert the specificed register.                                      
    constant opcode_NOTR  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(22,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_NOTR  : integer := 2;
 
    --! Bitwise AND two registers together.                                          
    constant opcode_ANDI  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(23,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_ANDI  : integer := 4;
 
    --! Bitwise NAND two registers together.                                         
    constant opcode_NANDI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(24,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_NANDI : integer := 4;
 
    --! Bitwise OR two registers together.                                           
    constant opcode_ORI   : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(25,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_ORI   : integer := 4;
 
    --! Bitwise NOR two registers together.                                          
    constant opcode_NORI  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(26,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_NORI  : integer := 4;
 
    --! Bitwise XOR two registers together.                                          
    constant opcode_XORI  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(27,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_XORI  : integer := 4;
 
    --! Logical shift left the bits in register X by the immediate value             
    constant opcode_LSLI  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(28,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_LSLI  : integer := 4;
 
    --! Logical shift right the bits in register X by the immediate value            
    constant opcode_LSRI  : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(29,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_LSRI  : integer := 4;
 
    --! Integer Add register X to immediate value.                                   
    constant opcode_IADDI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(30,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IADDI : integer := 4;
 
    --! Integer Subtract immediate value from register X.                            
    constant opcode_ISUBI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(31,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_ISUBI : integer := 4;
 
    --! Integer Multiply register X by immediate value.                              
    constant opcode_IMULI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(32,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IMULI : integer := 4;
 
    --! Integer Divide register X by immediate value.                                
    constant opcode_IDIVI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(33,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IDIVI : integer := 4;
 
    --! Integer Arithmetic shift register X right immediate value.                   
    constant opcode_IASRI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(34,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IASRI : integer := 4;
 
    --! Integer Add register X to register Y.                                        
    constant opcode_IADDR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(35,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IADDR : integer := 3;
 
    --! Integer Subtract register X from register Y.                                 
    constant opcode_ISUBR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(36,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_ISUBR : integer := 3;
 
    --! Integer Multiply register X by register Y.                                   
    constant opcode_IMULR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(37,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IMULR : integer := 3;
 
    --! Integer Divide register X by register Y.                                     
    constant opcode_IDIVR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(38,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IDIVR : integer := 3;
 
    --! Integer Arithmetic shift register X right value in register Y.               
    constant opcode_IASRR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(39,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_IASRR : integer := 3;
 
    --! Floating point Add register X to immediate value.                            
    constant opcode_FADDI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(40,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FADDI : integer := 4;
 
    --! Floating point Subtract immediate value from register X.                     
    constant opcode_FSUBI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(41,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FSUBI : integer := 4;
 
    --! Floating point Multiply register X by immediate value.                       
    constant opcode_FMULI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(42,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FMULI : integer := 4;
 
    --! Floating point Divide register X by immediate value.                         
    constant opcode_FDIVI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(43,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FDIVI : integer := 4;
 
    --! Floating point Arithmetic shift register X right immediate value.            
    constant opcode_FASRI : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(44,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FASRI : integer := 4;
 
    --! Floating point Add register X to register Y.                                 
    constant opcode_FADDR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(45,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FADDR : integer := 3;
 
    --! Floating point Subtract register X from register Y.                          
    constant opcode_FSUBR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(46,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FSUBR : integer := 3;
 
    --! Floating point Multiply register X by register Y.                            
    constant opcode_FMULR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(47,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FMULR : integer := 3;
 
    --! Floating point Divide register X by register Y.                              
    constant opcode_FDIVR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(48,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FDIVR : integer := 3;
 
    --! Floating point Arithmetic shift register X right value in register Y.        
    constant opcode_FASRR : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(49,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_FASRR : integer := 3;
 
    --! SLEEP for a while
    constant opcode_SLEEP : std_logic_vector(opcode_width-1 downto 0) := std_logic_vector(to_unsigned(50,opcode_width));
 
    --! The length in bytes of the instruction 
    constant opcode_width_SLEEP : integer := 2;

end package;
