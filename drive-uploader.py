import sys
print (sys.path)
import os
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive

print("Start Authentication")
gauth = GoogleAuth()
print("Google Auth")
#GoogleAuth().DEFAULT_SETTINGS['client_config_file']=['/home/pi/client_secrets.json']
gauth.LocalWebserverAuth() # Creates local webserver and auto handles authentication.

print("Authentication done")


drive = GoogleDrive(gauth)

"""file1 = drive.CreateFile({'title': 'Hellonew.txt'})  # Create GoogleDriveFile instance with title 'Hello.txt'.
file1.SetContentString('Hello World!') # Set content of the file from given string.
file1.Upload()"""
print(os.path.abspath('client_secrets.json'))
      



file1 = drive.CreateFile()  # Create GoogleDriveFile instance with title 'Hello.txt'.
#file1.SetContentFile('ship_project.csv') # Set content of the file from given string.
file1.SetContentFile('document.txt')
file1.Upload()
      

print('title: %s, mimeType: %s' % (file1['title'], file1['mimeType']))