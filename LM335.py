import os 
import sys
cwd = os.getcwd()
(setpath,Examples) = os.path.split(cwd)
sys.path.append(setpath)

from Arduino import Arduino
from time import sleep

class LM335:
    def __init__(self, baudrate):
        self.baudrate = baudrate
        self.setup()
        self.run()
        self.exit()

    def setup(self):
        self.obj_arduino = Arduino()
        self.port = self.obj_arduino.locateport()
        self.obj_arduino.open_serial(1, self.port, self.baudrate)

    def run(self):
        #initializing pins
        self.lm335 = 5
        self.rs=12
        self.en=11
        self.d4=7
        self.d5=6
        self.d6=5
        self.d7=4
        self.rx=2
        self.tx=3
        #infinite loop
        while True:
            val = self.obj_arduino.cmd_analog_in(1, self.lm335)
            val=val*0.489-273
            val=int(val)
            #String to be send
            st="Temperature:"+str(val)+"C "
            #sending string to LCD
            self.obj_arduino.lcd_out(1,st,self.rs, self.en, self.d4, self.d5, self.d6, self.d7 )
            sleep(2)
            #Sending string to Webpage
            self.obj_arduino.wifi_out(1,st,self.rx,self.tx )
            print(val)
            sleep(2)

    def exit(self):
        self.obj_arduino.close_serial()

def main():
    obj_lm335 = LM335(115200)#Creating a object

if __name__=='__main__':
    main()
