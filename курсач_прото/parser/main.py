import requests
from bs4 import BeautifulSoup
import json
import random
req = requests.get("https://zippybus.com/by/brest/stop")
src = req.text
soup = BeautifulSoup(src,"lxml")

stops = soup.find_all(class_="list-group-item stop-name focusLink")
bus = []
data = {}
data['stops'] = []
for item in stops:
    title = "Остановка: "+item.text.strip()
    data['stops'].append({"title":title,"position":{"lat":random.randrange(50, 53),"lng":random.randrange(22,24)}})
    data['stops'].append({"title":title+"1", "position": {"lat":random.randrange(50, 53),"lng":random.randrange(22,24)}})
with open('data.json', 'w') as f:
    json.dump(data, f, ensure_ascii=False, indent=2)

print(data)