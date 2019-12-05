# coding=utf8
# the above tag defines encoding for this document and is for Python 2.x compatibility

import re

regex = r"S\([-]?[0-9]+"

test_str = ("{//Knight\n"
	"{S(-175,-96),S(-92,-65),S(-74,-49),S(-73,-21)},\n"
	"{S(-77,-67),S(-41,-54),S(-27,-18),S(-15,8)},\n"
	"{S(-61,-40),S(-17,-27),S(6,-8),S(12,29)},\n"
	"{S(-35,-35),S(8,-2),S(40,13),S(49,28)},\n"
	"{S(-34,-45),S(13,-16),S(44,9),S(51,39)},\n"
	"{S(-9,-51),S(22,-44),S(58,-16),S(53,17)},\n"
	"{S(-67,-69),S(-27,-50),S(4,-51),S(37,12)},\n"
	"{S(-201,-100),S(-83,-88),S(-56,-56),S(-26,-17)}\n"
	"},\n"
	"{//Bishop\n"
	"{S(-53,-57),S(-5,-30),S(-8,-37),S(-23,-12)},\n"
	"{S(-15,-37),S(8,-13),S(19,-17),S(4,1)},\n"
	"{S(-7,-16),S(21,-1),S(-5,-2),S(17,10)},\n"
	"{S(-5,-20),S(11,-6),S(25,0),S(39,17)},\n"
	"{S(-12,-17),S(29,-1),S(22,-14),S(31,15)},\n"
	"{S(-16,-30),S(6,6),S(1,4),S(11,6)},\n"
	"{S(-17,-31),S(-14,-20),S(5,-1),S(0,1)},\n"
	"{S(-48,-46),S(1,-42),S(-14,-37),S(-23,-24)}\n"
	"},\n"
	"{//Rook\n"
	"{S(-31,-9),S(-20,-13),S(-14,-10),S(-5,-9)},\n"
	"{S(-21,-12),S(-13,-9),S(-8,-1),S(6,-2)},\n"
	"{S(-25,6),S(-11,-8),S(-1,-2),S(3,-6)},\n"
	"{S(-13,-6),S(-5,1),S(-4,-9),S(-6,7)},\n"
	"{S(-27,-5),S(-15,8),S(-4,7),S(3,-6)},\n"
	"{S(-22,6),S(-2,1),S(6,-7),S(12,10)},\n"
	"{S(-2,4),S(12,5),S(16,20),S(18,-5)},\n"
	"{S(-17,18),S(-19,0),S(-1,19),S(9,13)}\n"
	"},\n"
	"{//Queen\n"
	"{S(3,-69),S(-5,-57),S(-5,-47),S(4,-26)},\n"
	"{S(-3,-55),S(5,-31),S(8,-22),S(12,-4)},\n"
	"{S(-3,-39),S(6,-18),S(13,-9),S(7,3)},\n"
	"{S(4,-23),S(5,-3),S(9,13),S(8,24)},\n"
	"{S(0,-29),S(14,-6),S(12,9),S(5,21)},\n"
	"{S(-4,-38),S(10,-18),S(6,-12),S(8,1)},\n"
	"{S(-5,-50),S(6,-27),S(10,-24),S(8,-8)},\n"
	"{S(-2,-75),S(-2,-52),S(1,-43),S(-2,-36)}\n"
	"},\n"
	"{//King\n"
	"{S(271,1),S(327,45),S(270,85),S(192,76)},\n"
	"{S(278,53),S(303,100),S(230,133),S(174,135)},\n"
	"{S(195,88),S(258,130),S(169,169),S(120,175)},\n"
	"{S(164,103),S(190,156),S(138,172),S(98,172)},\n"
	"{S(154,96),S(179,166),S(105,199),S(70,199)},\n"
	"{S(123,92),S(145,172),S(81,184),S(31,191)},\n"
	"{S(88,47),S(120,121),S(65,116),S(33,131)},\n"
	"{S(59,11),S(89,59),S(45,73),S(-1,78)}\n"
	"}")

matches = re.finditer(regex, test_str, re.MULTILINE)

for matchNum, match in enumerate(matches, start=1):
    print match.group().replace("S(",""),
    if (matchNum)%4 != 0:
    	print ",",
    if (matchNum)%4 == 0:
    	print "}"    
    	print "{",
# Note: for Python 2.7 compatibility, use ur"" to prefix the regex and u"" to prefix the test string and substitution.