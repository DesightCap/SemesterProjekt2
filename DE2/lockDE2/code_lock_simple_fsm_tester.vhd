library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.all;


entity code_lock_simple_fsm_tester is
	port
	(
		-- Input ports
		SW		: in  std_logic_vector(3 downto 0);
		KEY 	: in 	std_logic_vector(2 downto 0);
		CLOCK_50 : in std_logic;
		GPIO_0: in std_logic_vector(4 downto 0);
		-- Output ports
		LEDR	:	out std_logic_vector(0 downto 0);
		HEX0 	:	out std_logic_vector(6 downto 0);
		HEX1 	:	out std_logic_vector(6 downto 0);
		HEX2 	:	out std_logic_vector(6 downto 0);
		HEX3 	:	out std_logic_vector(6 downto 0);
		HEX4 	:	out std_logic_vector(6 downto 0);
		HEX5 	:	out std_logic_vector(6 downto 0);
		HEX6 	:	out std_logic_vector(6 downto 0);
		HEX7 	:	out std_logic_vector(6 downto 0)
		
	);
end code_lock_simple_fsm_tester;


architecture code_lock_simple_fsm_tester_impl of code_lock_simple_fsm_tester is
begin
	call_code_lock_simple_fsm: entity code_lock_simple_fsm(code_lock_simple_fsm_impl)
		port map(
			clk => CLOCK_50,
			reset => KEY(0),
			unlocking => KEY(1),
			locking => KEY(2),
			code_in => SW(3 downto 0),
			lock => LEDR(0),
			new_code => GPIO_0(4 downto 0),
			HEX0 => HEX0,
			HEX1 => HEX1,
			HEX2 => HEX2,
			HEX3 => HEX3,
			HEX4 => HEX4,
			HEX5 => HEX5,
			HEX6 => HEX6,
			HEX7 => HEX7
			);
end code_lock_simple_fsm_tester_impl;