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
fn read_file(filename: &str, numbers: bool) -> io::Result<()> {
    let file = File::open(filename)?;
    let reader = BufReader::new(file);

    let mut counter = 1u32;

    for line in reader.lines() {
        if numbers {
            print!("{:>5}\t", counter);
        }
        println!("{}", line?);
        counter += 1;
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

    // standard input
    if filename == "-" {
        read_file("-", numbers).unwrap();
    } else {
        // ensure the file exists
        if Path::new(filename).exists() {
            read_file(filename, numbers).unwrap();
        } else {
            eprintln!("Error: {} does not exist.", filename);
            process::exit(1);
        }
    }

}
