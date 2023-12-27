from PyQt5 import QtWidgets
import time
import math
import sqlite3 
import numpy as np
from datetime import datetime
from sender.radiosender import BssrProtocolSender


class App:

    def __init__(self, sender: BssrProtocolSender):
        self.sender = sender
        self.sender
        # Qt app setup
        self.qt_application = QtWidgets.QApplication([])
        
        #self.cur = con.cursor()

        # Layout of Parent Widget
        self.parent_window = QtWidgets.QWidget()
        self.parent_window.setGeometry(100, 100, 400, 300)
        self.parent_window.setWindowTitle("Communications to Solar")
        self.parent_layout = QtWidgets.QFormLayout()
        self.parent_layout.setSpacing(10)
        self.parent_layout.setContentsMargins(0, 0, 0, 0)
        self.parent_window.setLayout(self.parent_layout)

        # Configure the communications plotting tab - tharaka

        self.custom_msg_bar = QtWidgets.QLineEdit()
        self.custom_msg_bar.setMaxLength(11)
        self.custom_msg_enter_button = QtWidgets.QPushButton('Enter')

        self.request = QtWidgets.QPushButton('Hear us?')
        self.pull_over = QtWidgets.QPushButton('Pull Over')
        self.egress = QtWidgets.QPushButton('EGRESS')
        self.eco_off = QtWidgets.QPushButton('ECO Off')
        self.eco_on = QtWidgets.QPushButton('ECO On')
        self.vfm_up = QtWidgets.QPushButton('VFM Up')
        self.vfm_down = QtWidgets.QPushButton('VFM Down')
        
        self.PI_KP_bar = QtWidgets.QLineEdit()
        self.PI_KP_bar.setMaxLength(10)
        self.PI_KP_enter_button = QtWidgets.QPushButton('Enter KP')

        self.PI_KI_bar = QtWidgets.QLineEdit()
        self.PI_KI_bar.setMaxLength(10)
        self.PI_KI_enter_button = QtWidgets.QPushButton('Enter KI')

        self.PI_KD_bar = QtWidgets.QLineEdit()
        self.PI_KD_bar.setMaxLength(10)
        self.PI_KD_enter_button = QtWidgets.QPushButton('Enter KD')

        self.f_enable = QtWidgets.QPushButton('f_enable')
        self.f_disable = QtWidgets.QPushButton('f_disable')

        self.parent_layout.addWidget(self.request)
        self.parent_layout.addWidget(self.pull_over)
        self.parent_layout.addWidget(self.egress)
        self.parent_layout.addWidget(self.custom_msg_enter_button)
        self.parent_layout.addWidget(self.custom_msg_bar)
        self.parent_layout.addWidget(self.custom_msg_enter_button)
        self.parent_layout.addWidget(self.eco_off)
        self.parent_layout.addWidget(self.eco_on)
        self.parent_layout.addWidget(self.vfm_up)
        self.parent_layout.addWidget(self.vfm_down)
        self.parent_layout.addWidget(self.PI_KP_bar)
        self.parent_layout.addWidget(self.PI_KP_enter_button)
        self.parent_layout.addWidget(self.PI_KI_bar)
        self.parent_layout.addWidget(self.PI_KI_enter_button)
        self.parent_layout.addWidget(self.PI_KD_bar)
        self.parent_layout.addWidget(self.PI_KD_enter_button)
        self.parent_layout.addWidget(self.f_enable)
        self.parent_layout.addWidget(self.f_disable)

        # Connect each button to its corresponding function
        
        # communication buttons
        self.custom_msg_enter_button.clicked.connect(self.send_text)
        self.request.clicked.connect(lambda: self.sender.phrase_sender("Hear us?"))
        self.pull_over.clicked.connect(lambda: self.sender.phrase_sender("Pull Over"))
        self.egress.clicked.connect(lambda: self.sender.phrase_sender("EGRESS"))
        
        # motor control buttons
        self.eco_off.clicked.connect(self.sender.eco_off_sender)
        self.eco_on.clicked.connect(self.sender.eco_on_sender)
        self.vfm_up.clicked.connect(self.sender.vfm_up_sender)
        self.vfm_down.clicked.connect(self.sender.vfm_down_sender)

        # PI gain buttons
        self.PI_KP_enter_button.clicked.connect(
            lambda: self.sender.cruise_PI_KP_sender(float(self.PI_KP_bar.text())))
        self.PI_KI_enter_button.clicked.connect(
            lambda: self.sender.cruise_PI_KI_sender(float(self.PI_KI_bar.text())))
        self.PI_KD_enter_button.clicked.connect(
            lambda: self.sender.cruise_PI_KD_sender(float(self.PI_KD_bar.text())))

        # f enable buttons
        self.f_enable.clicked.connect(self.sender.f_enable_sender)
        self.f_disable.clicked.connect(self.sender.f_disable_sender)

    #Functions for the buttons in Communication Request - tharaka
    def send_text(self):
        print(len(self.custom_msg_bar.text()))
        text = self.custom_msg_bar.text()
        self.sender.phrase_sender(text)


    def start(self):
        """
        Visualize the app and begin the event loop
        """
        self.parent_window.show()
        self.qt_application.exec()