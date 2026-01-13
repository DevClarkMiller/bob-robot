import serial, time, argparse, threading

port = None
baud = None
quit_event = threading.Event()
arduino = None

def get_serial(com, baud):
    return serial.Serial(port=com,  baudrate=baud, timeout=.1)

def read():
    data = arduino.readline()
    try:
        return data.decode('utf-8').strip()
    except:
        return data

def write(msg): 
    # while True:
    #     state = read()

    #     if state == '': continue
    #     elif state == '$READY:1': break
    #     return False

    arduino.write(msg.encode())
    print(f"[Client] {msg}")
    return True

def writeSerial():
    global quit_event
    while not quit_event.is_set():
        try:
            msg = input("Enter message or enter 'exit' to quit: ")
            if msg == "exit":
                quit_event.set()
                break
            write(msg)
        except Exception as e:
            print(e)
            quit_event.set()

def readSerial():
    global quit_event
    while not quit_event.is_set():
        try:
            msg = read()
            if msg: 
                print(f"[{port}] {msg}")
        except Exception as e:
            print(e)
            quit_event.set()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Testing client for bob the robots receiver")

    parser.add_argument("--com", help="The com to use")
    parser.add_argument("--baud_rate", help="The baud rate to use with the program")

    args = parser.parse_args()
    
    try:
        port = args.com or 'COM9'
        baud = args.baud_rate or 9600

        arduino = get_serial(port, baud)
        time.sleep(1.5)

        writeThread = threading.Thread(target=writeSerial)
        readThread = threading.Thread(target=readSerial)

        writeThread.start()
        readThread.start()

        writeThread.join()
        readThread.join()

        arduino.close()
    except Exception as e:
        print(e)