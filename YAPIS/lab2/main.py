import re
from enum import Enum
from typing import List, Tuple, Optional
class TokenType(Enum):
    IF = "IF"
    THEN = "THEN"
    TRUE = "TRUE"
    FALSE = "FALSE"
    AND = "AND"
    OR = "OR"
    NOT = "NOT"
    IDENTIFIER = "IDENTIFIER"
    NUMBER = "NUMBER"
    STRING = "STRING"
    EQUAL = "EQUAL"
    NOT_EQUAL = "NOT_EQUAL"
    GREATER = "GREATER"
    LESS = "LESS"
    GREATER_EQUAL = "GREATER_EQUAL"
    LESS_EQUAL = "LESS_EQUAL"
    AND_OP = "AND_OP"
    OR_OP = "OR_OP"
    NOT_OP = "NOT_OP"
    SEMICOLON = "SEMICOLON"
    LPAREN = "LPAREN"
    RPAREN = "RPAREN"
    ASSIGN = "ASSIGN"
    COMMA = "COMMA"
    EOF = "EOF"

class Token:
    def __init__(self, type: TokenType, value: str, line: int, column: int):
        self.type = type
        self.value = value
        self.line = line
        self.column = column

    def __str__(self):
        return f"Token({self.type.name}, '{self.value}', line={self.line}, col={self.column})"

    def __repr__(self):
        return self.__str__()

class LexerError(Exception):
    def __init__(self, message: str, line: int, column: int):
        super().__init__(f"Lexer error at line {line}, column {column}: {message}")
        self.line = line
        self.column = column

class Lexer:
    def __init__(self, source: str):
        self.source = source
        self.position = 0
        self.line = 1
        self.column = 1
        self.tokens = []

    def current_char(self) -> Optional[str]:
        if self.position >= len(self.source):
            return None
        return self.source[self.position]

    def peek_char(self) -> Optional[str]:
        if self.position + 1 >= len(self.source):
            return None
        return self.source[self.position + 1]

    def advance(self):
        if self.current_char() == '\n':
            self.line += 1
            self.column = 1
        else:
            self.column += 1
        self.position += 1

    def skip_whitespace(self):
        while self.current_char() and self.current_char().isspace():
            self.advance()

    def skip_comment(self):
        if self.current_char() == '/' and self.peek_char() == '*':
            self.advance()
            self.advance()

            while self.current_char():
                if self.current_char() == '*' and self.peek_char() == '/':
                    self.advance()
                    self.advance()
                    break
                self.advance()

    def read_identifier(self) -> str:
        start = self.position
        while (self.current_char() and
               (self.current_char().isalnum() or self.current_char() == '_')):
            self.advance()
        return self.source[start:self.position]

    def read_number(self) -> str:
        start = self.position
        while self.current_char() and self.current_char().isdigit():
            self.advance()
        return self.source[start:self.position]

    def read_string(self) -> str:
        start = self.position
        self.advance()

        while self.current_char() and self.current_char() != '"':
            if self.current_char() == '\\':
                self.advance()
            self.advance()

        if not self.current_char():
            raise LexerError("Unterminated string literal", self.line, self.column)

        end = self.position
        self.advance()

        return self.source[start:end]

    def add_token(self, type: TokenType, value: str = None):
        if value is None:
            value = type.value.lower()
        token = Token(type, value, self.line, self.column - len(value))
        self.tokens.append(token)

    def tokenize(self) -> List[Token]:
        keywords = {
            'if': TokenType.IF,
            'then': TokenType.THEN,
            'true': TokenType.TRUE,
            'false': TokenType.FALSE,
            'and': TokenType.AND,
            'or': TokenType.OR,
            'not': TokenType.NOT
        }

        while self.current_char():
            if self.current_char().isspace():
                self.skip_whitespace()
                continue

            if self.current_char() == '/' and self.peek_char() == '*':
                self.skip_comment()
                continue

            if self.current_char().isalpha() or self.current_char() == '_':
                identifier = self.read_identifier()
                if len(identifier) > 32:
                    raise LexerError(f"Identifier too long (max 32 chars): {identifier}",
                                     self.line, self.column - len(identifier))

                if identifier.lower() in keywords:
                    self.add_token(keywords[identifier.lower()], identifier)
                else:
                    self.add_token(TokenType.IDENTIFIER, identifier)
                continue

            if self.current_char().isdigit():
                number = self.read_number()
                self.add_token(TokenType.NUMBER, number)
                continue

            if self.current_char() == '"':
                string_literal = self.read_string()
                if len(string_literal) > 32:
                    raise LexerError(f"String too long (max 32 chars): {string_literal}",
                                     self.line, self.column - len(string_literal))
                self.add_token(TokenType.STRING, string_literal)
                continue

            if self.current_char() == '=':
                self.advance()
                self.add_token(TokenType.EQUAL, '=')
                continue

            if self.current_char() == '>':
                self.advance()
                if self.current_char() == '=':
                    self.advance()
                    self.add_token(TokenType.GREATER_EQUAL, '>=')
                else:
                    self.add_token(TokenType.GREATER, '>')
                continue

            if self.current_char() == '<':
                self.advance()
                if self.current_char() == '=':
                    self.advance()
                    self.add_token(TokenType.LESS_EQUAL, '<=')
                elif self.current_char() == '>':
                    self.advance()
                    self.add_token(TokenType.NOT_EQUAL, '<>')
                else:
                    self.add_token(TokenType.LESS, '<')
                continue

            if self.current_char() == '!':
                self.advance()
                if self.current_char() == '=':
                    self.advance()
                    self.add_token(TokenType.NOT_EQUAL, '!=')
                else:
                    raise LexerError(f"Unexpected character: !", self.line, self.column - 1)
                continue

            if self.current_char() == ';':
                self.advance()
                self.add_token(TokenType.SEMICOLON, ';')
                continue

            if self.current_char() == '(':
                self.advance()
                self.add_token(TokenType.LPAREN, '(')
                continue

            if self.current_char() == ')':
                self.advance()
                self.add_token(TokenType.RPAREN, ')')
                continue

            if self.current_char() == ':':
                self.advance()
                if self.current_char() == '=':
                    self.advance()
                    self.add_token(TokenType.ASSIGN, ':=')
                else:
                    raise LexerError(f"Unexpected character: :", self.line, self.column - 1)
                continue

            if self.current_char() == ',':
                self.advance()
                self.add_token(TokenType.COMMA, ',')
                continue
            raise LexerError(f"Unexpected character: {self.current_char()}", self.line, self.column)
        self.add_token(TokenType.EOF, '')
        return self.tokens

def print_tokens_table(tokens: List[Token]):
    print("=" * 80)
    print(f"{'Type':<20} {'Value':<20} {'Line':<8} {'Column':<8}")
    print("=" * 80)

    for token in tokens:
        if token.type == TokenType.EOF:
            continue
        print(f"{token.type.name:<20} {repr(token.value):<20} {token.line:<8} {token.column:<8}")

    print("=" * 80)

def main():
    import sys
    import os

    if len(sys.argv) != 2:
        print("Usage: python lexer.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]

    if not os.path.exists(input_file):
        print(f"Error: File '{input_file}' not found")
        sys.exit(1)

    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            source_code = f.read()

        lexer = Lexer(source_code)
        tokens = lexer.tokenize()

        print("Lexical analysis completed successfully!")
        print_tokens_table(tokens)

    except LexerError as e:
        print(f"Error: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()