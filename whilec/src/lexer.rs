use logos::Logos;

#[derive(Debug, Logos, Clone, PartialEq)]
#[logos(skip(".|\n", priority = 0))]
pub enum Token {
    #[regex(r"\d+", |int| int.slice().parse::<i32>().unwrap())]
    Int(i32),

    #[token("+")]
    Add,
    #[token("-")]
    Sub,
    #[token("*")]
    Mul,

    #[token("(")]
    OpenParen,
    #[token(")")]
    CloseParen,

    #[token("false")]
    False,
    #[token("true")]
    True,

    #[token("=")]
    Eq,
    #[token("<")]
    Lt,
    #[token("<=")]
    LEq,
    #[token(">")]
    Gt,
    #[token(">=")]
    GEq,

    #[token("&")]
    And,
    #[token("|")]
    Or,
    #[token("!")]
    Not,

    #[token(":=")]
    Assign,

    #[token("skip")]
    Skip,

    #[token(";")]
    SemiColon,

    #[token("if")]
    If,
    #[token("then")]
    Then,
    #[token("else")]
    Else,
    #[token("while")]
    While,
    #[token("do")]
    Do,

    #[regex(r"[A-Za-z][0-9A-Za-z]*", |ident| ident.slice().to_string())]
    Ident(String),
}

pub fn lex(input: String) -> Vec<Token> {
    let code = std::fs::read_to_string(input).expect("Failed to read file");
    let lexer = Token::lexer(&code);
    let mut tokens = vec![];
    for (token, span) in lexer.spanned() {
        match token {
            Ok(token) => tokens.push(token),
            Err(_) => {
                panic!("lexer error at {:?}", span);
            }
        }
    }
    tokens
}
