#!/bin/bash

# $1 should be /sys/class/pwm/pwmchip0
# $2 should be 0 or 1

# sudo ./play.sh /sys/class/pwm/pwmchip0 0

device=$1
output=$2
pwm=$device/pwm$output

echo Setting up $pwm...
echo $output > $device/export

function play_note () {
  echo $1 > $pwm/period
  echo $2 > $pwm/duty_cycle
  echo 1 > $pwm/enable
  sleep $3
  echo 0 > $pwm/enable
}

c_period=3822192;c_duty_cyle=1911096
cs_period=3607764;cs_duty_cyle=1803882
d_period=3405299;d_duty_cyle=1702649
ds_period=3214091;ds_duty_cyle=1607045
e_period=3033704;e_duty_cyle=1516852
f_period=2863442;f_duty_cyle=1431721
fs_period=2702776;fs_duty_cyle=1351388
g_period=2551020;g_duty_cyle=1275510
gs_period=2407898;gs_duty_cyle=1203949
a_period=2272727;a_duty_cyle=1136364
as_period=2145186;as_duty_cyle=1072593
b_period=2024783;b_duty_cyle=1012392
c2_period=1911132;c2_duty_cyle=955566

echo playing...
play_note $c_period $c_duty_cyle 1
play_note $d_period $d_duty_cyle 1
play_note $e_period $e_duty_cyle 1
play_note $f_period $f_duty_cyle 1
play_note $g_period $g_duty_cyle 1
play_note $a_period $a_duty_cyle 1
play_note $b_period $b_duty_cyle 1
play_note $c2_period $c2_duty_cyle 1
play_note $b_period $b_duty_cyle 1
play_note $a_period $a_duty_cyle 1
play_note $g_period $g_duty_cyle 1
play_note $f_period $f_duty_cyle 1
play_note $e_period $e_duty_cyle 1
play_note $d_period $d_duty_cyle 1
play_note $c_period $c_duty_cyle 1

echo shutting down...
echo $output > $device/unexport