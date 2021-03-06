import time
from PyQt5.QtCore import QThread, pyqtSignal
from pymavlink.dialects.v10.mavmessages import *
from pymavlink import mavutil

from serial_data import *

ACCUM_LEN = 1
serial_device = '/dev/ttyUSB0'
serial_baud = 115200

class MsgAccumulator:
    def __init__(self, batch_size, signal):
        self.batch_size = batch_size
        self.signal = signal
        self.accumulator = []

    def push_message(self, msg):
        self.accumulator.append(msg)
        if len(self.accumulator) >= self.batch_size:
            self.signal.emit(self.accumulator)          # send all accumulator to slot @QtCore.pyqtSlot(list)
            self.accumulator = []
            # print('PUSH COMPLETED')


class TransferThread(QThread):
    new_serial_record = pyqtSignal(list)
    set_status_signal = pyqtSignal(str, int)
    blank_status_signal = pyqtSignal()

    def __init__(self):
        QThread.__init__(self)
        self.serial_accum = MsgAccumulator(ACCUM_LEN, self.new_serial_record)

    def process_message(self, msg):
        if isinstance(msg, SerialMessage):
            self.serial_accum.push_message(msg)

    def connect(self, error):
        try:
            dev = SerialConnector()
            return dev
        except BaseException as ex:
            if error: print('error', ex)
            return None

    def run(self):
        t = time.time()

        # First connection.
        dev = self.connect(True)
        if not dev:
            self.set_status_signal.emit("Check your connection", 0)
            while not dev:
                time.sleep(1)
                dev = self.connect(False)
            self.blank_status_signal.emit()

        while True:
            # Try to get data.
            try:
                pack = dev.parse_packet()
            # Close device on error and connect again.
            except BaseException:
                dev.close()
                dev = None
                self.set_status_signal.emit("Check your connection", 0)
                while not dev:
                    time.sleep(1)
                    dev = self.connect(True)
                self.blank_status_signal.emit()
                continue
            # if pack:
            #     print(pack)
            t_prev = t
            t = time.time()
            print(t - t_prev)
            self.process_message(pack)

# ;MAVLINK20=TRUE;MAVLINK_DIALECT=mavmessages