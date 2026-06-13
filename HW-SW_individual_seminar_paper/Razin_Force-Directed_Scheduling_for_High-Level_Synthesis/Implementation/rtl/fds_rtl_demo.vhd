library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity fds_rtl_demo is
  port(
    clk   : in  std_logic;
    rst   : in  std_logic;
    start : in  std_logic;

    a : in signed(31 downto 0);
    b : in signed(31 downto 0);
    c : in signed(31 downto 0);
    d : in signed(31 downto 0);
    e : in signed(31 downto 0);
    f : in signed(31 downto 0);

    y    : out signed(63 downto 0);
    done : out std_logic
  );
end entity;

architecture rtl of fds_rtl_demo is

  type state_t is (IDLE, S1, S2, S3, S4, DONE_STATE);
  signal state : state_t := IDLE;

  signal mul_a, mul_b : signed(31 downto 0);
  signal mul_y        : signed(63 downto 0);

  signal add_a, add_b : signed(63 downto 0);
  signal add_y        : signed(63 downto 0);

  signal r_v1 : signed(63 downto 0);
  signal r_v2 : signed(63 downto 0);
  signal r_v3 : signed(63 downto 0);
  signal r_v4 : signed(63 downto 0);
  signal r_y  : signed(63 downto 0);

begin

  -- one shared multiplier entity
  MUL0: entity work.mul_unit
    port map(
      a => mul_a,
      b => mul_b,
      y => mul_y
    );

  -- one shared adder entity
  ADD0: entity work.add_unit
    port map(
      a => add_a,
      b => add_b,
      y => add_y
    );

  -- multiplexers select which operation uses the shared units
  process(state, a, b, c, d, e, f, r_v1, r_v2, r_v3, r_v4)
  begin
    mul_a <= (others => '0');
    mul_b <= (others => '0');
    add_a <= (others => '0');
    add_b <= (others => '0');

    case state is
      when S1 =>
        -- s1: v2 = c * d
        mul_a <= c;
        mul_b <= d;

      when S2 =>
        -- s2: v1 = a * b
        mul_a <= a;
        mul_b <= b;

      when S3 =>
        -- s3: v3 = e * f and v4 = v1 + v2
        mul_a <= e;
        mul_b <= f;
        add_a <= r_v1;
        add_b <= r_v2;

      when S4 =>
        -- s4: v5 = v3 + v4
        add_a <= r_v3;
        add_b <= r_v4;

      when others =>
        null;
    end case;
  end process;

  -- FSM controller: one state per FDS control step
  process(clk, rst)
  begin
    if rst = '1' then
      state <= IDLE;
      r_v1  <= (others => '0');
      r_v2  <= (others => '0');
      r_v3  <= (others => '0');
      r_v4  <= (others => '0');
      r_y   <= (others => '0');

    elsif rising_edge(clk) then
      case state is
        when IDLE =>
          if start = '1' then
            state <= S1;
          end if;

        when S1 =>
          r_v2 <= mul_y;
          state <= S2;

        when S2 =>
          r_v1 <= mul_y;
          state <= S3;

        when S3 =>
          r_v3 <= mul_y;
          r_v4 <= add_y;
          state <= S4;

        when S4 =>
          r_y <= add_y;
          state <= DONE_STATE;

        when DONE_STATE =>
          state <= IDLE;
      end case;
    end if;
  end process;

  y <= r_y;
  done <= '1' when state = DONE_STATE else '0';

end architecture;