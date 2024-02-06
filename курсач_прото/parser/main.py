import requests
from bs4 import BeautifulSoup

req = requests.get("https://zippybus.com/by/brest/route/bus")
src = req.text
soup = BeautifulSoup(src,"lxml")

stops = soup.find_all("li")
bus = []
for item in stops:
    bus.append(item.text.strip())
print(bus)