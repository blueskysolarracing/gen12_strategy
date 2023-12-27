# add shared dir to path
import sys, os

# set path to parent directory, which contains the lib folder
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, BASE_DIR)

import connection, gui, radiosender


if __name__ == "__main__":
    # Create connection
    conn = connection.Connection()

    # Create sender
    sender = radiosender.BssrProtocolSender(conn)

    # Create GUI
    gui = gui.App(sender)

    # Start GUI
    gui.start()
    
