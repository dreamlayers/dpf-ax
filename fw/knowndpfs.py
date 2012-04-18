# AX206 based DPF profiles
#
# Feel free to add your own and post this file..
#

KNOWN_DPFS = [
# Very bright and colourful TFT screen
	[ ('20090113', 'Nov 13 2010\xff\xff\xff\xff\xff', 'ProcTbl4'), # Version
		"pink",                                                    # Short ID
		"",                                                        # URL
	 	[ ],
	],
	[ ('20090113', 'Sep 16 2010\xff\xff\xff\xff\xff', 'ProcTbl4'), # Version
		"DX_white",                                                # Short ID
		"http://www.dealextreme.com/details.dx/sku.27893",
	 	[ ],
	],
	[ ('20090401', 'Oct 15 2010\xff\xff\xff\xff\xff', 'ProcTbl1'),
		"DX_blue",
		"http://www.dealextreme.com/details.dx/sku.27894",
	 	[ ],
	],
	[       () ,
		"pearl (by scan)",
		"http://www.pearl.de/a-HPM1184-5618.shtml",
	 	([ 0xba995aef ], [ 0x1dac2801 ]),	# LcdIniTbl CRCs, OpenWin CRCs
	],
	[ ('20090504', 'Mar!27 2010\xff\xff\xff\xff\xff', 'ProcTbl5' ),
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
# Bad quality, weak screen. Not recommended.
	[ ('20090401', 'Nov 29 2010\xff\xff\xff\xff\xff', 'ProcTbl1' ),
		"Ebay_silver",
		"",
	 	[ ],
	],
	[ ('20090113', 'Aug 26 2010\xff\xff\xff\xff\xff', 'ProcTbl2'),
		"DX_black",
		"http://www.dealextreme.com/details.dx/sku.16133",
	 	[ ],
	],
	[       () ,
		"linkdelight",
		("http://www.ebay.de/itm/ws/eBayISAPI.dll?ViewItem&rd=1&item=260978964426&ssPageName=STRK:MEAFB:IT",
		 "http://www.ebay.de/itm/New-Mini-2-4-inch-Digital-Time-Day-Calendar-Alarm-Clock-Photo-Frame-POF16-/260883908884?pt=LH_DefaultDomain_0&hash=item3cbde48914#ht_4709wt_1046"
		),
	 	([ 0x30149543, 0xef9bb8e3 ], [ 0x334bfc75 ]),
	],
]
