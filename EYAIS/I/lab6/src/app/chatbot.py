from Levenshtein import distance
from collections import Counter
from data import pairs


class ChatBot:
    def __init__(self):
        self.pairs = pairs.pairs

    def find_best_match(self, user_input):
        best_match = None
        max_similarity = 0

        user_lower = user_input.lower()
        user_words = user_lower.split()
        user_counter = Counter(user_words)
        user_length = len(user_words)

        for pair in self.pairs:
            question = pair[0]
            question_lower = question.lower()
            question_words = question_lower.split()

            lev_dist = distance(user_lower, question_lower)
            lev_sim = 1 - (lev_dist / max(len(user_lower), len(question_lower)))

            common_words = sum((user_counter & Counter(question_words)).values())
            word_sim = common_words / user_length if user_length > 0 else 0

            total_sim = 0.6 * lev_sim + 0.3 * word_sim

            if total_sim > max_similarity and total_sim > 0.4:
                max_similarity = total_sim
                best_match = pair

        return best_match[1][0] if best_match else None