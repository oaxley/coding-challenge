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
 * @brief	Rock, Paper, Scissors in Rust
 */

//----- imports


mod rps;
use rps::RPS;

//----- functions
fn play() {
    // scores
    let mut usr_score = 0;
    let mut com_score = 0;

    // mainloop
    loop {

        // player move
        let mut usr = RPS::random();
        if !usr.query_user() {
            println!("Invalid choice! Please Try again.");
            continue;
        }

        // get the computer move
        let com = RPS::random();

        // print choices
        println!("You: {} \t Computer: {}", usr, com);

        // check the results
        if usr > com {
            println!("You wins!");
            usr_score += 1;
        } else if usr < com {
            println!("Computer wins!");
            com_score += 1;
        } else {
            println!("Draw game!");
        }

        // print the score
        print!("Scores --- ");
        println!("Computer: {} \t Player: {}", com_score, usr_score);
    }

}


//----- main entry point
fn main() {
    play();
}