/* -*- coding: utf-8 -*-
 * vim: set ft=rust
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Magic-8 ball in Rust
 */

//----- imports
use std::io::{self, Write};
use std::{thread, time};
use rand::seq::SliceRandom;


//----- globals

// available responses
static RESPONSES: [&str; 20] = [
    "It is certain.",
    "It is decidedly so.",
    "Without a doubt.",
    "Yes - definitely.",
    "You may rely on it.",
    "As I see it, yes.",
    "Most likely.",
    "Outlook good.",
    "Yes.",
    "Signs point to yes.",
    "Reply hazy, try again.",
    "Ask again later.",
    "Better not tell you now.",
    "Cannot predict now.",
    "Concentrate and ask again.",
    "Don't count on it.",
    "My reply is no.",
    "My sources say no.",
    "Outlook not so good.",
    "Very doubtful.",
];

// thinking time
static THINK_TIME: u64 = 1000;


//----- functions
fn shake_the_ball() {
    println!("Magic 8 ball Oracle");

    // mainloop
    loop {
        // get the input from the user
        let question = query_user();

        if question == "q" {
            break;
        } else if question.is_empty() {
            println!("No question, no answer!");
            continue;
        }

        // thinking time (output is buffered so we need to flush it everytime)
        print!("Thinking ");
        io::stdout().flush().unwrap();

        for _ in 0..3 {
            print!(".");
            io::stdout().flush().unwrap();
            thread::sleep(time::Duration::from_millis(THINK_TIME));
        }
        println!("");

        // get a random answer
        let mut rng = rand::thread_rng();
        let response = RESPONSES.choose(&mut rng).unwrap();

        // print it
        println!("Magic-8 says: {response}");
        println!();
    }

    println!("Bye.");
}


// ask the user for his question
fn query_user() -> String {
    let mut question = String::new();

    println!("Please enter your question (q to quit):");
    io::stdin().read_line(&mut question).unwrap();

    // remove extra spaces and set everything to lowercase
    question.trim().to_lowercase()
}


//----- main entry point
fn main() {
    shake_the_ball();
}