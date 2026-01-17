import serial, time, argparse, threading, os

LAST_ACT_MS_THRESHOLD = 1500 # 

port = None
baud = None
quit_event = threading.Event()
arduino = None

def clear(): os.system('cls' if os.name == 'nt' else 'clear')
def stop(): quit_event.set()

def get_serial(com, baud):
    return serial.Serial(port=com, baudrate=baud, timeout=.1)

def read(encoding = 'utf-8'):
    data = arduino.readline()
    try:
        decoded = data.decode(encoding).strip()
        return decoded if decoded else None
    except:
        return None

def write(msg): 
    arduino.write(msg.encode())
    print(f"[Client] {msg}")
    return True

def writeSerial():
    global quit_event
    while not quit_event.is_set():
        try:
            msg = input("Enter message or enter 'exit' to quit: ")
            if msg == "exit":
                stop()
                break
            elif msg == "clear": 
                clear()
                continue
            write(msg)
        except Exception as e:
            print(e)
            stop()

def readSerial():
    global quit_event
    while not quit_event.is_set():
        try:
            msg = read()
            if msg: print(f"[{port}] {msg}")
            time.sleep(0.25)
        except Exception as e:
            print(e)
            stop()

def millis():
    return time.time_ns() // 1_000_000

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