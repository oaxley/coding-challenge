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
 * @brief	'wc' command in Rust
 */

//----- imports
use std::process;
use std::fs::File;
use std::path::Path;
use std::io::{self, prelude::*, BufReader};

use clap::{Arg, crate_description, Command};


//----- class
pub struct FileCounter {
    line_counter: usize,
    byte_counter: usize,
}

impl FileCounter {
    // constructor
    pub fn new() -> FileCounter {
        FileCounter {
            line_counter: 0,
            byte_counter: 0,
        }
    }

    // process stdin
    pub fn process_stdin(&mut self) {
        let mut stdin = io::stdin().lock();

        let mut lines: usize = 0;
        let mut bytes: usize = 0;

        let mut buffer = String::new();
        loop {
            let num_bytes = stdin.read_line(&mut buffer).unwrap();

            if num_bytes == 0 {
                break;
            }

            bytes += num_bytes;
            lines += 1;
        }

        println!("{:>4} {:>4}", lines, bytes);
        self.line_counter += lines;
        self.byte_counter += bytes;
    }

    pub fn process_file(&mut self, filename: &str) {
        todo!()
    }

    pub fn print_total(self) {
        println!("{:>4} {:>4} total", self.line_counter, self.byte_counter);
    }
}


//----- main entry point
fn main() {

    // read the command line arguments
    let app = Command::new("rs-wc")
                .about(crate_description!())

                .arg(Arg::new("FILE")
                    .multiple_values(true)
                    .help("With no FILE, or when FILE is -, read standard input."))

                .arg(Arg::new("lines")
                    .short('l')
                    .long("lines")
                    .help("Count newlines"))

                .arg(Arg::new("chars")
                    .short('c')
                    .long("chars")
                    .help("Count chars"))

                .get_matches();

    // create the FileCounter instance
    let mut obj = FileCounter::new();

    // process the input
    let values = app.get_many::<String>("FILE");
    match values {
        None => obj.process_stdin(),
        Some(files) => {
            for file in files {
                if file == "-" {
                    obj.process_stdin();
                } else {
                    obj.process_file(file);
                }
            }

            // print totals
            obj.print_total();
        }
    }
}

