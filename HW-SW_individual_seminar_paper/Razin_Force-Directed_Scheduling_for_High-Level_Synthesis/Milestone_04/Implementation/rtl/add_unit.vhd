library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity add_unit is
  port(
    a : in  signed(63 downto 0);
    b : in  signed(63 downto 0);
    y : out signed(63 downto 0)
  );
end entity;

architecture rtl of add_unit is
begin
  y <= a + b;
end architecture;