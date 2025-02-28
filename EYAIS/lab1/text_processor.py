from collections import defaultdict

import pymorphy3


class TextProcessor:
    def __init__(self):
        self.morph = pymorphy3.MorphAnalyzer()

    def process_text(self, words):
        """Обрабатывает слова из текста"""
        lemmas_info = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))

        for word in words:
            parsed_word = self.morph.parse(word)[0]
            lemma = parsed_word.normal_form  # Нормальная форма (лексема)
            pos = parsed_word.tag.POS  # Часть речи
            case = parsed_word.tag.case  # Падеж
            number = parsed_word.tag.number  # Число
            tense = parsed_word.tag.tense  # Время
            rus_pos = self.determine_pos(pos)  # Часть речи
            rus_case = self.determine_case(case)  # Падеж
            rus_number = self.determine_number(number)  # Число
            rus_tense = self.determine_tense(tense)  # Время
            rus_role = self.determine_role(pos, case, tense)  # Роль в предложении

            lemmas_info[lemma][word][
                (rus_pos, rus_case, rus_number, rus_tense, rus_role)
            ] += 1
        return self.prepare_data(lemmas_info)

    @staticmethod
    def prepare_data(lemmas_info):
        """Метод для сборки обработанных слов в красивый словарик"""
        result = []
        for lemma, word_forms in lemmas_info.items():
            lemma_info = {"lemma": lemma, "word_forms": []}
            for word, forms in word_forms.items():
                for (pos, case, number, tense, role), count in forms.items():
                    lemma_info["word_forms"].append(
                        {
                            "word": word,
                            "pos": pos,
                            "case": case,
                            "number": number,
                            "tense": tense,
                            "role": role,
                            "count": f"{count}/{len(lemmas_info)}",  # Частота встречаемости
                        }
                    )
            result.append(lemma_info)
        result.sort(key=lambda x: x["lemma"])
        return result

    def determine_role(self, pos, case, tense):
        if pos == "NOUN":  # Существительное
            if case == "nomn":  # Именительный падеж
                return "Подлежащее"
            if case in [
                "gent",
                "datv",
                "accs",
                "ablt",
                "loct",
            ]:  # Родительный, дательный, винительный, творительный, предложный
                return "Дополнение"
        elif pos == "ADJF":  # Полное прилагательное
            return "Определение"
        elif pos == "ADJS":  # Краткое прилагательное
            return "Именная часть составного сказуемого"
        elif pos == "COMP":  # Компаратив
            return "Определение (сравнительная степень)"
        elif pos == "VERB":  # Глагол (личная форма)
            if tense == "pres" or tense == "futr":  # Настоящее или будущее время
                return "Сказуемое"
            if tense == "past":  # Прошедшее время
                return "Сказуемое (прошедшее время)"
        elif pos == "INFN":  # Инфинитив
            return "Сказуемое (инфинитив)"
        elif pos == "PRTF":  # Полное причастие
            return "Определение (причастный оборот)"
        elif pos == "PRTS":  # Краткое причастие
            return "Именная часть составного сказуемого"
        elif pos == "GRND":  # Деепричастие
            return "Обстоятельство (деепричастный оборот)"
        elif pos == "NUMR":  # Числительное
            if case == "nomn":  # Именительный падеж
                return "Подлежащее (числительное)"
            return "Дополнение (числительное)"
        elif pos == "ADVB":  # Наречие
            return "Обстоятельство"
        elif pos == "NPRO":  # Местоимение-существительное
            if case == "nomn":  # Именительный падеж
                return "Подлежащее (местоимение)"
            return "Дополнение (местоимение)"
        elif pos == "PRED":  # Предикатив
            return "Сказуемое (предикатив)"
        elif pos == "PREP":  # Предлог
            return "Предлог"
        elif pos == "CONJ":  # Союз
            return "Союз"
        elif pos == "PRCL":  # Частица
            return "Частица"
        elif pos == "INTJ":  # Междометие
            return "Междометие"
        else:
            return "-"

    def determine_pos(self, pos):
        """Переводит часть речи на русский"""
        pos_map = {
            "NOUN": "имя существительное",
            "ADJF": "имя прилагательное (полное)",
            "ADJS": "имя прилагательное (краткое)",
            "COMP": "компаратив",
            "VERB": "глагол (личная форма)",
            "INFN": "глагол (инфинитив)",
            "PRTF": "причастие (полное)",
            "PRTS": "причастие (краткое)",
            "GRND": "деепричастие",
            "NUMR": "числительное",
            "ADVB": "наречие",
            "NPRO": "местоимение-существительное",
            "PRED": "предикатив",
            "PREP": "предлог",
            "CONJ": "союз",
            "PRCL": "частица",
            "INTJ": "междометие",
        }
        return pos_map.get(pos, "-")

    def determine_case(self, case):
        """Переводит падеж на русский"""
        case_map = {
            "nomn": "именительный",
            "gent": "родительный",
            "datv": "дательный",
            "accs": "винительный",
            "ablt": "творительный",
            "loct": "предложный",
            "voct": "звательный",
            "gen2": "второй родительный",
            "acc2": "второй винительный",
            "loc2": "второй предложный",
        }
        return case_map.get(case, "-")

    def determine_number(self, number):
        """Переводит число на русский"""
        number_map = {"sing": "единственное", "plur": "множественное"}
        return number_map.get(number, "-")

    def determine_tense(self, tense):
        """Переводит время на русский"""
        tense_map = {"pres": "настоящее", "past": "прошедшее", "futr": "будущее"}
        return tense_map.get(tense, "-")
