
--! ------------------------------------------------------------------------------------------------
--!
--! @file common.vhdl
--! @brief Contains all constants, types and functions used across the architecture.
--!
--! ------------------------------------------------------------------------------------------------


--! Package header for all constants, functions and types used in TIM.
package tim_common_package is

    --! @brief The number of bits used to represent an instruction opcode.
    constant opcode_length : integer    := 6;

    --! @brief The number of bits used to represent a single word in memory.
    constant word_length   : integer    := 32;

    --! @brief The number of bits used to represent an instruction's condition code.
    constant condition_length: integer  := 2;

    --
    -- The following are the opcode bit values and the length in bytes of every instruction in the
    -- architecture.
    --

    --! Load to register X from address in register Y with offset in register Z.     
    constant opcode_LOADR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_LOADR : integer := 4;
  
    --! Load to register X from address in register Y with immediate offset.         
    constant opcode_LOADI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_LOADI : integer := 4;
 
    --! Store register X to address in register Y with offset in register Z.         
    constant opcode_STORI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_STORI : integer := 4;
 
    --! Store register X to address in register Y with immediate offset.             
    constant opcode_STORR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_STORR : integer := 4;
 
    --! Push register X onto the top of the stack and decrement the stack pointer.   
    constant opcode_PUSH  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_PUSH  : integer := 4;
 
    --! Pop element at top of stack into register X and increment the stack pointer. 
    constant opcode_POP   : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_POP   : integer := 4;
 
    --! Move the content of register X into register Y                               
    constant opcode_MOVR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_MOVR  : integer := 4;
 
    --! Move immediate I into register X                                             
    constant opcode_MOVI  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_MOVI  : integer := 4;
 
    --! Jump to address contained within register X                                  
    constant opcode_JUMPR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_JUMPR : integer := 4;
 
    --! Jump to address contained within instruction immediate.                      
    constant opcode_JUMPI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_JUMPI : integer := 4;
 
    --! Call to function who's address is contained within register X                
    constant opcode_CALLR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_CALLR : integer := 4;
 
    --! Call to function who's address is contained within instruction immediate.    
    constant opcode_CALLI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_CALLI : integer := 4;
 
    --! Return from the last function call.                                          
    constant opcode_RETURN: std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_RETUR : integer := 4;
N
    --! Test two general or special registers and set comparison bits.               
    constant opcode_TEST  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_TEST  : integer := 4;
 
    --! Stop processing and wait to be reset.                                        
    constant opcode_HALT  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_HALT  : integer := 4;
 
    --! Bitwise AND two registers together.                                          
    constant opcode_ANDR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_ANDR  : integer := 4;
 
    --! Bitwise NAND two registers together.                                         
    constant opcode_NANDR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_NANDR : integer := 4;
 
    --! Bitwise OR two registers together.                                           
    constant opcode_ORR   : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_ORR   : integer := 4;
 
    --! Bitwise NOR two registers together.                                          
    constant opcode_NORR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_NORR  : integer := 4;
 
    --! Bitwise XOR two registers together.                                          
    constant opcode_XORR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_XORR  : integer := 4;
 
    --! Logical shift left the bits in register X by the value in register Y.        
    constant opcode_LSLR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_LSLR  : integer := 4;
 
    --! Logical shift right the bits in register X by the value in register Y.       
    constant opcode_LSRR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_LSRR  : integer := 4;
 
    --! Bitwise invert the specificed register.                                      
    constant opcode_NOTR  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_NOTR  : integer := 4;
 
    --! Bitwise AND two registers together.                                          
    constant opcode_ANDI  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_ANDI  : integer := 4;
 
    --! Bitwise NAND two registers together.                                         
    constant opcode_NANDI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_NANDI : integer := 4;
 
    --! Bitwise OR two registers together.                                           
    constant opcode_ORI   : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_ORI   : integer := 4;
 
    --! Bitwise NOR two registers together.                                          
    constant opcode_NORI  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_NORI  : integer := 4;
 
    --! Bitwise XOR two registers together.                                          
    constant opcode_XORI  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_XORI  : integer := 4;
 
    --! Logical shift left the bits in register X by the immediate value             
    constant opcode_LSLI  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_LSLI  : integer := 4;
 
    --! Logical shift right the bits in register X by the immediate value            
    constant opcode_LSRI  : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_LSRI  : integer := 4;
 
    --! Integer Add register X to immediate value.                                   
    constant opcode_IADDI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IADDI : integer := 4;
 
    --! Integer Subtract immediate value from register X.                            
    constant opcode_ISUBI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_ISUBI : integer := 4;
 
    --! Integer Multiply register X by immediate value.                              
    constant opcode_IMULI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IMULI : integer := 4;
 
    --! Integer Divide register X by immediate value.                                
    constant opcode_IDIVI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IDIVI : integer := 4;
 
    --! Integer Arithmetic shift register X right immediate value.                   
    constant opcode_IASRI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IASRI : integer := 4;
 
    --! Integer Add register X to register Y.                                        
    constant opcode_IADDR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IADDR : integer := 4;
 
    --! Integer Subtract register X from register Y.                                 
    constant opcode_ISUBR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_ISUBR : integer := 4;
 
    --! Integer Multiply register X by register Y.                                   
    constant opcode_IMULR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IMULR : integer := 4;
 
    --! Integer Divide register X by register Y.                                     
    constant opcode_IDIVR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IDIVR : integer := 4;
 
    --! Integer Arithmetic shift register X right value in register Y.               
    constant opcode_IASRR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_IASRR : integer := 4;
 
    --! Floating point Add register X to immediate value.                            
    constant opcode_FADDI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FADDI : integer := 4;
 
    --! Floating point Subtract immediate value from register X.                     
    constant opcode_FSUBI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FSUBI : integer := 4;
 
    --! Floating point Multiply register X by immediate value.                       
    constant opcode_FMULI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FMULI : integer := 4;
 
    --! Floating point Divide register X by immediate value.                         
    constant opcode_FDIVI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FDIVI : integer := 4;
 
    --! Floating point Arithmetic shift register X right immediate value.            
    constant opcode_FASRI : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FASRI : integer := 4;
 
    --! Floating point Add register X to register Y.                                 
    constant opcode_FADDR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FADDR : integer := 4;
 
    --! Floating point Subtract register X from register Y.                          
    constant opcode_FSUBR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FSUBR : integer := 4;
 
    --! Floating point Multiply register X by register Y.                            
    constant opcode_FMULR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FMULR : integer := 4;
 
    --! Floating point Divide register X by register Y.                              
    constant opcode_FDIVR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FDIVR : integer := 4;
 
    --! Floating point Arithmetic shift register X right value in register Y.        
    constant opcode_FASRR : std_logic_vector(opcode_length-1 downto 0) := std_logic_vector(to_unsigned(0));
 
    --! The length in bytes of the instruction 
    constant opcode_length_FASRR : integer := 4;
 

end package;
