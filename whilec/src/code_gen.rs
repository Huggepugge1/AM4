use std::collections::HashMap;

use crate::parser::{Arithmetic, Boolean, Declaration, Statement};

#[derive(Clone)]
pub struct CodeGenerator {
    pub output_string: String,
    pub label: usize,
    pub scopes: Vec<HashMap<String, String>>,
    pub var_counter: usize,
}

impl CodeGenerator {
    fn next_label(&mut self) -> String {
        let label = self.label.to_string() + ":";
        self.label += 1;
        label
    }

    pub fn code_gen(&mut self, ast: Statement) {
        match ast {
            Statement::Assignment { ident, value } => {
                let resolved = self.resolve_var(&ident);
                self.code_gen_arithmetic(value.clone());
                self.output_string += &format!("    store {resolved}\n");
            }
            Statement::Skip => self.output_string += "    noop\n",
            Statement::Composition(statement, statement1) => {
                self.code_gen(*statement);
                self.code_gen(*statement1);
            }
            Statement::If {
                condition,
                if_branch,
                else_branch,
            } => self.code_gen_if(condition, *if_branch, *else_branch),
            Statement::While { condition, body } => self.code_gen_while(condition, *body),
            Statement::Paren(statement) => self.code_gen(*statement),
            Statement::Print(arithmetic) => match arithmetic {
                Arithmetic::Int(int) => self.output_string += &format!("    printc {int}\n"),
                Arithmetic::Ident(ident) => {
                    let resolved = self.resolve_var(&ident);
                    self.output_string += &format!("    printv {resolved}\n");
                }
                Arithmetic::Binary { .. } => unreachable!(),
                Arithmetic::Paren(_) => unreachable!(),
            },
            Statement::Block { declarations, body } => self.code_gen_block(declarations, *body),
        }
    }

    fn code_gen_arithmetic(&mut self, ast: Arithmetic) {
        match ast {
            Arithmetic::Int(int) => self.output_string += &format!("    push {int}\n"),
            Arithmetic::Ident(string) => self.output_string += &format!("    fetch {string}\n"),
            Arithmetic::Binary { lhs, op, rhs } => {
                self.code_gen_arithmetic(*lhs);
                self.code_gen_arithmetic(*rhs);
                self.output_string += &format!("    {}\n", op);
            }
            Arithmetic::Paren(arithmetic) => self.code_gen_arithmetic(*arithmetic),
        }
    }

    fn code_gen_boolean(&mut self, ast: Boolean) {
        match ast {
            Boolean::False => self.output_string += "    push false\n",
            Boolean::True => self.output_string += "    push true\n",
            Boolean::Cmp { lhs, op, rhs } => {
                self.code_gen_arithmetic(*lhs);
                self.code_gen_arithmetic(*rhs);
                self.output_string += &format!("    {}\n", op);
            }
            Boolean::Binary { lhs, op, rhs } => {
                self.code_gen_boolean(*lhs);
                self.code_gen_boolean(*rhs);
                self.output_string += &format!("    {}\n", op);
            }
            Boolean::Paren(boolean) => self.code_gen_boolean(*boolean),
        }
    }

    fn code_gen_if(&mut self, condition: Boolean, if_branch: Statement, else_branch: Statement) {
        let else_label = self.next_label();
        let end_label = self.next_label();
        self.code_gen_boolean(condition);
        self.output_string += &format!("    jeqz {else_label}\n");
        self.code_gen(if_branch);
        self.output_string += &format!("    jmp {end_label}\n");
        self.output_string += &format!("{else_label}\n");
        self.code_gen(else_branch);
        self.output_string += &format!("{end_label}\n");
    }

    fn code_gen_while(&mut self, condition: Boolean, body: Statement) {
        let condition_label = self.next_label();
        let end_label = self.next_label();
        self.output_string += &format!("{condition_label}\n");
        self.code_gen_boolean(condition);
        self.output_string += &format!("    jeqz {end_label}\n");
        self.code_gen(body);
        self.output_string += &format!("    jmp {condition_label}\n");
        self.output_string += &format!("{end_label}\n");
    }

    fn code_gen_block(&mut self, declarations: Vec<Declaration>, body: Statement) {
        self.scopes.push(HashMap::new());

        for decl in declarations {
            let Declaration::Variable { ident, value } = decl;
            let unique_name = self.declare_var(&ident);
            self.code_gen_arithmetic(value);
            self.output_string += &format!("    store {unique_name}\n")
        }

        self.code_gen(body);

        self.scopes.pop();
    }

    fn declare_var(&mut self, name: &str) -> String {
        self.var_counter += 1;
        let unique_name = format!("{}_{name}", self.var_counter);
        self.scopes
            .last_mut()
            .unwrap()
            .insert(name.to_string(), unique_name.clone());
        unique_name
    }

    fn resolve_var(&mut self, name: &str) -> String {
        for scope in self.scopes.iter().rev() {
            if let Some(unique_name) = scope.get(name) {
                return unique_name.clone();
            }
        }

        name.to_string()
    }
}
