import serial, time, argparse

PORT = 'COM9'
BAUD = 9600
arduino = None

def get_serial(com, baud):
    return serial.Serial(port=com,  baudrate=baud, timeout=.1)

def read():
    time.sleep(0.05)
    data = arduino.readline().decode('utf-8')
    return data

def write(msg): 
    while True:
        state = read()

        if state == '': continue
        elif state == '1': break
        return False

    arduino.write(msg.encode())
    print(f"sent {msg}")
    return True    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Testing client for bob the robots receiver")

    parser.add_argument("--com", help="The com to use")
    parser.add_argument("--baud_rate", help="The baud rate to use with the program")
    parser.add_argument("--listener", action='store_true', help="Only listen for messages")
    parser.add_argument("--messages", nargs="+", help="One or more messages to send")

    args = parser.parse_args()
    
    try:
        arduino = get_serial(args.com or PORT, args.baud_rate or BAUD)
        time.sleep(5)
        
        if args.listener:
            while True:
                message = read()
                if message: 
                    print(message)

        for msg in args.messages:
            write(msg)

        arduino.close()
    except Exception as e:
        print(e)