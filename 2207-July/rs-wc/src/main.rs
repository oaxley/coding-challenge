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

    print_bytes: bool,
    print_lines: bool,
}

impl FileCounter {
    // constructor
    pub fn new() -> FileCounter {
        FileCounter {
            line_counter: 0,
            byte_counter: 0,
            print_bytes: false,
            print_lines: false
        }
    }

    // set the flags
    pub fn activate_bytes(&mut self) {
        self.print_bytes = true;
    }

    pub fn activate_lines(&mut self) {
        self.print_lines = true;
    }

    // print values depending on the flags
    fn print_values(&self, bytes: usize, lines: usize) {
        if self.print_lines {
            print!("{:>4} ", lines);
        }

        if self.print_bytes {
            print!("{:>4} ", bytes);
        }
    }

    pub fn print_total(self) {
        self.print_values(self.byte_counter, self.line_counter);
        println!("total");
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

        // print intermediate values
        self.print_values(bytes, lines);
        println!("");

        // increment global counters
        self.line_counter += lines;
        self.byte_counter += bytes;
    }

    pub fn process_file(&mut self, filename: &str) {
        let mut lines: usize = 0;
        let mut bytes: usize = 0;
        let mut buffer = String::new();

        if !Path::new(filename).exists() {
            eprintln!("{filename}: No such file or directory");
            process::exit(1);
        }

        let file = File::open(filename).unwrap();
        let mut reader = BufReader::new(file);

        loop {
            let num_bytes = reader.read_line(&mut buffer).unwrap();

            if num_bytes == 0 {
                break;
            }

            bytes += num_bytes;
            lines += 1;
        }

        // print intermediate values
        self.print_values(bytes, lines);
        println!("{filename}");

        // increment global counters
        self.line_counter += lines;
        self.byte_counter += bytes;

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

    // process flags
    if app.contains_id("lines") {
        obj.activate_lines();
    }

    if app.contains_id("chars") {
        obj.activate_bytes();
    }

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

