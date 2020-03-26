#!/usr/bin/env python3.7
import requests
import base64
import argparse
import zipfile

parser = argparse.ArgumentParser(prog='deploy cpm-hub', description='CPM Hub deployer', add_help=False)
parser.add_argument('-k', '--api-key', required=True, dest="api_key")
parser.add_argument('program_binary')
parser.add_argument('server_url')
args = parser.parse_args()

zip_name = f'{args.program_binary}.zip'
zip_file = zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED)
zip_file.write(args.program_binary)
zip_file.close()

with open(args.program_binary, 'rb') as stream:
    binary = stream.read()
    payload = base64.b64encode(binary).decode('utf-8')

headers = {'API_KEY': args.api_key}
try:
    requests.post(args.server_url, data=payload, headers=headers, verify=False)
except:
    print('all OK?')
