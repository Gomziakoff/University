# some imports
from googleAPILib import *
from bs4 import BeautifulSoup
import csv
import os
import datetime
import requests

# month's to paste in link
months = {
    1: 'января',
    2: 'февраля',
    3: 'марта',
    4: 'апреля',
    5: 'мая',
    6: 'июня',
    7: 'июля',
    8: 'августа',
    9: 'сентября',
    10: 'октября',
    11: 'ноября',
    12: 'декабря'
}


# func to get ids of .csv files from folders_id.csv
def get_ids():
    try:
        with open('folders_id.csv', 'r', newline='') as csvfile:
            csvreader = csv.reader(csvfile)
            next(csvreader)
            ids = []
            for row in csvreader:
                ids.append(row[0])
            return ids
    except FileNotFoundError as err:
        print(f"An error occurred: {err}")


# func to download .csv files from list of ids
def download_all_csv(ids):
    file_ids = []
    if ids:
        for id in ids:
            try:
                files = get_files_by_mimetype_google_folder('text/csv',id)
                for file in files:
                    file_ids.append(file['id'])
            except:
                continue
        for file in file_ids:
            download_file(file)
    else:
        print("No new files downloaded. Only local files (if any) will be processed.")
        return


# simple splitting csv files
def split_csv():
    data_folder_path = 'download/'
    file_list = os.listdir(data_folder_path)
    # Читаем каждый CSV файл и объединяем их в один DataFrame
    try:
        combined_df = pd.concat((pd.read_csv(data_folder_path+file) for file in file_list), ignore_index=True)
    except ValueError as err:
        print(err)
        return
    # Сохраняем объединенный DataFrame в новый CSV файл
    upload_folder_path = 'upload/'
    if not os.path.exists(upload_folder_path):
        os.makedirs(upload_folder_path)
    else:
        pass
    print(combined_df)
    combined_df.to_csv(upload_folder_path+'data.csv', index=False)
    # for data in combined_df['время начала фиксации (глобальное)']:
    #     get_weather(data)
    upload_file('1uaUSsETcJ4K93xingfVuDTwzdGNkppmf','data.csv',upload_folder_path+'data.csv')


# func to parce historical weather
def get_weather(unix_time):
    date = datetime.datetime.fromtimestamp(unix_time)
    try:
        req = requests.get(f"https://brest.nuipogoda.ru/{date.day}-{months[date.month]}#{date.year}")
    except:
        print("Check your internet connection")
        return
    src = req.text
    soup = BeautifulSoup(src, "lxml")

    rows_with_time = soup.find_all('tr', {'time':True})
    weather_data = {}
    for row in rows_with_time:
        time = int(row.find('span', class_='c1').text[:2])
        precipitation = row.find('div', class_='i')['title']
        temperature = row.find('span', class_='ht').text
        wind_direction = row.find('span', class_='wd').text
        wind_speed = row.find('span', class_='ws').text
        weather_data[time] = {'precipitation':precipitation, 'temperature':temperature,'wind_direction':wind_direction,'wind_speed':wind_speed}
    print(weather_data[date.hour - (date.hour % 3)])

#get_weather(1983644559.2516)
download_all_csv(get_ids())
split_csv()
