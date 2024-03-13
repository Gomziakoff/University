import requests
from bs4 import BeautifulSoup

req = requests.get("https://brestcity.com/blog/raspisanie-i-puti-peredvizheniya-marshrutok-bresta")
src = req.text
soup = BeautifulSoup(src,"lxml")

stops = soup.find_all("h3")
bus = []
for item in stops:
    bus.append(item.text.strip())
print(bus)