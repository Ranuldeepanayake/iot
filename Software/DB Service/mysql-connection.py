import mysql.connector

mydb = mysql.connector.connect(
  host="54.169.116.74",
  user="sep",
  password="RHFkwLSa62uXb7vQ",
  database="iot"
)

def dbTopicAmbientLight(value):
    print("Connecting to the database")

    mycursor = mydb.cursor()
    sql = "update device set topic_ambient_light= %s where device_id= 1"
    val = [value]

    mycursor= mydb.cursor()
    mycursor.execute(sql, val)
    mydb.commit()
    mycursor.close()
    mydb.close()

    print(mycursor.rowcount, "Record updated")

def function():
    mycursor = mydb.cursor()

    sql = "INSERT INTO topic_temperature (value) VALUES (%s)"
    val = [32.56]
    mycursor.execute(sql, val)

    mydb.commit()
    mycursor.close()
    mydb.close()
    print(mycursor.rowcount, "record inserted.")

dbTopicAmbientLight(31.78)
#function()