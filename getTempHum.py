from Adafruit_IO import Client
aio = Client('3589cfa6e2ba482b8445148ced3d4cbd')
temp = aio.receive('Temperatur')
hum = aio.receive('Humidity')
print("{0:.1f}C/{1}%".format(float(temp.value),hum.value))