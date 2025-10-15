use crate::lexer::Token;

#[derive(Debug, Clone)]
pub enum ArithmeticOp {
    Add,
    Sub,
    Mul,
}

#[derive(Debug, Clone)]
pub enum Arithmetic {
    Int(i32),
    Ident(String),
    Binary {
        lhs: Box<Arithmetic>,
        op: ArithmeticOp,
        rhs: Box<Arithmetic>,
    },
}

#[derive(Debug, Clone)]
pub enum BooleanOp {
    Eq,
    LEq,
    Not,
}

#[derive(Debug, Clone)]
pub enum Boolean {
    False,
    True,
    Binary {
        lhs: Box<Arithmetic>,
        op: BooleanOp,
        rhs: Box<Arithmetic>,
    },
}

#[derive(Debug, Clone)]
pub enum Statement {
    Assignment {
        ident: String,
        value: Arithmetic,
    },
    Skip,
    Composision(Box<Statement>, Box<Statement>),
    If {
        condition: Boolean,
        if_branch: Box<Statement>,
        else_branch: Box<Statement>,
    },
    While {
        condition: Boolean,
        body: Box<Statement>,
    },
}

pub struct Parser {
    pub tokens: Vec<Token>,
    pub index: usize,
}

impl Parser {
    fn peek(&mut self) -> Option<Token> {
        self.tokens.get(self.index).cloned()
    }

    fn next(&mut self) -> Option<Token> {
        let token = self.tokens.get(self.index).cloned();
        self.index += 1;
        token
    }

    fn expect(&mut self, expected: Token) {
        let token = self.next();
        if let Some(actual) = token
            && expected != actual
        {
            panic!("error: expected {expected:?}, found {actual:?}");
        }
    }

    pub fn parse(&mut self) -> Statement {
        let mut statement = None;
        while self.index < self.tokens.len() {
            statement = Some(self.parse_statement(statement.clone()));
        }
        if let Some(statement) = statement {
            statement
        } else {
            panic!("empty program!");
        }
    }

    fn parse_statement(&mut self, lhs: Option<Statement>) -> Statement {
        let token = self.next().expect("unexpected end of file");
        if token != Token::SemiColon && lhs.is_some() {
            panic!("expected `;`, found `{token:?}`");
        }
        match token {
            Token::Skip => Statement::Skip,
            Token::SemiColon => {
                if let Some(lhs) = lhs {
                    match lhs {
                        Statement::Composision(statement1, statement2) => Statement::Composision(
                            statement1,
                            Box::new(Statement::Composision(
                                statement2,
                                Box::new(self.parse_statement(None)),
                            )),
                        ),
                        lhs => Statement::Composision(
                            Box::new(lhs),
                            Box::new(self.parse_statement(None)),
                        ),
                    }
                } else {
                    panic!("No left hand side of composision");
                }
            }
            Token::If => self.parse_if(),
            Token::While => self.parse_while(),
            Token::Ident(ident) => self.parse_assignment(ident),
            t => panic!("unexpected token {t:?}"),
        }
    }

    fn parse_arithmetic(&mut self) -> Arithmetic {
        let mut lhs = self.parse_arithmetic_component();
        while let Some(token) = self.peek() {
            match token {
                Token::Add | Token::Sub | Token::Mul => {
                    let operator_token = self.next().expect("unexpected end of file");
                    let op = self.get_arithmetic_operator(operator_token);
                    let rhs = self.parse_arithmetic_component();
                    lhs = match rhs {
                        Arithmetic::Binary {
                            lhs: rhs_lhs,
                            op: rhs_op,
                            rhs: rhs_rhs,
                        } => Arithmetic::Binary {
                            lhs: Box::new(Arithmetic::Binary {
                                lhs: Box::new(lhs),
                                op,
                                rhs: rhs_lhs,
                            }),
                            op: rhs_op,
                            rhs: rhs_rhs,
                        },
                        rhs => Arithmetic::Binary {
                            lhs: Box::new(lhs),
                            op,
                            rhs: Box::new(rhs),
                        },
                    };
                }
                _ => return lhs,
            }
        }
        lhs
    }

    fn parse_arithmetic_component(&mut self) -> Arithmetic {
        let token = self.next().expect("unexpected end of file");
        match token {
            Token::Int(int) => Arithmetic::Int(int),
            Token::Ident(ident) => Arithmetic::Ident(ident),
            t => panic!("unexpected token {t:?}"),
        }
    }

    fn parse_boolean(&mut self) -> Boolean {
        let lhs = match self.peek().expect("unexpected end of file") {
            Token::False => {
                self.next();
                return Boolean::False;
            }
            Token::True => {
                self.next();
                return Boolean::True;
            }
            Token::Not => {
                self.next();
                return self.parse_boolean();
            }
            _ => self.parse_arithmetic(),
        };

        let token = self.next().expect("unexpected end of file");
        match token {
            Token::Eq | Token::LEq => {
                let op = self.get_boolean_operator(token);
                let rhs = self.parse_arithmetic();
                Boolean::Binary {
                    lhs: Box::new(lhs),
                    op,
                    rhs: Box::new(rhs),
                }
            }
            t => panic!("unexpected token {t:?}"),
        }
    }

    fn parse_assignment(&mut self, ident: String) -> Statement {
        self.expect(Token::Assign);
        let value = self.parse_arithmetic();

        Statement::Assignment { ident, value }
    }

    fn parse_if(&mut self) -> Statement {
        let condition = self.parse_boolean();
        self.expect(Token::Then);
        let if_branch = Box::new(self.parse_statement(None));
        self.expect(Token::Else);
        let else_branch = Box::new(self.parse_statement(None));

        Statement::If {
            condition,
            if_branch,
            else_branch,
        }
    }

    fn parse_while(&mut self) -> Statement {
        let condition = self.parse_boolean();
        self.expect(Token::Do);
        let body = Box::new(self.parse_statement(None));

        Statement::While { condition, body }
    }

    fn get_arithmetic_operator(&self, token: Token) -> ArithmeticOp {
        match token {
            Token::Add => ArithmeticOp::Add,
            Token::Sub => ArithmeticOp::Sub,
            Token::Mul => ArithmeticOp::Mul,
            t => panic!("unexpected token {t:?}"),
        }
    }

    fn get_boolean_operator(&self, token: Token) -> BooleanOp {
        match token {
            Token::Eq => BooleanOp::Eq,
            Token::LEq => BooleanOp::LEq,
            Token::Not => BooleanOp::Not,
            t => panic!("unexpected token {t:?}"),
        }
    }
}
