import re
class TokenType:
    IDENTIFIER = "IDENTIFIER"
    BOOLEAN = "BOOLEAN"
    COMPARISON = "COMPARISON"
    LOGICAL = "LOGICAL"
    KEYWORD = "KEYWORD"
    ASSIGN = "ASSIGN"
    SEMICOLON = "SEMICOLON"
    LPAREN = "LPAREN"
    RPAREN = "RPAREN"
    COMMENT = "COMMENT"
class Token:
    def __init__(self, type_, value):
        self.type = type_
        self.value = value
    def __repr__(self):
        return f"Token({self.type}, {self.value!r})"
class Lexer:
    def __init__(self, text):
        self.text = text
        self.tokens = []
        self.patterns = [
            (TokenType.COMMENT,   re.compile(r"^(//.*|/\*[\s\S]*?\*/)")),
            ("WHITESPACE",        re.compile(r"^\s+")),
            (TokenType.BOOLEAN,   re.compile(r"^(true|false)\b")),
            (TokenType.LOGICAL,   re.compile(r"^(and|or|not)\b")),
            (TokenType.KEYWORD,   re.compile(r"^(if|then)\b")),
            (TokenType.COMPARISON, re.compile(r"^(=|<|>)")),
            (TokenType.ASSIGN,    re.compile(r"^:=")),
            (TokenType.IDENTIFIER, re.compile(r"^[a-zA-Z_][a-zA-Z0-9_]*")),
            (TokenType.SEMICOLON, re.compile(r"^;")),
            (TokenType.LPAREN,    re.compile(r"^\(")),
            (TokenType.RPAREN,    re.compile(r"^\)")),
        ]
    def tokenize(self):
        pos = 0
        while pos < len(self.text):
            substring = self.text[pos:]
            matched = False
            for token_type, pattern in self.patterns:
                match = pattern.match(substring)
                if match:
                    value = match.group(0)
                    if token_type not in ("WHITESPACE", TokenType.COMMENT):
                        if token_type == TokenType.IDENTIFIER and len(value) > 32:
                            raise ValueError(f"Ошибка: идентификатор '{value}' превышает 32 символа")
                        self.tokens.append(Token(token_type, value))
                    pos += len(value)
                    matched = True
                    break
            if not matched:
                raise ValueError(f"Неожиданный символ '{substring[0]}' на позиции {pos}")
        return self.tokens
def analyze_file(filename):
    with open(filename, "r", encoding="utf-8") as f:
        text = f.read()
    lexer = Lexer(text)
    try:
        tokens = lexer.tokenize()
        print(f"{'Тип токена':15} | {'Значение':20}")
        print("-" * 40)
        for t in tokens:
            print(f"{t.type:15} | {t.value:20}")
    except ValueError as e:
        print("Лексическая ошибка:", e)
if __name__ == "__main__":
    analyze_file("input.txt")
