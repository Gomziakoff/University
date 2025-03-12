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
        """Поиск данных по ключевому слову во всех полях, с учетом регистра."""
        if not keyword.strip():
            return data  # Если поле пустое, возвращаем весь словарь

        keyword = keyword.lower()
        filtered_data = []

        for item in data:
            matched_word_forms = [
                wf
                for wf in item["word_forms"]
                if any(keyword in str(value).lower() for value in wf.values())
            ]

            if matched_word_forms:
                filtered_data.append(
                    {"lemma": item["lemma"], "word_forms": matched_word_forms}
                )

        return filtered_data
