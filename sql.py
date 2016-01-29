__author__ = 'KAUSTUBH1'

import MySQLdb
import sqlparams
import os
import sys
import subprocess
from subprocess import call

# db = MySQLdb.connect(host = 'localhost', port = 3306,db = "test_db", user = 'root', passwd = 'password')
# cursor = db.cursor()
# cursor.execute('DROP TABLE users')
# cursor.execute('CREATE TABLE IF NOT EXISTS users ( id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, firstname VARCHAR(30) NOT NULL, lastname VARCHAR(30) NOT NULL,username VARCHAR(50), password VARCHAR(200))')
# print "Done"

def cppcheck(filename):
    dir = os.path.dirname(__file__)
    # print dir
    # dir = "C:\Users\KAUSTUBH1\PycharmProjects\secure\"
    file = dir + filename
    # print file
    file = os.path.join(dir,filename)
    # print file
    # file.replace("/","\")
    # temp = open("output.txt",'w')
    output = os.path.join(dir,"output.txt")
    # print output
    sys.stdout = open("output.txt","w",0)
    # print temp
    var =  "cppcheck "+ file + " 2>" + output
    # print var
    # os.system(var)
    response = subprocess.call(var, stdout=sys.stdout, stderr=sys.stdout)
    # print response
cppcheck("file.c")



# call(['C:\\Program Files\\Cppcheck\\cppcheck', '--enable=all', sourcefile])
# print data

#
# val = raw_input("Enter your username: ")
# #
# # print type(val)
# row = cursor.execute("SELECT * from users where username = %s", [val])
#
# # for row in cursor.fetchall():
# print row[4]
#     # if  row != 1:
#     #     print "Invalid Credentails"
#     # else:
#     #     print "User Logged in"