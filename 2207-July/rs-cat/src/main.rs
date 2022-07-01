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
 * @brief	'cat' command in Rust
 */

//----- imports
use std::process;
use std::fs::File;
use std::path::Path;
use std::io::{self, prelude::*, BufReader};

use clap::{Arg, App, crate_description};


//----- functions
fn process_file(filename: &str, numbers: bool, mut counter: u32) -> u32 {
    if !Path::new(filename).exists() {
        eprintln!("{}: No such file or directory", filename);
        process::exit(1);
    }

    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);

    for line in reader.lines() {
        if numbers {
            print!("{:>6}\t", counter);
        }

        println!("{}", line.unwrap());
        counter += 1;
    }

    counter
}

fn process_stdin(numbers: bool, mut counter: u32) -> u32 {
    let stdin = io::stdin();
    let reader = stdin.lock();

    for line in reader.lines() {
        if numbers {
            print!("{:>6}\t", counter);
        }

        println!("{}", line.unwrap());
        counter += 1;
    }

    counter
}

fn process_entry(filename: &str, numbers: bool) -> io::Result<()> {
    // line counter
    let mut counter: u32 = 1;

    if filename == "-" {
        counter = process_stdin(numbers, counter);
    } else {
        counter = process_file(filename, numbers, counter);
    }

    Ok(())
}

//----- main entry point
fn main() {
    // read the command line arguments
    let app = App::new("rs-cat")
                    //.version(crate_version!())
                    //.author(crate_authors!())
                    .about(crate_description!())

                    .arg(Arg::with_name("FILE")
                        .help("With no FILE, or when FILE is -, read standard input."))

                    .arg(Arg::with_name("number")
                        .short('n')
                        .long("--number")
                        .help("number all output lines"))

                    .get_matches();

    // retrieve the arguments
    let filename = app.value_of("FILE").unwrap_or("-");
    let numbers: bool = app.contains_id("number");

    process_entry(filename, numbers).unwrap();

}
