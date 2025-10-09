import json

class DataManager:
    @staticmethod
    def save_data(data, filename):
        with open(filename, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=4)

    @staticmethod
    def load_data(filename):
        with open(filename, "r", encoding="utf-8") as f:
            return json.load(f)

    @staticmethod
    def search_data(data, keyword):
        """Поиск предложений по ключевому слову"""
        keyword = keyword.lower()
        result = []
        for sentence in data:
            if any(keyword in str(value).lower()
                   for word in sentence["words"]
                   for value in word.values()):
                result.append(sentence)
        return result