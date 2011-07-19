import serial, struct

serial_port = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
read = serial_port.read()

while True:
	serial_port.write(struct.pack('B', int(raw_input())))
	print '>>',struct.unpack('B', serial_port.read(1))[0]
	serial_port.read(1)


