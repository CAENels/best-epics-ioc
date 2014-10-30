# -*- coding: utf-8 -*-
"""
Created on Thu Oct 30 13:44:13 2014

@author: jan
"""

import sys

_texTopLevel = 2
_texUseTabularx = True

__texLevels = ['chapter',
               'section',
               'subsection',
               'subsubsection',
               'paragraph',
               'subparagraph']

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
        
        recFields.append((fName, fVal))
        
    return (recName, recType, recFields)
    
    
###############################################################################
def escape_dollar_sign(string):
    return string.replace('$', '\\$')
    
###############################################################################
def escape_underscore(string):
    return string.replace('_', '\\_')
        
    
###############################################################################
def tex_record(recName, recType, recFields):
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
    
    string += """Lorem ipsum dolor sit amet, consectetur adipiscing elit. 
    Maecenas a nulla tincidunt, rutrum neque at, pharetra arcu. 
    Cras malesuada leo vitae mi laoreet iaculis. Suspendisse 
    rutrum vel quam consequat tincidunt. Aliquam ultrices, mi 
    ac ullamcorper cursus, lorem tellus consectetur ante, non 
    luctus nisl tellus ut ipsum. In hac habitasse platea dictumst. \n"""
    
    # Space at end
    string += "\\newline \\newline \\newline\n"
    string += "\n\n"
    
    # Some LateX spceific escapes
    string = escape_dollar_sign(string)
    string = escape_underscore(string)
    
    return string
    
    
###############################################################################
def parse_file(dbFile):
    print "Opening file: ", dbFile

    f = open(dbFile, 'r')
    fileStr = f.read()
    f.close()    
    
    records = fileStr.split('record')[1:]
    
    texFile = dbFile.replace('.db', '.tex')
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
        