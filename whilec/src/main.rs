use std::io::Write;

use clap::Parser;

mod code_gen;
mod lexer;
mod parser;

#[derive(Parser, Debug)]
struct Args {
    input: String,

    #[arg(short, long, default_value_t = String::from("out.bin"))]
    output: String,
}

fn main() {
    let args = Args::parse();
    let tokens = lexer::lex(args.input);
    let mut parser = parser::Parser { tokens, index: 0 };
    let ast = parser.parse_statement();
    let mut code_generator = code_gen::CodeGenerator {
        output_string: String::from("0:\n"),
        label: 1,
    };
    code_generator.code_gen(ast);
    let mut file = std::fs::File::create("out.asm").unwrap();
    file.write_all(code_generator.output_string.as_bytes())
        .unwrap();
}
