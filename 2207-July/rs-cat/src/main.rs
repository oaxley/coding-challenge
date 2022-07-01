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

use clap::{Arg, crate_description, Command};


//----- class
pub struct FileProcessor {
    counter: u32,
    numbers: bool
}

impl FileProcessor {
    // constructor
    pub fn new(numbers: bool) -> FileProcessor {
        FileProcessor {
            counter: 1,
            numbers:  numbers
        }
    }

    // print the line with the counter and increment it
    fn print_line(&mut self, line: String) {
        if self.numbers {
            print!("{:>6}\t", self.counter);
        }
        println!("{line}");
        self.counter += 1;
    }

    // process a file
    pub fn process_file(&mut self, filename: &str) {
        if !Path::new(filename).exists() {
            eprintln!("{filename}: No such file or directory");
            process::exit(1);
        }

        let file = File::open(filename).unwrap();
        let reader = BufReader::new(file);

        for line in reader.lines() {
            self.print_line(line.unwrap());
        }
    }

    // process stdin
    pub fn process_stdin(&mut self) {
        let stdin = io::stdin();
        let stdin = stdin.lock();

        for line in stdin.lines() {
            self.print_line(line.unwrap());
        }
    }

}


//----- main entry point
fn main() {

    // read the command line arguments
    let app = Command::new("rs-cat")
                    .about(crate_description!())

                    .arg(Arg::with_name("FILE")
                        .multiple_values(true)
                        .help("With no FILE, or when FILE is -, read standard input."))

                    .arg(Arg::with_name("number")
                        .short('n')
                        .long("--number")
                        .help("number all output lines"))

                    .get_matches();

    // create the FileProcessor instance
    let mut obj = FileProcessor::new(app.contains_id("number"));

    // process input(s)
    let values = app.get_many::<String>("FILE");
    match values {
        None => obj.process_stdin(),
        Some(files) => {
                for file in files {
                    if file == "-" {
                        obj.process_stdin()
                    } else {
                        obj.process_file(file)
                    }
                }

            }
    }

}
