#!/usr/bin/python
import sys
"""
Notes:
See redpathintel.blogspot.com
For bugs and errors please email me at slydgotcha at gmail dot com 
"""

CharSet = { 'a':['a','4','@','A'], 'b':['8','B','b'], 'c':['c','C'], 'd':['d','D'], 'e':['e','3','E'], 'f':['F','f','ph', 'pH', 'Ph', 'PH'], 'g':['9','g','G'], 'h':['h','H'], 'i':['i','1','!','I','l'], 'j':['j','J'], 'k':['k','K'],'l':['1','!','L','l'], 'm':['m','M'], 'n':['n','N'],'o':['o','0','O'],'p':['p','P'],'q':['q','Q'],'r':['r','R'],'s':['s','S','$'],'t':['t','T','7'],'u':['u','U'],'v':['v','V'],'w':['w','W'],'x':['x','X'],'y':['y','Y','?'],'z':['z','Z'],
'A':['a','A','@','4'],'B':['b','B','8'],'C':['c','C'],'D':['d','D'],'E':['e','E','3'],'F':['f','F'], 'G':['g','G','9'], 'H':['H','h'], 'I':['i','I','1','!','l'], 'J':['j','J'], 'K':['k','K'], 'L':['l','L','!','1'], 'M':['m','M'], 'N':['n','N'],'O':['o','O','0'],'P':['p','P'],'Q':['q','Q'],'R':['r','R'],'S':['s','S'],'T':['t','T','7'], 'U':['u','U'], 'V':['v','V'], 'W':['w','W'], 'X':['x','X'], 'Y':['y','Y','?'], 'Z':['z','Z'],
'1':['1','l','L','!','i','I'], '2':'2', '3':['3','e','E'],'4':['4','a','A','@'], '5':'5', '6':'6', '7':['7','t','T'], '8':['8','B','b'], '9':['9','g','G'], '0':['0','o','O'], '@':['a','A','4'], '!':['1','l'], '?':['y','Y'], '#':'#', '%':'%', '^':'^', '&':'&', '*':'*', '(':'(', ')':')', '//':'//', '\\':'\\', '<':'<', '>':'>', '+':'+', '-':'-', '|':'|', '~':'~', '`':'`', ' ':' ', ':':':', ';':';'}

def possibilities(password,file):
    i = 0
    instances = 1
    while True:
        try:
            if password[i] in CharSet:
                instances *= len(CharSet[password[i]])
                i += 1
            else:
                pass
        except:
            break;
    file.write('Possibilities are: %d\n' % instances)
    manipulate(password, file)

def manipulate(password, file):
    if len(password) == 1:
        manip1(password, file)
    elif len(password) == 2:
        manip2(password, file)
    elif len(password) == 3:
        manip3(password, file)
    elif len(password) == 4:
        manip4(password, file)
    elif len(password) == 5:
        manip5(password, file)
    elif len(password) == 6:
        manip6(password, file)
    elif len(password) == 7:
        manip7(password, file)
    elif len(password) == 8:
        manip8(password, file)
    elif len(password) == 9:
        manip9(password, file)
    elif len(password) == 10:
        manip9(password, file)
    elif len(password) == 11:
        manip11(password, file)
    elif len(password) == 12:
        manip12(password, file)
    elif len(password) == 13:
        manip13(password, file)
    elif len(password) == 14:
        manip14(password, file)
    elif len(password) == 15:
        manip15(password, file)
    elif len(password) == 16:
        manip16(password, file)
    elif len(password) == 17:
        manip17(password, file)
    elif len(password) == 18:
        manip18(password, file)
    elif len(password) == 19:
        manip19(password, file)
    elif len(password) == 20:
        manip20(password, file)
    else:
        print "The password you input is not 1-16 characters in length, quitting."

def manip1(password, file):
    for char1 in CharSet[password[0]]:
        file.write("%s\n" % char1)
        
def manip2(password, file):
    for char1 in CharSet[password[0]]:
        for char2 in CharSet[password[1]]:
            file.write("%s%s\n" % (char1,char2))
    
def manip3(password, file):
    for char1 in CharSet[password[0]]:
        for char2 in CharSet[password[1]]:
            for char3 in CharSet[password[2]]:
                file.write("%s%s%s\n" % (char1,char2,char3))

def manip4(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        file.write("%s%s%s%s\n" % (char1,char2,char3,char4))

def manip5(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            file.write("%s%s%s%s%s\n" % (char1,char2,char3,char4,char5))

def manip6(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                file.write("%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6))

def manip7(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    file.write("%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7))

def manip8(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        file.write("%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8))
                                        
def manip9(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            file.write("%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9))
                                            
def manip10(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                file.write("%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10))

def manip11(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    file.write("%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11))
                                                    
def manip12(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        file.write("%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12))
                                                        
def manip13(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13))
                                                            
def manip14(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14))
                                                                
def manip15(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                for char15 in CharSet[password[14]]:
                                                                    file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14,char15))
                                                                
def manip16(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                for char15 in CharSet[password[14]]:
                                                                    for char16 in CharSet[password[15]]:
                                                                        file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14,char15,char16))
                                                                        
def manip17(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                for char15 in CharSet[password[14]]:
                                                                    for char16 in CharSet[password[15]]:
                                                                        for char17 in CharSet[password[16]]:
                                                                            file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14,char15,char16,char17))

def manip18(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                for char15 in CharSet[password[14]]:
                                                                    for char16 in CharSet[password[15]]:
                                                                        for char17 in CharSet[password[16]]:
                                                                            for char18 in CharSet[password[17]]:
                                                                                file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14,char15,char16,char17,char18))

def manip19(password, file):
    for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                for char15 in CharSet[password[14]]:
                                                                    for char16 in CharSet[password[15]]:
                                                                        for char17 in CharSet[password[16]]:
                                                                            for char18 in CharSet[password[17]]:
                                                                                for char19 in CharSet[password[18]]:
                                                                                    file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14,char15,char16,char17,char18,char19))

def manip20(password, file):
        for char1 in CharSet[password[0]]:
            for char2 in CharSet[password[1]]:
                for char3 in CharSet[password[2]]:
                    for char4 in CharSet[password[3]]:
                        for char5 in CharSet[password[4]]:
                            for char6 in CharSet[password[5]]:
                                for char7 in CharSet[password[6]]:
                                    for char8 in CharSet[password[7]]:
                                        for char9 in CharSet[password[8]]:
                                            for char10 in CharSet[password[9]]:
                                                for char11 in CharSet[password[10]]:
                                                    for char12 in CharSet[password[11]]:
                                                        for char13 in CharSet[password[12]]:
                                                            for char14 in CharSet[password[13]]:
                                                                for char15 in CharSet[password[14]]:
                                                                    for char16 in CharSet[password[15]]:
                                                                        for char17 in CharSet[password[16]]:
                                                                            for char18 in CharSet[password[17]]:
                                                                                for char19 in CharSet[password[18]]:
                                                                                    for char20 in CharSet[password[19]]:
                                                                                        file.write("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n" % (char1,char2,char3,char4,char5,char6,char7,char8,char9,char10,char11,char12,char13,char14,char15,char16,char17,char18,char19,char20))
    
def main():
    password = raw_input('Please enter your password: ')
    possibilities(password)

def helpUse():
    print 'Password Manipulation Tool, currently supports passwords up to 20 characters long '
    print 'For more information see: redpathintel.blogspot.com'
    print 'By Devon Greene'
    print "\n\tUsage is:\n\t\t SWMT.py -s [Single Word Mode] <Single Word> <Output File>\n\t\t SWMT.py -f [Word File Mode]  <Word Input File> <Output File>"
    print "\n\n\tExamples:\n\t\t SWMT.py -s cat possibilities.txt\n\t\t SWMT.py -f wordlist.txt newlist.txt"

def singleUse(args):
    try:
        outFile = open(args[2], 'w')
    except:
        print "Error creating output file: %s " % args[2]
        raise
    else:
        possibilities(args[1], outFile)
        outFile.close
            
def fileUse(args):
    print "Made it to fileUse"
    try:
        inFile = open(args[1], 'r')
        try:
            outFile = open(args[2], 'w')
        except:
            print "Error creating output file: %s" % args[2]
            raise
    except:
        print "Error opening wordlist file: %s" % args[1]
        raise
    else:
        for line in inFile:
            manipulate(line.strip('\n'),outFile)
        outFile.close
        inFile.close
    
    
def checkArgs(args):
        if args[0] == '-s':
            singleUse(args)
        elif args[0] == '-f':
            fileUse(args)
        else:
            print "Invalid switch."
            
if __name__ == '__main__':
    args = sys.argv[1:]
    if len(sys.argv) != 4:
        helpUse()
    else:
        checkArgs(args)
