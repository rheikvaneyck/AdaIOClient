from Adafruit_IO import Client
aio = Client('io_key')
hum = aio.receive('Luftfeuchte')
print("{0}".format(hum.value))
