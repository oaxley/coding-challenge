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
 * @brief	Rock, Paper, Scissors module
 */

//----- imports
use std::io::{self, Write};
use std::process;
use rand::seq::SliceRandom;


//----- globals
static ROCK: &str = "rock";
static PAPER: &str = "paper";
static SCISSORS: &str = "scissors";


//----- class
pub struct RPS {
    selection: String
}

impl RPS {

    // constructor from a user String
    pub fn new(choice: String) -> Option<RPS> {
        if [ROCK, PAPER, SCISSORS].contains(&choice.as_ref()) {
            return Some(RPS { selection: choice });
        }

        // unable to determine user choice
        None
    }

    // select a random choice
    pub fn random() -> RPS {
        let mut rng = rand::thread_rng();
        RPS {
            selection: [ROCK, PAPER, SCISSORS].choose(&mut rng).unwrap().to_string()
        }
    }

    // query the user for his selection
    pub fn query_user(&mut self) -> bool {
        let mut user_move = String::new();

        // prompt
        print!("Rock, Paper or Scissors? ('q' to quit): ");
        io::stdout().flush().unwrap();

        // read user input
        io::stdin().read_line(&mut user_move).unwrap();
        let user_move = user_move.trim().to_lowercase();

        // check user input against the valid moves
        if [ROCK, PAPER, SCISSORS].contains(&user_move.as_ref()) {
            self.selection = user_move;
            true
        } else if user_move == "q" {
            println!("Bye.");
            process::exit(0);
        } else {
            false
        }
    }

}