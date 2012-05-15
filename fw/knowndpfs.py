# AX206 based DPF profiles
#
# Feel free to add your own and post this file..
#

KNOWN_DPFS = [
#***************** SUPPORTED DPFS ***************************

##################### 128 x 128 #############################
# Very bright and colourful TFT screen
	[ ('20090113', 'Nov 13 2010\xff\xff\xff\xff\xff', 'ProcTbl4'), # Version (old detection method)
		("pink", "yes"),             # Short ID, supported (yes/wip/no)
		"",                          # Description / URL(s)
	 	[ ],                         # LcdIniTbl CRCs, OpenWin CRCs (new detection method)
	],
	[ ('20090113', 'Sep 16 2010\xff\xff\xff\xff\xff', 'ProcTbl4'),
		("white", "yes"),
		"http://www.dealextreme.com/details.dx/sku.27893",
	 	[ ],
	],
	[ ('20090401', 'Oct 15 2010\xff\xff\xff\xff\xff', 'ProcTbl1'),
		("blue", "yes"),
		"http://www.dealextreme.com/details.dx/sku.27894",
	 	[ ],
	],
	[       () ,
		("agk_violet", "yes"),
		"",
	 	([ 0x54cc8527 ], [ 0xd5efd5d9 ]),
	],
# 2012-04-22 by rotidude
	[       () ,
		("delightdigi_black", "yes"),
		"black from ebay seller delight-digi",
	 	([ 0xda6ee4c3 ], [ 0xdda9c297 ]),
	],
# 2012-05-14 by donashe
	[       () ,
		("coby_dp151_white", "yes"),
		"http://www.frys.com/product/5439059",
	 	([ 0 ], [ 0xd455d959 ]),
	],
##################### 128 x 144 #############################
	[       () ,
		("focalprice144", "yes"),
		"",
		#Note: no LcdIniTbl
	 	([ 0 ], [ 0x305ef014 ]),
	],

##################### 240 x 320 #############################
	[       () ,
		("pearl (by scan)", "yes"),
		"http://www.pearl.de/a-HPM1184-5618.shtml",
	 	([ 0xba995aef ], [ 0x1dac2801 ]),	# LcdIniTbl CRCs, OpenWin CRCs
	],
	[ ('20090504', 'Mar 27 2010\xff\xff\xff\xff\xff', 'ProcTbl5' ),
		("pearl (by version)", "yes"),
		"http://www.pearl.de/a-HPM1184-5618.shtml",
	 	(),
	],

	[ ('20090504', 'Jul 24 2010\xff\xff\xff\xff\xff', 'ProcTbl5'),
		("focal", "yes"),
		("http://www.focalprice.com/detail_EX042W.html",
		 "http://www.ebay.com/itm/220885046238",
		 "http://www.ebay.de/itm/220885046238"
		 "http://www.ebay.de/itm/170808432027"
		),

	 	( [ 0xfbed1394, 0xa4343eb3 ], [ 0x174c10b9 ]),
	],
	[       () ,
		("linkdelight", "no"),
		("http://www.ebay.de/itm/ws/eBayISAPI.dll?ViewItem&rd=1&item=260978964426&ssPageName=STRK:MEAFB:IT",
		 "http://www.ebay.de/itm/New-Mini-2-4-inch-Digital-Time-Day-Calendar-Alarm-Clock-Photo-Frame-POF16-/260883908884?pt=LH_DefaultDomain_0&hash=item3cbde48914#ht_4709wt_1046"
		),
	 	([ 0x30149543, 0xef9bb8e3 ], [ 0x334bfc75 ]),
	],

#************** WIP: WORK IN PROGRESS ***********************
	[       () ,
		("dx21334", "wip"),
		"http://www.dealextreme.com/p/2-4-lcd-desktop-digital-photo-frame-and-calendar-27-picture-memory-storage-21334",
	 	([ 0x53c2379b ], [ 0xe7bb71fc ]),
	],

#**************** UNSUPPORTED DPFS **************************

]
