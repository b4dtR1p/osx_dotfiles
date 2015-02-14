#!/usr/bin/env python
# -*- coding: utf-8 -*-

########## begin config ##########

output = 'homepage.html'      # output file
title = 'Homepage'          # page title
font = 'fixed'  # font
separator = '    »  '   	# separator between group title and links
#colors = (
#    '#181512',  # background
#    '#bea492',  # links
#    '#646a6d',  # group title 
#    '#8c644c',  # separator
#)
colors = (
    '#181512',  # background
    '#bea492',  # links
    '#9a875f',  # group title 
    '#504339',  # separator
)

# add your links here
links = {
		'search': [
			['google', 'https://www.google.com/'],
			['duckduckgo', 'http://duckduckgo.com/'],
			['wiki', 'http://en.wikipedia.org/wiki/Main_Page'],
			['images', 'https://images.google.com/'],
		],
		'social': [
			['twitter', 'http://www.twitter.com'],
			['gmail', 'http://mail.google.com'],
			['youtube', 'http://www.youtube.com/'],
			['tumblr', 'http://www.tumblr.com/'],
			['github', 'http://www.github.com/'],
			['last.fm', 'http://www.last.fm/'],
			['deviantart', 'http://www.deviantart.com/'],
		],
		'news': [
			['reddit', 'http://www.reddit.com'],
			['hackernews', 'http://news.ycombinator.com'],
			['arch', 'http://bbs.archlinux.org'],
			['crunch', 'http://www.crunchbanglinux.org/forums'],
			['omgubuntu', 'http://www.omgubuntu.co.uk'],
		],
		'private': [
                        ['vps', 'http://b4dtr1p.tk'],
                        ['git', 'http://git.b4dtr1p.tk'],
		],
}

########## end config ##########

import os

css = '''body {
  background-color: %s;
  font-family: %s;
  font-size: %s;
  font-weight: normal;
  margin-left: 7%%;
}
a:link,a:visited,a:active {
  text-decoration: none;
  color: %s;
  font-weight: normal;
}
a:hover {
  text-decoration: underline;
  color: %s;
  font-weight: normal;
}
table {
  border-spacing: 8px;
}
td:first-child {
  font-weight: bold;
  color: %s
}
td:nth-child(2) {
  font-weight: normal;
  color: %s;
}''' % (colors[0], font[0], font[1], colors[1], colors[1], colors[2], colors[3])

links_html = ''
for group in links:
    links_html += '<tr><td align="right">%s</td><td>%s</td><td>' % (group, separator)
    for site in links[group]:
        links_html += '<a href="%s">%s</a> ' % (site[1], site[0])
    links_html += '</td></tr>'

html = '''<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
  <title>%s</title>
  <link rel="stylesheet" type="text/css" href="style.css" />
</head>
<body>
  <table valign="middle" border="0" width="100%%" height="100%%"><tr><td><table>
    %s
  </table></td></tr></table>
</body>
</html>''' % (title, links_html)

with open(output, 'w') as file:
    file.write(html)
with open('style.css', 'w') as file:
    file.write(css)

os.system('tidy -utf8 -i -m -q -asxhtml %s' % output)
