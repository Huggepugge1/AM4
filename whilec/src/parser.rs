use crate::lexer::Token;

#[derive(Debug, Clone)]
pub enum ArithmeticOp {
    Add,
    Sub,
    Mul,
}

impl std::fmt::Display for ArithmeticOp {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            ArithmeticOp::Add => writeln!(f, "add"),
            ArithmeticOp::Sub => writeln!(f, "sub"),
            ArithmeticOp::Mul => writeln!(f, "mul"),
        }
    }
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
    Paren(Box<Arithmetic>),
}

#[derive(Debug, Clone)]
pub enum BooleanOp {
    Eq,
    Lt,
    LEq,
    Gt,
    GEq,

    And,
    Or,

    Not,
}

impl std::fmt::Display for BooleanOp {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            BooleanOp::Eq => write!(f, "eq"),
            BooleanOp::Lt => write!(f, "lt"),
            BooleanOp::LEq => write!(f, "le"),
            BooleanOp::Gt => write!(f, "gt"),
            BooleanOp::GEq => write!(f, "ge"),

            BooleanOp::And => write!(f, "land"),
            BooleanOp::Or => write!(f, "lor"),

            BooleanOp::Not => write!(f, "lnot"),
        }
    }
}

#[derive(Debug, Clone)]
pub enum Boolean {
    False,
    True,
    Binary {
        lhs: Box<Boolean>,
        op: BooleanOp,
        rhs: Box<Boolean>,
    },
    Cmp {
        lhs: Box<Arithmetic>,
        op: BooleanOp,
        rhs: Box<Arithmetic>,
    },
    Paren(Box<Boolean>),
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
    Paren(Box<Statement>),
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

    fn parse_statement_component(&mut self) -> Statement {
        let token = self.next().expect("unexpected end of file");
        match token {
            Token::OpenParen => {
                let statement = self.parse_statement();
                self.expect(Token::CloseParen);
                Statement::Paren(Box::new(statement))
            }
            Token::Skip => Statement::Skip,
            Token::If => self.parse_if(),
            Token::While => self.parse_while(),
            Token::Ident(ident) => self.parse_assignment(ident),
            t => panic!("unexpected token {t:?}"),
        }
    }

    pub fn parse_statement(&mut self) -> Statement {
        let mut lhs = self.parse_statement_component();

        while let Some(token) = self.peek() {
            if token == Token::SemiColon {
                self.next();
                lhs = match lhs {
                    Statement::Composision(statement1, statement2) => Statement::Composision(
                        statement1,
                        Box::new(Statement::Composision(
                            statement2,
                            Box::new(self.parse_statement()),
                        )),
                    ),
                    lhs => Statement::Composision(
                        Box::new(lhs),
                        Box::new(self.parse_statement_component()),
                    ),
                };
            } else {
                break;
            }
        }

        lhs
    }
    fn parse_arithmetic_component(&mut self) -> Arithmetic {
        let token = self.next().expect("unexpected end of file");
        match token {
            Token::Int(int) => Arithmetic::Int(int),
            Token::Ident(ident) => Arithmetic::Ident(ident),
            Token::OpenParen => Arithmetic::Paren(Box::new(self.parse_arithmetic())),
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

    fn parse_boolean_component(&mut self) -> Boolean {
        let token = self.next().expect("unexpected end of file");
        let lhs = match token {
            Token::OpenParen => {
                let boolean = self.parse_boolean();
                self.expect(Token::CloseParen);
                return Boolean::Paren(Box::new(boolean));
            }
            Token::False => return Boolean::False,
            Token::True => return Boolean::True,
            Token::Int(int) => Arithmetic::Int(int),
            Token::Ident(ident) => Arithmetic::Ident(ident),
            t => panic!("unexpected token {t:?}"),
        };
        match self.peek().expect("unexpected end of file") {
            Token::Eq | Token::Lt | Token::LEq | Token::Gt | Token::GEq => {
                let operator_token = self.next().expect("unexpected end of file");
                let op = self.get_boolean_operator(operator_token);
                let rhs = self.parse_arithmetic();
                Boolean::Cmp {
                    lhs: Box::new(lhs),
                    op,
                    rhs: Box::new(rhs),
                }
            }
            t => panic!("unexpected token {t:?}"),
        }
    }

    fn parse_boolean(&mut self) -> Boolean {
        let mut lhs = self.parse_boolean_component();
        while let Some(token) = self.peek() {
            match token {
                Token::And | Token::Or => {
                    let operator_token = self.next().expect("unexpected end of file");
                    let op = self.get_boolean_operator(operator_token);
                    let rhs = self.parse_boolean_component();
                    lhs = match rhs {
                        Boolean::Binary {
                            lhs: rhs_lhs,
                            op: rhs_op,
                            rhs: rhs_rhs,
                        } => Boolean::Binary {
                            lhs: Box::new(Boolean::Binary {
                                lhs: Box::new(lhs),
                                op,
                                rhs: rhs_lhs,
                            }),
                            op: rhs_op,
                            rhs: rhs_rhs,
                        },
                        rhs => Boolean::Binary {
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

    fn parse_assignment(&mut self, ident: String) -> Statement {
        self.expect(Token::Assign);
        let value = self.parse_arithmetic();

        Statement::Assignment { ident, value }
    }

    fn parse_if(&mut self) -> Statement {
        let condition = self.parse_boolean();
        self.expect(Token::Then);
        let if_branch = Box::new(self.parse_statement_component());
        self.expect(Token::Else);
        let else_branch = Box::new(self.parse_statement_component());

        Statement::If {
            condition,
            if_branch,
            else_branch,
        }
    }

    fn parse_while(&mut self) -> Statement {
        let condition = self.parse_boolean();
        self.expect(Token::Do);
        let body = Box::new(self.parse_statement_component());

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
            Token::Lt => BooleanOp::Lt,
            Token::LEq => BooleanOp::LEq,
            Token::Gt => BooleanOp::Gt,
            Token::GEq => BooleanOp::GEq,

            Token::And => BooleanOp::And,
            Token::Or => BooleanOp::Or,

            Token::Not => BooleanOp::Not,
            t => panic!("unexpected token {t:?}"),
        }
    }
}
