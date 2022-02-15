import serial # Import pySerial Library
import datetime # Import DateTime library

ser = serial.Serial("COM6", 9600) #
ser.flushInput()


while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
        now = datetime.datetime.now()
        now = now.strftime("%Y-%m-%d %H:%M:%S")
        data = str( "'{}',{}\r\n".format(now,decoded_bytes) )
        if "E3 4C 2B 13" in data:
            data = str( "'{}',{}\r\n".format(now," Faaiz") )
        print(data)
        with open('C:\\FilePath\\Record-1.txt', 'a') as f: #Replace File Path with your own.
            f.write(data)
            f.write('\n')
    except:
        print("*")
