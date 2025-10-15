use clap::Parser;

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
    let ast = parser.parse();
    println!("{ast:#?}");
}
