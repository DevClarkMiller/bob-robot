import argparse, qrcode, requests
from dotenv import load_dotenv

def generate_qr_to_cli(data):
    """
    Generates a QR code for the given data and prints it to the console.
    """
    try:
        # Create a QR code instance
        qr = qrcode.QRCode(
            version=1,
            error_correction=qrcode.constants.ERROR_CORRECT_L,
            box_size=1, # Smaller box size for better CLI fit
            border=2,
        )
        qr.add_data(data)
        qr.make(fit=True)

        # Print the QR code to the console
        # The qrcode library handles the console rendering when you iterate over it
        qr.print_tty() 

    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

def getHeaders(token):
    return {
        "Authorization": f"Bearer {token}",
	}

if __name__ == "__main__":
	load_dotenv()
	parser = argparse.ArgumentParser(description="QR Code generator for different bob the robot projects")
    
	parser.add_argument("--tokenType", help="The type of token to generate")

	args = parser.parse_args()

	# generate_qr_to_cli('https://clarkmiller.ca')