#!/usr/bin/env python
from __future__ import division
import serial
import time
import MySQLdb
import smtplib

#enter your device file
device = '/dev/ttyUSB1'
baudrate = 9600
bytesize=serial.EIGHTBITS
parity=serial.PARITY_NONE
stopbits=serial.STOPBITS_ONE
timeout=None
xonxoff=serial.XOFF
rtscts=False
writeTimeout=None
dsrdtr=False
interCharTimeout=None

i=0
j=0
dummy=0

#setup - if a Serial object can't be created, a SerialException will be raised.
while True:
    try:
        ser = serial.Serial(device, baudrate, bytesize, parity, stopbits, timeout, xonxoff, rtscts, writeTimeout, dsrdtr, interCharTimeout)
		
        #break out of while loop when connection is made
        break
    except serial.SerialException:
        print 'Waiting for device ' + device + ' to be available.'
        time.sleep(3)

#read lines from serial device
while True:
		
   	ch = ser.readline().strip('\n')
	
	char0=ord(ch[0])	
	char1=ord(ch[1])
	char2=ord(ch[2])
	char3=ord(ch[3])-30
	char4=ord(ch[4])-30
	char5=ord(ch[5])-30
	char6=ord(ch[6])-30
	
	'''
	print char0
	print char1
	print char2
	print char3
	print char4
	print char5
	print char6
	print "---------------"
	'''
	
	if char0==170:	
		if char1==191: #FOR SWITCH
				if char2==1:
					SOnOff="True"
				if char2==0:
					SOnOff="False"
					
				SDimmer=char3

				char5=char5/10;
				STemp=char4+char5
				
				SLumens=char6
				
				db = MySQLdb.connect("localhost","root","passwd","Home") # Open database connection
				cursor = db.cursor() # prepare a cursor object using cursor() method 
					
				# Prepare SQL query to INSERT a record into the database.
				query1 = "UPDATE Switch SET OnOff=%s WHERE ID='BF'"
				query2 = "UPDATE Switch SET Dimmer=%s WHERE ID='BF'"
				query3 = "UPDATE Switch SET Temp=%s WHERE ID='BF'"
				query4 = "UPDATE Switch SET Lumens=%s WHERE ID='BF'"
				try:
					
					cursor.execute(query1, SOnOff) # Execute the SQL command
					db.commit() # Commit your changes in the database
					cursor.execute(query2, SDimmer)
					db.commit()
					cursor.execute(query3, STemp)
					db.commit()
					cursor.execute(query4, SLumens)
					db.commit()
				except:
					# Rollback in case there is any error
					db.rollback()

				# disconnect from server
				db.close()
		
		if char1==174: #FOR THERMOSTAT
				if char2==1:
					TOnOff="True"
				if char2==0:
					TOnOff="False"
					
				char6=char6/10
				Ttemp=char5+char6 	
				char4=char4/10
				TtempUser=char3+char4
				
				db = MySQLdb.connect("localhost","root","passwd","Home") # Open database connection
				cursor = db.cursor() # prepare a cursor object using cursor() method 
					
				# Prepare SQL query to INSERT a record into the database.
				query1 = "UPDATE Thermostat SET Temp=%s WHERE ID='AE'"
				query2 = "UPDATE Thermostat SET OnOff=%s WHERE ID='AE'"
				query3 = "UPDATE Thermostat SET TempUser=%s WHERE ID='AE'"
				try:
					
					cursor.execute(query1, Ttemp) # Execute the SQL command
					db.commit() # Commit your changes in the database
					cursor.execute(query2, TOnOff)
					db.commit()
					cursor.execute(query3, TtempUser)
					db.commit()
				except:
					# Rollback in case there is any error
					db.rollback()

				# disconnect from server
				db.close()
				
		if char1==126: #FOR RADAR/ALARM
			if char5==48 or char5==49: #ALARM SET FEEDBACK
				if char5==49:
					query = "UPDATE Radar SET AlarmSet='True' WHERE ID='7E'"
				if char5==48:
					query = "UPDATE Radar SET AlarmSet='False' WHERE ID='7E'"	
				
				db = MySQLdb.connect("localhost","root","passwd","Home") 
				cursor = db.cursor() 
					
				try:					
					cursor.execute(query) 
					db.commit() 
				except:
					db.rollback()

				db.close()	
				
			if char6==181: #REPORT FAILURE
				i += 1
				db = MySQLdb.connect("localhost","root","passwd","Home") 
				cursor = db.cursor() 
				
				query = "UPDATE Radar SET AlarmReport='True' WHERE ID='7E'"
					
				try:					
					cursor.execute(query) 
					db.commit() 
				except:
					db.rollback()

				db.close()
				
				if i==1:
					# Import the email modules we'll need
					from email.mime.text import MIMEText
					# Create a text/plain message
					msg = MIMEText("Home is not secure. Security Breach by Radar Failure.")
					msg['Subject'] = 'SECURITY BREACH'
					msg['From'], msg['To'] = "emailfrom", "emailto"
					msg['Pass']="passwdfrom"
					# Send the message via gmail SMTP server
					s = smtplib.SMTP('smtp.gmail.com', 587)
					s.ehlo()
					s.starttls()
					s.login(msg['From'], msg['Pass']) 
					try:
					   # Python 3.2.1
					   s.send_message(msg)
					except AttributeError:
					   # Python 2.7.2
					   s.sendmail(msg['From'], [msg['To']], msg.as_string())
					   
					s.quit()
				
			if char6==154: #SECURITY BREACH BY MOVEMENT	
				j += 1
				db = MySQLdb.connect("localhost","root","passwd","Home") 
				cursor = db.cursor() 
				
				query = "UPDATE Radar SET AlarmMove='True' WHERE ID='7E'"
					
				try:					
					cursor.execute(query) 
					db.commit() 
				except:
					db.rollback()

				db.close()	
				
				if j==1:
					# Import the email modules we'll need
					from email.mime.text import MIMEText
					# Create a text/plain message
					msg = MIMEText("Home is not secure. Security Breach by Radar Failure.")
					msg['Subject'] = 'SECURITY BREACH'
					msg['From'], msg['To'] = "emailfrom", "emailto"
					msg['Pass']="passwdfrom"
					# Send the message via gmail SMTP server
					s = smtplib.SMTP('smtp.gmail.com', 587)
					s.ehlo()
					s.starttls()
					s.login(msg['From'], msg['Pass']) 
					try:
					   # Python 3.2.1
					   s.send_message(msg)
					except AttributeError:
					   # Python 2.7.2
					   s.sendmail(msg['From'], [msg['To']], msg.as_string())
					   
					s.quit()
			
			if char4==48 or char4==49: #ALERT BY REPORT FAILURE CLEAR FEEDBACK
				if char4==49:
					i=0
					query = "UPDATE Radar SET AlarmReport='False' WHERE ID='7E'"
				if char4==48:
					query = "UPDATE Radar SET AlarmReport='True' WHERE ID='7E'"	
				
				db = MySQLdb.connect("localhost","root","passwd","Home") 
				cursor = db.cursor() 
					
				try:					
					cursor.execute(query) 
					db.commit() 
				except:
					db.rollback()

				db.close()
					
			if char3==48 or char3==49: #ALERT BY MOVEMENT CLEAR FEEDBACK
				if char3==49:
					j=0
					query = "UPDATE Radar SET AlarmMove='False' WHERE ID='7E'"
				if char3==48:
					query = "UPDATE Radar SET AlarmMove='True' WHERE ID='7E'"	
				
				db = MySQLdb.connect("localhost","root","passwd","Home") 
				cursor = db.cursor() 
					
				try:					
					cursor.execute(query) 
					db.commit() 
				except:
					db.rollback()

				db.close()		
				
	if char0==238: #SEND SETTINGS TO uC
		db = MySQLdb.connect("localhost","root","passwd","Home") 
		cursor = db.cursor() 
			
		query1 = "SELECT DimmerSet, OnOffSet, SetMov, SetAutoLumensOnOff,SetAutoLumens FROM Switch WHERE ID='BF'"
		query2 = "SELECT OnOffSet, TempUserSet FROM Thermostat WHERE ID='AE'"
		query3 = "SELECT AlarmSet FROM Radar WHERE ID='7E'"
		try:
			cursor.execute(query1)
			SwitchSettings = cursor.fetchone()
			
			cursor.execute(query2)
			ThermostatSettings = cursor.fetchone()
						
			cursor.execute(query3)
			AlarmSettings = cursor.fetchone()
					
		except:
		   print "Error: unable to fecth data"

		db.close()
		
		######################################### THERMOSTAT
		if ThermostatSettings[0]=="True":
			TOnOffSetting="1"
		if ThermostatSettings[0]=="False":	
			TOnOffSetting="0"
			
		temporary=int(ThermostatSettings[1])
		TUserTemperatureSetting1=temporary
		TUserTemperatureSetting1=str(TUserTemperatureSetting1)
		TUserTemperatureSetting2=int((ThermostatSettings[1]-temporary)*10)
		TUserTemperatureSetting2=str(TUserTemperatureSetting2)
		
		Thermostatcommand="T"+TOnOffSetting+TUserTemperatureSetting1+TUserTemperatureSetting2+"NN"
		ser.write(Thermostatcommand)
		time.sleep(0.1)
		
		######################################### ALARM	
		if AlarmSettings[0]=="True":
			AlarmSet="1"
		if AlarmSettings[0]=="False":	
			AlarmSet="0"
			
		Alarmcommand="R"+AlarmSet+"NNNNN"
		ser.write(Alarmcommand)
		time.sleep(0.1)
		
		######################################### SWITCH
		SDimmerSetting=str(SwitchSettings[0])
		
		if SwitchSettings[1]=="True":
			SOnOffSetting="1"
		if SwitchSettings[1]=="False":	
			SOnOffSetting="2"
			
		if SwitchSettings[2]=="True":
			SMoveSetting="1"
		if SwitchSettings[2]=="False":	
			SMoveSetting="0"	
			
		if SwitchSettings[3]=="True":
			SOnOffLumensSetting="1"
		if SwitchSettings[3]=="False":	
			SOnOffLumensSetting="0"	
			
		SAutoLumensSetting=str(SwitchSettings[4])
		
		Switchcommand="S"+SOnOffSetting+SDimmerSetting+SMoveSetting+SOnOffLumensSetting+SAutoLumensSetting+"N"
		ser.write(Switchcommand)
		
		
		
		