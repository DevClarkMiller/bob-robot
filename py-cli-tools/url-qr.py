import argparse, qrcode, requests, os
from dotenv import load_dotenv
from urllib.parse import urlencode

def print_qr(data):
    try:
        # Create a QR code instance
        qr = qrcode.QRCode(
            version=1,
            error_correction=qrcode.constants.ERROR_CORRECT_L,
            box_size=10, # Larger box size for better visibility in image
            border=4,
        )
        qr.add_data(data)
        qr.make(fit=True)

        # Generate and display the QR code image in a window
        img = qr.make_image(fill_color="black", back_color="white")
        img.show()

    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

def getHeaders(token):
    return {
        "Authorization": f"Bearer {token}",
	}

class Client:
    def get_base_url(self): return os.getenv('SITE_URL')
    def get_base_api_url(self): return os.getenv('API_URL')
    def get_admin_token_key(self): return os.getenv('ADMIN_TOKEN_KEY')
    def get_url(self): return self.get_base_url() 

    # Use the admin token key to generate the given type
    def get_token(self):
        query_string = urlencode({ 
            'adminTokenKey': self.get_admin_token_key() 
        })

        url = f"{self.get_base_api_url()}/Token/createAdminToken?{query_string}"
        resp = requests.post(url)
        return resp.text

    def __init__(self, unitGuid):
        self.unitGuid = unitGuid

class ChatterClient(Client):
    def get_token(self): 
        # Get admin token first
        adminToken = super().get_token()

        url = f"{self.get_base_api_url()}/Token/createChatterToken"
        resp = requests.post(url, headers=getHeaders(adminToken))
        return resp.text
    
    def get_url(self): 
        token = self.get_token()

        params = { 'chatterToken': token, 'unitGuid': self.unitGuid }
        query_string = urlencode(params)

        url = f"{self.get_base_url()}?{query_string}"

        return url

class KeyboardClient(Client):
    def get_base_url(self): return f"{super().get_base_url()}/keyboard"
    
    def get_token(self):
        # Get admin token first
        adminToken = super().get_token()

        params = { 'unitGuid': self.unitGuid }
        query_string = urlencode(params)

        url = f"{self.get_base_api_url()}/Token/createToken?{query_string}"
        resp = requests.post(url, headers=getHeaders(adminToken))
        return resp.text

    def get_url(self): 
        token = self.get_token()

        params = { 'unitToken': token, 'unitGuid': self.unitGuid }
        query_string = urlencode(params)

        url = f"{self.get_base_url()}?{query_string}"

        return url

def clientFactory(tokenType, unitGuid):
    match tokenType:
        case 'chatter': return ChatterClient(unitGuid)
        case 'unit': return KeyboardClient(unitGuid)
    raise Exception('Unknown TokenType')

if __name__ == "__main__":
    load_dotenv()

    parser = argparse.ArgumentParser(description="QR Code generator for different bob the robot projects")
    
    parser.add_argument("--tokenType", help="The type of token to generate", choices=['chatter', 'unit'], required=True)
    parser.add_argument("--unitGuid", help="The guid of the unit", required=True)

    args = parser.parse_args()

    client = clientFactory(args.tokenType, unitGuid=args.unitGuid)
    url = client.get_url()
    print(url)
    print_qr(url)
