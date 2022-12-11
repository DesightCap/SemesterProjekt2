library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.all;

entity code_lock_simple_fsm is
	port
	(
		-- Input ports
		clk, reset, unlocking, locking	:	in std_logic;
		code_in									: 	in std_logic_vector(3 downto 0);
		new_code									:	in std_logic_vector(4 downto 0);
		-- Output ports
		lock 	:	out std_logic;
		HEX0 	:	out std_logic_vector(6 downto 0);
		HEX1 	:	out std_logic_vector(6 downto 0);
		HEX2 	:	out std_logic_vector(6 downto 0);
		HEX3 	:	out std_logic_vector(6 downto 0);
		HEX4 	:	out std_logic_vector(6 downto 0);
		HEX5 	:	out std_logic_vector(6 downto 0);
		HEX6 	:	out std_logic_vector(6 downto 0);
		HEX7 	:	out std_logic_vector(6 downto 0)
		
	);
end code_lock_simple_fsm;

architecture code_lock_simple_fsm_impl of code_lock_simple_fsm is
	type state is (idle, ev_code, unlocked);
	signal present_state, next_state : state;

	constant default_code	:	std_logic_vector := "1100";
	signal code	:	std_logic_vector(3 downto 0) := default_code;
	
	constant maxNumber : integer := 500000000; --Passer med 10 sekunder
	shared variable varClock : integer := 0;

	

begin

	state_reg : process (clk, reset)
	begin
		if reset = '0' then
			present_state <= idle;
			varClock := 0;
		elsif rising_edge(clk) then
			if present_state = ev_code then
				varClock := 0;
			end if;
			present_state <= next_state;
			varClock := varClock + 1;
			if present_state = unlocked then
				if varClock >= maxNumber then
					present_state <= idle;
				end if;
			end if;
		end if;
	end process;



	outputs: process (present_state)
	begin
		case present_state is
			when idle =>
				lock <= '1';
				
			when ev_code =>
				lock <= '1';
			
			when unlocked =>
				lock <= '0';
		end case;
	end process;
	
	update_code: process (new_code(0))
	begin
		case new_code(0) is
			when '1' =>
				code <= new_code(4 downto 1);
			when '0' =>
				code <= code;				
		end case;
	end process;
	

	nxt_state : process (present_state, unlocking, locking, code_in)
	begin
		case present_state is
			when idle =>
				if unlocking = '0' then
					next_state <= ev_code;
				else
					next_state <= idle;
				end if;
				HEX7 <= "1111111"; -- empty
				HEX6 <= "1111111"; -- empty
				HEX5 <= "0010010"; -- S
				HEX4 <= "0000110"; -- E
				HEX3 <= "0001000"; -- A
				HEX2 <= "1000111"; -- L
				HEX1 <= "0000110"; -- E
				HEX0 <= "0100001"; -- d
				
			when ev_code =>
				if code_in = code then
					next_state <= unlocked;
				else
					next_state <= idle;
				end if;
				
			when unlocked =>
				if locking = '0' then
					next_state <= idle;
				else
					next_state <= unlocked;
				end if;
				HEX7 <= "1000001"; -- U
				HEX6 <= "0101011"; -- n
				HEX5 <= "0010010"; -- S
				HEX4 <= "0000110"; -- E
				HEX3 <= "0001000"; -- A
				HEX2 <= "1000111"; -- L
				HEX1 <= "0000110"; -- E
				HEX0 <= "0100001"; -- d
				
			end case;
	end process;
	
	
	
	
	
end code_lock_simple_fsm_impl;