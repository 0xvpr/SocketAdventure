#!/usr/bin/env bash

# Creator:    VPR
# Created:    May 9, 2022
# Updated:    May 9, 2022

# requires installation of vpr-pidjeon && python requirements

vpr-pidjeon-x86.exe "PwnAdventure3-Win32-Shipping.exe" ./lib/pwn-adv.dll -i ManualMap
./App.py
