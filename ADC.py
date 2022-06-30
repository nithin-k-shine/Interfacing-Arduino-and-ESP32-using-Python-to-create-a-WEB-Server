import os 
import sys
cwd = os.getcwd()
(setpath,Examples) = os.path.split(cwd)
sys.path.append(setpath)

from Arduino.Arduino import Arduino
from time import sleep

class ADC:
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
        self.adc = 4
        self.led = 13
        while True :
            val = self.obj_arduino.cmd_analog_in(1, self.adc)
            print(val)
            if val>=512:
                self.obj_arduino.cmd_digital_out(1, self.led, 1)
            else:
                self.obj_arduino.cmd_digital_out(1, self.led, 0)
            sleep(0.5)  

    def exit(self):
        self.obj_arduino.close_serial()

def main():
    obj_adc = ADC(115200)

if __name__=='__main__':
    main()
