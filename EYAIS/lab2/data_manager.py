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
    def filter_data(data, condition):
        return [item for item in data if condition(item)]

    @staticmethod
    def search_data(data, keyword):
        """Поиск данных по ключевому слову в поле 'word', с учетом регистра."""
        if not keyword.strip():
            return data  # Если поле пустое, возвращаем весь список данных

        keyword = keyword.lower()
        filtered_data = []

        for item in data:
            if keyword in item["word"].lower():  # Проверяем вхождение в поле 'word'
                filtered_data.append(item)  # Добавляем весь элемент, если найдено совпадение

        return filtered_data

