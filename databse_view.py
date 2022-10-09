import MySQLdb

mydb = MySQLdb.connect(
	host ="localhost",
	user = "root",
	passwd = "root",
	database = "hostel_db"
)

mycursor = mydb.cursor()
#mycursor.execute("CREATE TABLE student_data ( name VARCHAR(255),  roll_no VARCHAR(255), room_no VARCHAR(255))")
#mycursor.execute("CREATE TABLE hosteler_io_rec( name VARCHAR(255),  roll_no VARCHAR(255), room_no VARCHAR(255) , out_time VARCHAR(255), in_time VARCHAR(255))")

#sqlFormula = "INSERT INTO student_data (name , roll_no, room_no) VALUES (%s, %s, %s)"
#students = [("Alankritha", "EE19BTECH11037", "J225"),
#			("Charitha","EE19BTECH11001", "J221"),
#			("Sana","EE19BTECH11021","J223"),
#			("Eshwari","EE19BTECH11042","J220"),]
		
#mycursor.executemany(sqlFormula, students)
mycursor.execute("SELECT * FROM hostel_db.hosteler_io_rec")
mydb.commit()
myresult = mycursor.fetchall()
for x in myresult:
	print(x)
