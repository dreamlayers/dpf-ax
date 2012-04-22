# AX206 based DPF profiles
#
# Feel free to add your own and post this file..
#

KNOWN_DPFS = [
##################### 128 x 128 #############################
# Very bright and colourful TFT screen
	[ ('20090113', 'Nov 13 2010\xff\xff\xff\xff\xff', 'ProcTbl4'), # Version
		"pink",                                                    # Short ID
		"",                                                        # URL
	 	[ ],
	],
	[ ('20090113', 'Sep 16 2010\xff\xff\xff\xff\xff', 'ProcTbl4'), # Version
		"white",                                                # Short ID
		"http://www.dealextreme.com/details.dx/sku.27893",
	 	[ ],
	],
	[ ('20090401', 'Oct 15 2010\xff\xff\xff\xff\xff', 'ProcTbl1'),
		"blue",
		"http://www.dealextreme.com/details.dx/sku.27894",
	 	[ ],
	],
	[       () ,
		"agk_violet",
		"",
	 	([ 0x54cc8527 ], [ 0xd5efd5d9 ]),
	],
# 2012-04-22 by rotidude
	[       () ,
		"delightdigi_black",
		"black from ebay seller delight-digi",
	 	([ 0xda6ee4c3 ], [ 0xdda9c297 ]),
	],
##################### 128 x 144 #############################
	[       () ,
		"focalprice144",
		"",
		#Note: no LcdIniTbl
	 	([ 0 ], [ 0x305ef014 ]),
	],

##################### 240 x 320 #############################
	[       () ,
		"pearl (by scan)",
		"http://www.pearl.de/a-HPM1184-5618.shtml",
	 	([ 0xba995aef ], [ 0x1dac2801 ]),	# LcdIniTbl CRCs, OpenWin CRCs
	],
	[ ('20090504', 'Mar 27 2010\xff\xff\xff\xff\xff', 'ProcTbl5' ),
		"pearl (by version)",
		"http://www.pearl.de/a-HPM1184-5618.shtml",
	 	(),
	],

	[ ('20090504', 'Jul 24 2010\xff\xff\xff\xff\xff', 'ProcTbl5'),
		"focal",
		("http://www.focalprice.com/detail_EX042W.html",
		 "http://www.ebay.com/itm/220885046238",
		 "http://www.ebay.de/itm/220885046238"
		 "http://www.ebay.de/itm/170808432027"
		),

	 	( [ 0xfbed1394, 0xa4343eb3 ], [ 0x174c10b9 ]),
	],
	[       () ,
		"linkdelight",
		("http://www.ebay.de/itm/ws/eBayISAPI.dll?ViewItem&rd=1&item=260978964426&ssPageName=STRK:MEAFB:IT",
		 "http://www.ebay.de/itm/New-Mini-2-4-inch-Digital-Time-Day-Calendar-Alarm-Clock-Photo-Frame-POF16-/260883908884?pt=LH_DefaultDomain_0&hash=item3cbde48914#ht_4709wt_1046"
		),
	 	([ 0x30149543, 0xef9bb8e3 ], [ 0x334bfc75 ]),
	],
]
