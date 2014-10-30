# -*- coding: utf-8 -*-
"""
Created on Thu Oct 30 13:44:13 2014

@author: jan
"""

import sys

_texTopLevel = 1
_texUseTabularx = True

__texLevels = ['chapter',
               'section',
               'subsection',
               'subsubsection',
               'paragraph',
               'subparagraph']
             
__long_desc = []
__long_desc_cntr = 0

###############################################################################
def print_usage(file_name):
    print "Usage: python " + file_name + " <.db file>"


###############################################################################
def parse_record(record):
    recType = record.split('(')[1].split(',')[0]
    recName = record.split('"')[1]
    
    fields = record.split('field')[1:]
    
    recFields = []
    for field in fields:
        fName = field.split('(')[1].split(',')[0].strip()
        fVal  = field.split('"')[1]
        
        fVal  = fVal.replace("%", "\\%")
        fVal  = fVal.replace("&", "\\&")
        
        recFields.append((fName, fVal))
        
    return (recName, recType, recFields)
    
###############################################################################
def parse_long_desc(fileStr):
    global __long_desc
    lines = fileStr.split("\n")
    
    armed = False
    desc = ""
    
    for line in lines:
        line = line.strip()
        if(line[0:2] == "##"):
            desc += line[2:]
            armed = True
        elif armed:
            armed = False
            __long_desc.append( desc )
            desc = ""

    
###############################################################################
def escape_dollar_sign(string):
    return string.replace('$', '\\$')
    
###############################################################################
def escape_underscore(string):
    return string.replace('_', '\\_')
        
    
###############################################################################
def tex_record(recName, recType, recFields):
    global __long_desc, __long_desc_cntr
    string = ""
    
    # Record name
    string += "\\"+__texLevels[_texTopLevel+1]+"{" + recName + "}\n"
    
    # Record type
    string += "\\textbf{Record Type}: "+recType+" \\newline \\newline \n"
    
    # Fields
    string += "\\textbf{Fields}: \\newline \n"
    
    if _texUseTabularx:
        string += "\\begin{tabularx}{0.7\\linewidth}{|r|X|}\n"
    else:
        string += "\\begin{tabular}{|r|l|}\n"
        
    string += "\\hline Field & Value \\\\\n"
    string += "\\hline\n"
    
    for field in recFields:
        string += "\\hline\n"
        string += field[0]
        string += " & "
        string += field[1]
        string += "\\\\\n"
         
    string += "\\hline\n"
 
    if _texUseTabularx:
        string += "\\end{tabularx}\n"
    else:
        string += "\\end{tabular}\n"
 
    # Long description
    string += "\\newline \\newline \\newline\n" 
    string += "\\textbf{Long description}: \\newline \n"
    
    string += __long_desc[__long_desc_cntr] + "\n"
    __long_desc_cntr += 1
    
    # Space at end
    string += "\\newline \\newline\n"
    string += "\n\n"
    
    # Some LateX spceific escapes
    string = escape_dollar_sign(string)
    string = escape_underscore(string)
    
    return string
    
    
###############################################################################
fileStr = ""
def parse_file(dbFile):
    global fileStr
    print "Opening file: ", dbFile

    f = open(dbFile, 'r')
    fileStr = f.read()
    f.close()    
    
    parse_long_desc(fileStr)
    
    records = fileStr.split('record')[1:]
    
    texFile = dbFile.split('/')[-1].replace('.db', '.tex')
    f = open(texFile, 'w')  
    
    for record in records:
        recName, recType, recFields = parse_record(record)
        f.write(tex_record(recName, recType, recFields))
        
    f.close()
    
 
###############################################################################   
if __name__ == '__main__':
    if(len(sys.argv) < 2):
        print_usage(sys.argv[0])
        sys.exit()

    dbFile = sys.argv[-1]
    if(dbFile.split('.')[-1] != 'db'):
        print_usage(sys.argv[0])
        sys.exit()
        
    parse_file(dbFile)
        