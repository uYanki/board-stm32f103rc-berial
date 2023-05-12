# -*- coding: utf_8 -*-

from serial import Serial
import modbus_tk.defines as modbus_cst
from modbus_tk import modbus_rtu

try:
    ''' init interface '''
    serial = Serial(port="com11", baudrate=115200, bytesize=8, parity='N', stopbits=1, xonxoff=0)
    master = modbus_rtu.RtuMaster(serial)
    master.set_timeout(1.0)
    master.set_verbose(True)
    ''' event handler '''
    # toggle value
    slave = 0x0A
    address = 1004
    index = 0
    while index < 10:
        try:
            word = master.execute(slave, modbus_cst.READ_INPUT_REGISTERS, address, 1)
            print(word)
        except:
            pass
        index += 1
    # master.execute(slave, modbus_cst.WRITE_SINGLE_REGISTER, address, output_value=0x0F if word == 0x00 else ~word[0])
    # read words
    exit(1)
    slave = 199
    step = 10
    for address in range(0, step*10, step):
        words = master.execute(slave, modbus_cst.READ_HOLDING_REGISTERS, address, step)
        for index, word in enumerate(words):
            if word != 0:
                print(f"address = {address+index}, value = {word}")

except Exception as exc:
    print(str(exc))
    exit()
