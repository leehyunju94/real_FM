import fontforge
font = fontforge.font() 
glyph = font.createMappedChar('A') 
glyph.importOutlines('e/a.png')
glyph.autoTrace()
glyph = font.createMappedChar('B') 
glyph.importOutlines('e/b.png')
glyph.autoTrace()
glyph = font.createMappedChar('C') 
glyph.importOutlines('e/c.png')
glyph.autoTrace()
glyph = font.createMappedChar('D') 
glyph.importOutlines('e/d.png')
glyph.autoTrace()
glyph = font.createMappedChar('E') 
glyph.importOutlines('e/e.png')
glyph.autoTrace()
glyph = font.createMappedChar('F') 
glyph.importOutlines('e/f.png')
glyph.autoTrace()
glyph = font.createMappedChar('G') 
glyph.importOutlines('e/g.png')
glyph.autoTrace()
glyph = font.createMappedChar('H') 
glyph.importOutlines('e/h.png')
glyph.autoTrace()
glyph = font.createMappedChar('I') 
glyph.importOutlines('e/i.png')
glyph.autoTrace()
glyph = font.createMappedChar('J') 
glyph.importOutlines('e/j.png')
glyph.autoTrace()
glyph = font.createMappedChar('K') 
glyph.importOutlines('e/k.png')
glyph.autoTrace()
glyph = font.createMappedChar('L') 
glyph.importOutlines('e/l.png')
glyph.autoTrace()
glyph = font.createMappedChar('M') 
glyph.importOutlines('e/m.png')
glyph.autoTrace()
glyph = font.createMappedChar('N') 
glyph.importOutlines('e/n.png')
glyph.autoTrace()
glyph = font.createMappedChar('O') 
glyph.importOutlines('e/o.png')
glyph.autoTrace()
glyph = font.createMappedChar('P') 
glyph.importOutlines('e/p.png')
glyph.autoTrace()
glyph = font.createMappedChar('Q') 
glyph.importOutlines('e/q.png')
glyph.autoTrace()
glyph = font.createMappedChar('R') 
glyph.importOutlines('e/r.png')
glyph.autoTrace()
glyph = font.createMappedChar('S') 
glyph.importOutlines('e/s.png')
glyph.autoTrace()
glyph = font.createMappedChar('T') 
glyph.importOutlines('e/t.png')
glyph.autoTrace()
glyph = font.createMappedChar('U') 
glyph.importOutlines('e/u.png')
glyph.autoTrace()
glyph = font.createMappedChar('V') 
glyph.importOutlines('e/v.png')
glyph.autoTrace()
glyph = font.createMappedChar('W') 
glyph.importOutlines('e/w.png')
glyph.autoTrace()
glyph = font.createMappedChar('X') 
glyph.importOutlines('e/x.png')
glyph.autoTrace()
glyph = font.createMappedChar('Y') 
glyph.importOutlines('e/y.png')
glyph.autoTrace()
glyph = font.createMappedChar('Z') 
glyph.importOutlines('e/z.png')
glyph.autoTrace()
font.save('e/newf.sfd') 
font.generate('e/testfont.otf') 