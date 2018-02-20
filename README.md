# PowerUpSwitch

This project contains the firmware for the FMS Powerup Switch control unit.  It sends plate ownership commands to the faux FMS server located in the PowerUpFMSServer project and controls LEDs for each plate on the switch.  We used magnetic reed switches to detect when a plate was owned.

We used the Arduino IDE 1.8.5 to develop and build this project on Mac OSX 10.12.5.  The board targeted was an Arduino UNO R3 with an Ethernet shield.  The parts list follows.

The firmware can acquire an IP address using DHCP, or via a jumper on the board (see comments in program code) use a static IP hardcoded in the application.  It registers itself to the server application so that the server can send commands back (like randomizing the field, setting the state to play the game, safe, and staff-safe LED states).

There is also a jumper setting that defines which alliance's switch the controller is controlling (red or blue).  This orients the plates so that the LEDs can show the correct display when a plate is owned.

2x Screw Shield https://www.amazon.com/gp/product/B00UT0QLJA/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1  
LED Plug Connectors https://www.amazon.com/gp/product/B01AHJMR4A/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1  
Power Plugs https://www.amazon.com/gp/product/B076SXZK7M/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1  
Magnetic Switches https://www.amazon.com/gp/product/B00LYCUSBY/ref=oh_aui_detailpage_o04_s01?ie=UTF8&psc=1  
2x Arduino & Ethernet Shield https://www.amazon.com/gp/product/B00JB2Z74W/ref=od_aui_detailpages00?ie=UTF8&psc=1  
2x Power Supply https://www.amazon.com/gp/product/B017YEOAPA/ref=od_aui_detailpages00?ie=UTF8&psc=1  
2x LED Strips https://www.amazon.com/gp/product/B00ZHB9M6A/ref=od_aui_detailpages00?ie=UTF8&psc=1  

[Schematic](Schematic-Switch.jpg)
