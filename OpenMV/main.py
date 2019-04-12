import pyb,sensor, image, time, math
from pyb import UART
import ujson

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.VGA)
sensor.set_windowing((640, 80))
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
uart = UART(3, 115200)
led = pyb.LED(3)
def barcode_name(code):
    if(code.type() == image.EAN2):
        return "EAN2"
    if(code.type() == image.EAN5):
        return "EAN5"
    if(code.type() == image.EAN8):
        return "EAN8"
    if(code.type() == image.UPCE):
        return "UPCE"
    if(code.type() == image.ISBN10):
        return "ISBN10"
    if(code.type() == image.UPCA):
        return "UPCA"
    if(code.type() == image.EAN13):
        return "EAN13"
    if(code.type() == image.ISBN13):
        return "ISBN13"
    if(code.type() == image.I25):
        return "I25"
    if(code.type() == image.DATABAR):
        return "DATABAR"
    if(code.type() == image.DATABAR_EXP):
        return "DATABAR_EXP"
    if(code.type() == image.CODABAR):
        return "CODABAR"
    if(code.type() == image.CODE39):
        return "CODE39"
    if(code.type() == image.PDF417):
        return "PDF417"
    if(code.type() == image.CODE93):
        return "CODE93"
    if(code.type() == image.CODE128):
        return "CODE128"
while(True):
    clock.tick()
    img = sensor.snapshot()
    codes = img.find_barcodes()
    x=2
    for code in codes:
        img.draw_rectangle(code.rect())
        b = code.payload()
        #a = ujson.dumps(b)
        #print(b)
        uart.write(b+'\r\n')
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.off()
        time.sleep(30)
        led.on()
        time.sleep(30)
        led.off()
        time.sleep(15000)
        #pyb.delay(15000)