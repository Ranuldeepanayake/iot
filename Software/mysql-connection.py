import mysql.connector

mydb = mysql.connector.connect(
  host="54.169.116.74",
  user="sep",
  password="RHFkwLSa62uXb7vQ",
  database="iot"
)

mycursor = mydb.cursor()

sql = "INSERT INTO topic_temperature (value) VALUES (%s)"
val = [45]
mycursor.execute(sql, val)

mydb.commit()
mycursor.close()
mydb.close()
print(mycursor.rowcount, "record inserted.")