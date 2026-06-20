-- ============================================================================
-- Testbench for conflict_safety_coprocessor.vhd
-- Tool: QuestaSim / ModelSim, VHDL-2008
-- ============================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_conflict_safety_coprocessor is
end entity;

architecture sim of tb_conflict_safety_coprocessor is

    constant N : positive := 6;
    constant CLK_PERIOD : time := 10 ns;

    constant MODE_MM1 : std_logic := '0';
    constant MODE_MM2 : std_logic := '1';

    constant EAST  : std_logic_vector(1 downto 0) := "00";
    constant WEST  : std_logic_vector(1 downto 0) := "01";
    constant NORTH : std_logic_vector(1 downto 0) := "10";
    constant SOUTH : std_logic_vector(1 downto 0) := "11";

    constant STRAIGHT : std_logic_vector(1 downto 0) := "00";
    constant RIGHT    : std_logic_vector(1 downto 0) := "01";
    constant LEFT     : std_logic_vector(1 downto 0) := "10";

    signal clk        : std_logic := '0';
    signal rst_n      : std_logic := '0';
    signal ctrl_start : std_logic := '0';
    signal mode       : std_logic := MODE_MM2;
    signal cand       : std_logic_vector(2 downto 0) := (others => '0');
    signal origin     : std_logic_vector((2*N)-1 downto 0) := (others => '0');
    signal turn       : std_logic_vector((2*N)-1 downto 0) := (others => '0');
    signal emergency  : std_logic_vector(N-1 downto 0) := (others => '0');
    signal crossing   : std_logic_vector(N-1 downto 0) := (others => '0');

    signal st_safe  : std_logic;
    signal st_nocol : std_logic;
    signal st_cwc   : std_logic_vector(N-1 downto 0);
    signal st_alarm : std_logic;

    procedure set2(signal v : inout std_logic_vector; idx : natural; value : std_logic_vector(1 downto 0)) is
    begin
        v((2*idx)+1 downto (2*idx)) <= value;
    end procedure;

    procedure pulse_start(signal s : out std_logic) is
    begin
        s <= '1';
        wait for CLK_PERIOD;
        s <= '0';
        wait for CLK_PERIOD;
    end procedure;

begin

    clk <= not clk after CLK_PERIOD/2;

    dut : entity work.conflict_safety_coprocessor
        generic map (
            N => N
        )
        port map (
            clk        => clk,
            rst_n      => rst_n,
            ctrl_start => ctrl_start,
            mode       => mode,
            cand       => cand,
            origin     => origin,
            turn       => turn,
            emergency  => emergency,
            crossing   => crossing,
            st_safe    => st_safe,
            st_nocol   => st_nocol,
            st_cwc     => st_cwc,
            st_alarm   => st_alarm
        );

    stim : process
    begin
        report "TB started: Week 5 Conflict & Safety Coprocessor" severity note;

        -- Fleet setup from FreeRTOS/C proposal:
        -- V0 EAST straight emergency
        -- V1 EAST straight normal
        -- V2 WEST straight normal
        -- V3 NORTH straight normal
        -- V4 NORTH right normal
        -- V5 SOUTH left normal
        set2(origin, 0, EAST);  set2(turn, 0, STRAIGHT); emergency(0) <= '1';
        set2(origin, 1, EAST);  set2(turn, 1, STRAIGHT); emergency(1) <= '0';
        set2(origin, 2, WEST);  set2(turn, 2, STRAIGHT); emergency(2) <= '0';
        set2(origin, 3, NORTH); set2(turn, 3, STRAIGHT); emergency(3) <= '0';
        set2(origin, 4, NORTH); set2(turn, 4, RIGHT);    emergency(4) <= '0';
        set2(origin, 5, SOUTH); set2(turn, 5, LEFT);     emergency(5) <= '0';

        crossing <= (others => '0');
        cand <= "001"; -- V1
        mode <= MODE_MM2;

        wait for 25 ns;
        rst_n <= '1';
        wait for 20 ns;

        -- Test 1: Empty intersection, M/M/2, V1 should be safe.
        report "TEST 1: empty M/M/2, candidate V1 -> safe" severity note;
        wait for 5 ns;
        assert st_safe = '1' report "FAIL T1: V1 should be safe in empty M/M/2" severity failure;
        assert st_nocol = '1' report "FAIL T1: empty intersection must be noCollision" severity failure;

        -- Test 2: V1 crossing, candidate V2 is opposite straight on main road -> safe in M/M/2.
        report "TEST 2: V1 crossing, candidate V2 -> safe in M/M/2" severity note;
        crossing <= "000010"; -- bit1 = V1 crossing
        cand <= "010";       -- V2
        wait for 20 ns;
        assert st_safe = '1' report "FAIL T2: V2 should be safe with V1 in M/M/2" severity failure;
        assert st_nocol = '1' report "FAIL T2: V1 alone must be noCollision" severity failure;

        -- Test 3: V1 and V2 both crossing. This is allowed. CWC should match C output 0x39.
        report "TEST 3: V1 + V2 crossing -> noCollision, CWC=0x39" severity note;
        crossing <= "000110"; -- bit1 and bit2
        cand <= "011";       -- V3, should conflict
        wait for 20 ns;
        assert st_nocol = '1' report "FAIL T3: V1+V2 should be collision-free" severity failure;
        assert st_cwc = "111001" report "FAIL T3: expected st_cwc=111001b = 0x39" severity failure;
        assert st_safe = '0' report "FAIL T3: V3 should not be safe with V1+V2 crossing" severity failure;

        -- Test 4: Critical forbidden pair V4 and V5 both crossing -> noCollision=0 and alarm latches.
        report "TEST 4: V4 + V5 crossing -> conflict and alarm" severity note;
        crossing <= "110000"; -- bit4 and bit5
        cand <= "001";
        wait for 20 ns;
        assert st_nocol = '0' report "FAIL T4: V4+V5 must be detected as collision" severity failure;
        pulse_start(ctrl_start);
        assert st_alarm = '1' report "FAIL T4: alarm must latch after collision" severity failure;

        -- Reset clears alarm.
        report "TEST 5: reset clears alarm" severity note;
        rst_n <= '0';
        wait for 20 ns;
        assert st_alarm = '0' report "FAIL T5: alarm should clear after reset" severity failure;
        rst_n <= '1';
        wait for 20 ns;

        -- Test 6: M/M/1 blocks parallel crossing.
        report "TEST 6: M/M/1, V1 crossing, candidate V2 -> not safe" severity note;
        mode <= MODE_MM1;
        crossing <= "000010"; -- V1 crossing
        cand <= "010";       -- V2
        wait for 20 ns;
        assert st_safe = '0' report "FAIL T6: M/M/1 should block V2 while V1 crossing" severity failure;

        -- Test 7: M/M/1 empty intersection allows candidate.
        report "TEST 7: M/M/1 empty, candidate V2 -> safe" severity note;
        crossing <= "000000";
        cand <= "010";
        wait for 20 ns;
        assert st_safe = '1' report "FAIL T7: M/M/1 empty should allow V2" severity failure;

        -- Test 8: Emergency candidate conflicts with occupied intersection, so not safe until free.
        report "TEST 8: emergency V0 waits while another vehicle is crossing" severity note;
        mode <= MODE_MM2;
        crossing <= "100000"; -- V5 crossing
        cand <= "000";       -- V0 emergency
        wait for 20 ns;
        assert st_safe = '0' report "FAIL T8: emergency V0 should wait until current crossing finishes" severity failure;

        crossing <= "000000";
        wait for 20 ns;
        assert st_safe = '1' report "FAIL T8b: emergency V0 should be safe after intersection becomes free" severity failure;

        report "ALL TESTS PASSED: Conflict & Safety Coprocessor works" severity note;
        wait;
    end process;

end architecture sim;
