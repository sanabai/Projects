import MySQLdb

mydb = MySQLdb.connect(
	host ="localhost",
	user = "root",
	password = "root"
)

mycursor = mydb.cursor()

mycursor.execute("CREATE DATABASE hostel_db")
