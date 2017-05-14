from Adafruit_IO import Client
aio = Client('io_key')
temp = aio.receive('Temperatur')
print("{0:.1f}".format(float(temp.value)))
