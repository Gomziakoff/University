import pandas as pd
from google.oauth2 import service_account
from googleapiclient.http import MediaIoBaseDownload, MediaFileUpload
from googleapiclient.discovery import build
import io

SCOPES = ['https://www.googleapis.com/auth/drive']
SERVICE_ACCOUNT_FILE = 'key.json'
credentials = service_account.Credentials.from_service_account_file(SERVICE_ACCOUNT_FILE, scopes=SCOPES)


def upload_file(folder_id,name,file_path):
    service = build('drive', 'v3', credentials=credentials)

    file_metadata = {
        'name': name,
        'parents': [folder_id]
    }
    media = MediaFileUpload(file_path, resumable=True)
    r = service.files().create(body=file_metadata, media_body=media, fields='id').execute()
    print(r)


def get_folder_name(folder_id):
    # ------------- Build Service -------------
    service = build('drive', 'v3', credentials=credentials)
    try:
        return service.files().get(fileId=folder_id).execute()['name']
    except:
        return None


def get_files_from_google_folder(folder_id):
    # ------------- Build Service -------------
    service = build('drive', 'v3', credentials=credentials)
    if folder_id:
        query = f"'{folder_id}' in parents"
        folder_name = get_folder_name(folder_id)
    else:
        query = ""
        folder_name = None

    results = service.files().list(
        q=query,
        pageSize=1000,
        fields="nextPageToken, files(id, name, mimeType, parents, fileExtension)").execute()
    items = results.get('files', [])

    # ----------- Create DataFrame -------------
    df_files = pd.DataFrame(pd.DataFrame(data=items))
    df_files['parent_folder_name'] = folder_name
    return items


def get_files_from_google_folders():
    # ------------- Build Service -------------
    service = build('drive', 'v3', credentials=credentials)
    results = service.files().list(
        pageSize=1000,
        fields="nextPageToken, files(id, name, mimeType, parents, fileExtension)").execute()
    nextPageToken = results.get('nextPageToken')
    while nextPageToken:
        nextPage = service.files().list(pageSize=1000,
                                        fields="nextPageToken, files(id ,name, mimeType, parents, fileExtension)",
                                        pageToken=nextPageToken).execute()
        nextPageToken = nextPage.get('nextPageToken')
        results['files'] = results['files'] + nextPage['files']
    items = results.get('files', [])

    # ----------- Create DataFrame -------------
    df_files = pd.DataFrame(pd.DataFrame(data=items))
    return df_files


def get_files_by_mimetype_google_folder(mimeType,folder_id):
    service = build('drive', 'v3', credentials=credentials)
    results = service.files().list(
        q=f"mimeType='{mimeType}' and '{folder_id}' in parents",
        pageSize=10,
        fields="nextPageToken, files(id, name, mimeType, parents, fileExtension)").execute()
    nextPageToken = results.get('nextPageToken')
    while nextPageToken:
        nextPage = service.files().list(q=f"mimeType='{mimeType}' and '{folder_id}' in parents",
                                        pageSize=1000,
                                        fields="nextPageToken, files(id ,name, mimeType, parents, fileExtension)",
                                        pageToken=nextPageToken).execute()
        nextPageToken = nextPage.get('nextPageToken')
        results['files'] = results['files'] + nextPage['files']
    items = results.get('files', [])

    return items


def download_file(file_id):
    service = build('drive', 'v3', credentials=credentials)
    filename = 'download/'
    filename += service.files().get(fileId=file_id).execute()['name']
    request = service.files().get_media(fileId=file_id)
    fh = io.FileIO(filename, 'wb')
    downloader = MediaIoBaseDownload(fh, request)
    done = False
    while done is False:
        status, done = downloader.next_chunk()
        print("Download %d%%." % int(status.progress() * 100))

if __name__ == '__main__':
    #upload_file('1q9tAPkIM7NlLMiZhhYGafFdlMwsG7DWL','file.txt','file.txt')
    #print(get_files_from_google_folder('1q9tAPkIM7NlLMiZhhYGafFdlMwsG7DWL'))
    #print(get_files_from_google_folders())
    #print(get_files_by_mimetype_google_folder('text/plain','1q9tAPkIM7NlLMiZhhYGafFdlMwsG7DWL'))
    download_file('1yr--A8tkVVVt3nmCyabPDemTuI11CpM0')