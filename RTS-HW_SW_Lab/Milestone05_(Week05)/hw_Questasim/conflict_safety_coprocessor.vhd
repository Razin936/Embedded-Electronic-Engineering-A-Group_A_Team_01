-- ============================================================================
-- Week 5 HW partition: Conflict & Safety Coprocessor
-- Project: Cross-Traffic Intersection Management for Autonomous Vehicles
-- Tool: QuestaSim / ModelSim, VHDL-2008
--
-- Vehicle encoding, same as the FreeRTOS C code:
--   Vehicle IDs: V0..V5
--   origin: EAST=00, WEST=01, NORTH=10, SOUTH=11
--   turn  : STRAIGHT=00, RIGHT=01, LEFT=10
--   mode  : 0=M/M/1, 1=M/M/2
--
-- Main outputs:
--   st_safe  : safe_to_grant(candidate)
--   st_nocol : noCollision() over current crossing set
--   st_cwc   : conflictsWithCrossing vector for all candidates
--   st_alarm : latched collision alarm, cleared by rst_n=0
-- ============================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity conflict_safety_coprocessor is
    generic (
        N : positive := 6
    );
    port (
        clk       : in  std_logic;
        rst_n     : in  std_logic;
        ctrl_start: in  std_logic;

        -- Register-like inputs from software / testbench
        mode      : in  std_logic;  -- 0=M/M/1, 1=M/M/2
        cand      : in  std_logic_vector(2 downto 0);
        origin    : in  std_logic_vector((2*N)-1 downto 0);
        turn      : in  std_logic_vector((2*N)-1 downto 0);
        emergency : in  std_logic_vector(N-1 downto 0);
        crossing  : in  std_logic_vector(N-1 downto 0);

        -- Status-register-like outputs
        st_safe   : out std_logic;
        st_nocol  : out std_logic;
        st_cwc    : out std_logic_vector(N-1 downto 0);
        st_alarm  : out std_logic
    );
end entity conflict_safety_coprocessor;

architecture rtl of conflict_safety_coprocessor is

    constant MODE_MM1 : std_logic := '0';
    constant MODE_MM2 : std_logic := '1';

    constant ORIGIN_EAST  : std_logic_vector(1 downto 0) := "00";
    constant ORIGIN_WEST  : std_logic_vector(1 downto 0) := "01";
    constant ORIGIN_NORTH : std_logic_vector(1 downto 0) := "10";
    constant ORIGIN_SOUTH : std_logic_vector(1 downto 0) := "11";

    constant TURN_STRAIGHT : std_logic_vector(1 downto 0) := "00";
    constant TURN_RIGHT    : std_logic_vector(1 downto 0) := "01";
    constant TURN_LEFT     : std_logic_vector(1 downto 0) := "10";

    signal no_collision_s : std_logic;
    signal cwc_s          : std_logic_vector(N-1 downto 0);
    signal safe_s         : std_logic;
    signal alarm_latched  : std_logic := '0';

    function get2(v : std_logic_vector; idx : natural) return std_logic_vector is
        variable r : std_logic_vector(1 downto 0);
    begin
        r := v((2*idx)+1 downto (2*idx));
        return r;
    end function;

    function is_main_road(o : std_logic_vector(1 downto 0)) return boolean is
    begin
        return (o = ORIGIN_EAST) or (o = ORIGIN_WEST);
    end function;

    function is_opposite(a : std_logic_vector(1 downto 0);
                         b : std_logic_vector(1 downto 0)) return boolean is
    begin
        return ((a = ORIGIN_EAST)  and (b = ORIGIN_WEST))  or
               ((a = ORIGIN_WEST)  and (b = ORIGIN_EAST))  or
               ((a = ORIGIN_NORTH) and (b = ORIGIN_SOUTH)) or
               ((a = ORIGIN_SOUTH) and (b = ORIGIN_NORTH));
    end function;

    function both_straight(ta : std_logic_vector(1 downto 0);
                           tb : std_logic_vector(1 downto 0)) return boolean is
    begin
        return (ta = TURN_STRAIGHT) and (tb = TURN_STRAIGHT);
    end function;

    -- Direct VHDL translation of the FreeRTOS C hw_interface.c conflict_pair().
    function conflict_pair(i         : natural;
                           j         : natural;
                           origin_v  : std_logic_vector;
                           turn_v    : std_logic_vector;
                           emg_v     : std_logic_vector) return boolean is
        variable oi : std_logic_vector(1 downto 0);
        variable oj : std_logic_vector(1 downto 0);
        variable ti : std_logic_vector(1 downto 0);
        variable tj : std_logic_vector(1 downto 0);
        variable i_main : boolean;
        variable j_main : boolean;
    begin
        if i = j then
            return false;
        end if;

        oi := get2(origin_v, i);
        oj := get2(origin_v, j);
        ti := get2(turn_v, i);
        tj := get2(turn_v, j);

        -- Emergency vehicle receives exclusive crossing.
        if (emg_v(i) = '1') or (emg_v(j) = '1') then
            return true;
        end if;

        -- Same origin conflicts.
        if oi = oj then
            return true;
        end if;

        -- Critical regression vector from the verified model:
        -- V4 = North-right and V5 = South-left must never cross together.
        if ((i = 4) and (j = 5)) or ((i = 5) and (j = 4)) then
            return true;
        end if;

        i_main := is_main_road(oi);
        j_main := is_main_road(oj);

        -- Opposite straight movements on the same road can overlap in M/M/2.
        -- This allows V1 + V2 parallel crossing.
        if (i_main = j_main) and is_opposite(oi, oj) and both_straight(ti, tj) then
            return false;
        end if;

        -- Main-vs-side or turning movements conflict by default.
        return true;
    end function;

    function popcount(v : std_logic_vector) return natural is
        variable c : natural := 0;
    begin
        for k in v'range loop
            if v(k) = '1' then
                c := c + 1;
            end if;
        end loop;
        return c;
    end function;

begin

    -- Compute conflictsWithCrossing(i) for all possible candidate vehicles.
    p_cwc : process(all)
        variable temp : std_logic_vector(N-1 downto 0);
    begin
        temp := (others => '0');

        for i in 0 to N-1 loop
            for j in 0 to N-1 loop
                if crossing(j) = '1' then
                    if conflict_pair(i, j, origin, turn, emergency) then
                        temp(i) := '1';
                    end if;
                end if;
            end loop;
        end loop;

        cwc_s <= temp;
    end process;

    -- Compute noCollision() over the current crossing set.
    p_nocol : process(all)
        variable ok : std_logic;
    begin
        ok := '1';

        for i in 0 to N-1 loop
            if crossing(i) = '1' then
                for j in i+1 to N-1 loop
                    if crossing(j) = '1' then
                        if conflict_pair(i, j, origin, turn, emergency) then
                            ok := '0';
                        end if;
                    end if;
                end loop;
            end if;
        end loop;

        no_collision_s <= ok;
    end process;

    -- Compute safe_to_grant(candidate).
    p_safe : process(all)
        variable c      : natural;
        variable count  : natural;
        variable result : std_logic;
    begin
        c := to_integer(unsigned(cand));
        count := popcount(crossing);
        result := '0';

        if c < N then
            if crossing(c) = '0' then
                if mode = MODE_MM1 then
                    if count = 0 then
                        result := '1';
                    end if;
                else
                    -- M/M/2: max two vehicles, and the candidate must not conflict.
                    if (count < 2) and (cwc_s(c) = '0') then
                        result := '1';
                    end if;
                end if;
            end if;
        end if;

        safe_s <= result;
    end process;

    -- Independent collision alarm. It latches when ctrl_start is active and
    -- noCollision() is false. Reset clears it.
    p_alarm : process(clk, rst_n)
    begin
        if rst_n = '0' then
            alarm_latched <= '0';
        elsif rising_edge(clk) then
            if (ctrl_start = '1') and (no_collision_s = '0') then
                alarm_latched <= '1';
            end if;
        end if;
    end process;

    st_safe  <= safe_s;
    st_nocol <= no_collision_s;
    st_cwc   <= cwc_s;
    st_alarm <= alarm_latched;

end architecture rtl;
