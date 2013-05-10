#!/usr/bin/python
#
# Simple knowntypes.py builder
#

from __future__ import print_function
import sys
import os
import re
import cgi

LCD_SOURCE = "../src/lcd"

def writecomment(file, dict, key):
        if key in dict:
                for s in dict[key]:
                        file.write('\n# %s' % s.strip())
#
#
# MAIN


if not os.path.exists(LCD_SOURCE):
        print("Source directory '%s' not found!" % LCD_SOURCE)
        sys.exit(-1)

dirs = os.listdir(LCD_SOURCE)
dirs.sort()

# Parse all dpfmodel.h to alltypes
alltypes = []
dpf_count = len(dirs)
dpf_ok = 0

for dpf in dirs:
        if dpf.startswith("."):
                dpf_count -= 1
                continue
        dir = os.path.join(LCD_SOURCE, dpf)
        header = os.path.join(dir, "dpfmodel.h")
        if not os.path.exists(header):
                print("No dpfmodel.h in dir '%s' -- skipping." % dir)
                continue

        # Read dpfmodel.h
        fh = open(header, "r")
        data = fh.read()
        fh.close()

        typedict = {}

        #Keyword "Type"
        keys = re.findall(r'^// Type: (.+)$', data, re.MULTILINE)
        if len(keys) == 0:
                print("No 'Type' keyword in %s -- skipping." % header)
                continue
        if len(keys) > 1:
                print("More than one 'Type' keyword in %s -- skipping." % header)
                continue
        if keys[0] != dpf:
                print("'Type' keyword (%s) does not match dirname in %s -- skipping." % (keys[0], header))
                continue
        typedict['Type'] = keys

        #Keyword "Comment"
        keys = re.findall(r'^// Comment: (.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['Comment'] = keys

        #Keyword "Status"
        keys = re.findall(r'^// Status: (.+)$', data, re.MULTILINE)
        if len(keys) == 0:
                keys = ['supported']
        if len(keys) > 1:
                print("More than one 'Status' keyword in %s -- skipping." % header)
                continue
        s = keys[0].strip().lower()
        if s != "supported" and s != "wip" and s != "unsupported":
                print("Illegal 'Status' value in %s use supported/unsupported/wip -- skipping." % header)
        typedict['Status'] = s

        #Keyword "Credit"
        keys = re.findall(r'^// Credit: (.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['Credit'] = keys

        #Keyword "Date"
        keys = re.findall(r'^// Date: (.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['Date'] = keys
                
        #Keyword "Url"
        keys = re.findall(r'^// Url: (.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['Url'] = keys

        #Keyword "CRC"
        keys = re.findall(r'^// CRC: (.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                a1 = []
                for s in keys:
                        a2 = []
                        for i in s.strip().split(","):
                                a2.append(int(i, 0))
                        a1.append(a2)
                typedict['CRC'] = a1

        #Keyword "Version"
        keys = re.findall(r'^// Version: (.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                if len(keys) > 1:
                        print("More than one 'Version' keyword in %s -- skipping." % header)
                        continue
                a = []
                for s in keys[0].split(","):
                        a.append(s.strip().replace("'", "").replace('\\xff', '\xff'))
                typedict['Version'] = a

        #Lcd size
        keys = re.findall(r'#[ \t]*define[ \t]+LCD_WIDTH[ \t]+([0-9]+)L*[ \t]*$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['Lcd_width'] = keys[0]
        else:
                typedict['Lcd_width'] = '?unkown?'
        keys = re.findall(r'#[ \t]*define[ \t]+LCD_HEIGHT[ \t]+([0-9]+)L*[ \t]*$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['Lcd_height'] = keys[0]
        else:
                typedict['Lcd_height'] = '?unkown?'
                
        #Contrast support -- 0.4 style
        keys = re.findall(r'[^/]#[ \t]*define[ \t]+(LCD_DEFAULT_CONTRAST_(TABLE)|(CUSTOM).+)$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['ContrastSupport'] = 'Yes'
        else:
                #Contrast support -- pre 0.4 style
                keys = re.findall(r'[^/]#[ \t]*define[ \t]+(LCD_DEFAULT_CONTRAST_VALUE.+)$', data, re.MULTILINE)
                if len(keys) != 0:
                        typedict['ContrastSupport'] = 'Yes'
                else:
                        typedict['ContrastSupport'] = 'No'

        #Contrast menu support
        keys = re.findall(r'[^/]#[ \t]*define[ \t]+(LCD_USER_ADJUSTABLE_CONTRAST.+)$', data, re.MULTILINE)
        if len(keys) != 0:
                typedict['ContrastMenuSupport'] = 'Yes'
        else:
                typedict['ContrastMenuSupport'] = 'No'

        alltypes.append(typedict)

        dpf_ok += 1


# Generate knowntypes.py

alltypes = sorted(alltypes, key=lambda k: (k['Status'], k['Lcd_width'], k['Lcd_height'], k['Type']))

kt = open("knowntypes.py", "w")

kt.write('#\n# This file is autogenerated by knowntypes_builder.py\n#\n')
kt.write('# DO NOT EDIT!\n#\n\n')
kt.write('KNOWN_TYPES = [')
lastwidth = None
lastheight = None
laststatus = None
for typedict in alltypes:
        if laststatus != typedict['Status']:
                lastsize = None
                laststatus = typedict['Status']
                if laststatus == 'supported':
                        kt.write('\n\n#***************** SUPPORTED DPFS ***************************\n')
                elif laststatus == 'wip':
                        kt.write('\n\n#************** WIP: WORK IN PROGRESS ***********************\n')
                elif laststatus == 'unsupported':
                        kt.write('\n\n#**************** UNSUPPORTED DPFS **************************\n')
        if lastwidth != typedict['Lcd_width'] or lastheight != typedict['Lcd_height']:
                lastwidth = typedict['Lcd_width']
                lastheight = typedict['Lcd_height']
                kt.write('\n###################### %s x %s ##############################\n' % (lastwidth, lastheight))
        kt.write('\n#')
        writecomment(kt, typedict, 'Type')
        kt.write('\n#')
        writecomment(kt, typedict, 'Comment')
        writecomment(kt, typedict, 'Credit')
        writecomment(kt, typedict, 'Date')
        kt.write('\n#\n')
        kt.write(str(typedict))
        kt.write(',\n')
kt.write('\n]\n')
kt.close

# Generate knowntypes.html

kt = open("knowntypes.html", "w")

kt.write('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"\n   "http://www.w3.org/TR/html4/strict.dtd">\n')
kt.write('<HTML><HEAD><meta http-equiv="Content-type" content="text/html;charset=UTF-8">\n')
kt.write('<style type="text/css">\n')
kt.write('<!--\n')
kt.write('table.types { border: 1px solid; border-collapse: collapse; border-spacing: 0; }\n')
kt.write('table.types td { border: 1px solid;  padding: .1em .25em; }\n')
kt.write('table.types th { border: 1px solid; padding: .1em .25em; }\n')
kt.write('-->\n')
kt.write('</style>\n')
kt.write('<TITLE>dpf-ax: known models</TITLE></HEAD>\n')
kt.write('<BODY><H1>Models supported by dpf-ax</H1>\n')
kt.write('<table class="types">\n')
kt.write('<tr><th>Type</th><th>Size (w x h)</th><th>Contrast Support</th><th>Link(s)</th><th>Comment</th>\n')
for typedict in alltypes:
        if typedict['Status'] == 'supported':
                kt.write('<tr>')
                kt.write('<td>%s</td>' % cgi.escape(typedict['Type'][0]))
                kt.write('<td>%s x %s</td>' % (typedict['Lcd_width'], typedict['Lcd_height']))
                kt.write('<td>')
                if typedict['ContrastSupport'] == 'Yes':
                        if typedict['ContrastMenuSupport'] == 'Yes':
                                kt.write('In menu')
                        else:
                                kt.write('Yes')
                else:
                        kt.write('No')
                kt.write('</td>')
                kt.write('<td>')
                if 'Url' in typedict:
                        for s in typedict['Url']:
                                kt.write('<a href="%s">Link</a>\n' % cgi.escape(s))
                kt.write('</td>')
                kt.write('<td>')
                if 'Comment' in typedict:
                        for s in typedict['Comment']:
                                kt.write('%s\n' % cgi.escape(s))
                kt.write('</td>')
                kt.write('</tr>')
kt.write('</table>')
kt.write('</BODY></HTML>\n')
kt.close

print("%d types written to knowntypes.py" % dpf_ok, end=' ')
if dpf_ok < dpf_count:
        print("(detected: %d, errors: %d)" % (dpf_count, dpf_count - dpf_ok))
        sys.exit(1)
else:
        print()
