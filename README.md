# tinygang
All things related to tiny gang activites. TinyGang is an open source wearable project consisting of a set of jackets that communicate wirelessly with each other through proximity and motion. The patterns of the group change depending on the mode, number of people, duration and other elements to be defined by the group wearing the jackets.

## jackets

### ESP Thing Hardware Version

Codename : TrackStar :

- Finally, the classic sst with the trefoil is my favorite style ;) adidas Originals Men's Superstar Track Jacket, Black, L : https://www.amazon.com/dp/B072QBTS5S/ref=cm_sw_r_cp_api_i_Z6zFDbDNM03P9

- SparkFun Esp Thing : https://www.sparkfun.com/products/13907
- Dip Switch for mode select : 
- Usb Battery : 
- McMaster strapping for led reinforcement : https://www.mcmaster.com/14495t77
- 3/8 in heatshrink clear heatshrink for additional LED protection
- LEDS 4m per jacket : https://www.aliexpress.com/item/32627143856.html


other options:
- foral adidas small : https://www.amazon.com/adidas-Originals-Womens-Superstar-Tracktop/dp/B077VRYSYT
- mesh : https://www.hibbett.com/adidas-originals-tulle-jacket/A1185.html
- Fray Check Liquid Seam Sealant : https://www.amazon.com/dp/B000YQKIDY/ref=cm_sw_r_cp_api_i_NVzFDbZ252J34



### Teensy Hardware Version  

Base Setup:

- Teensy 3.2 - https://www.sparkfun.com/products/13736
- XBee 900 Pro - https://www.sparkfun.com/products/11216 ([configuration utility](https://www.digi.com/products/iot-platform/xctu))
- WS2801 LED Chain x 2 - https://www.adafruit.com/product/738 
  4-wire (power, ground, clock, data)
- 4 Pin JST Connectors - https://www.sparkfun.com/products/14576
- Teensy 3.1 XBee Adapter Board - https://www.sparkfun.com/products/13311
- USB Battery (5v with higher amp) - eg: https://www.amazon.com/Anker-PowerCore-Ultra-Compact-High-Speed-Technology/dp/B01CU1EC6Y/

Sensors (optional):

- Teensy Prop Shield with motion sensor ([calibrated](https://www.pjrc.com/store/prop_shield.html))

Jacket Integration:

- Grommets - 7/16th hole or Size 3 eg: https://www.amazon.com/50-QTY-C-S-Osborne-No-G1-3-Grommets/dp/B01MRHW94D/
- Grommet press / hammer kit

## steggos

some hackaday article about laser cut led diffusion :
http://imakethin.gs/blog/glowy-dinosaur-hoody

@metal and some 3d printing later..
https://www.thingiverse.com/thing:30800
and remixes ensued: https://www.thingiverse.com/thing:293796
https://www.thingiverse.com/thing:3327694


![alt text](https://raw.githubusercontent.com/adellelin/tinygang/master/img/steggo.png)
[@mpinner climbing the led wall from @mbientlab 
Steggo led hoodies are born and joining the tinygang of led wearables together. #makerfaire #climbies](https://flic.kr/p/ekGJMU)


## the code

as the intent is for all wearers or builders of tinygang to participate, the programming environment is pretty open.  you are encouraged to create and alter your own patterns.

banks of patterns are provided. you can definitely change these. only your patterns are ever shown, so remove or revise what you don't like. 

when your jacket excites all the other jackets will excite with your given mode. you can select your mode via little dip switches on the circuit board. 

each pattern is assigned a mode. in our code (todo: config file) we assign a pattern to each mode.  each person only shows their bank of patterns for each mode excited. 

the goal is people add more patterns and variety when they enter the proximity of tiny gang.  for this we need a variety of people with a variety of modes selected. another thing happens when everyone is on the same mode. the pattern fills up so it is nearly always on. still working out if there is a way to more dynamically assign people to mode. we'll see what you come up with.

side note, there is also some means of color handling and the ability to send that around live. it has only been tested with one jacket that had the teensy motion sensor. it was able to change all the colors for all the jackets. it was a bit too much, really.

additional works can include sensors to learn about the dynamic of social interactions, the mood of a room, or interactivity research.








## social links

#tinygang

http://twitter.com/tinyjacketgang

https://www.instagram.com/tinyjacketgang/

