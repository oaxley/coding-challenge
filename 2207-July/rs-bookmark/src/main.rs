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
 * @brief	Bookmarks manager in Rust
 */

//----- imports
use std::process;

//----- modules declaration
mod cli;
mod database;
mod store;
mod url;
mod utils;


//----- functions


//----- main entry point
fn main() {

    // parse command line arguments
    let matches = cli::command_line().get_matches();

    match matches.subcommand() {
        Some(("store", sub_matches)) => {
            match sub_matches.subcommand() {
                Some(("new", sub_matches)) => {

                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);

                    // if let Some(store) = sub_matches.get_one::<String>("NAME") {
                    //     if store::create(store) {
                    //         println!("Database [{}] has been created successfully.", store);
                    //         process::exit(0);
                    //     } else {
                    //         eprintln!("Error: unable to create database [{}]!", store);
                    //         process::exit(1);
                    //     }
                    // }
                }

                Some(("export", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);
                }

                Some(("test", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);
                }

                _ => cli::command_line().print_help().unwrap()
            }
        }

        Some(("url", sub_matches)) => {
            match sub_matches.subcommand() {
                Some(("new", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);

                    // retrieve the parameters
                    // let values = retrieve_params(sub_matches);

                    // create the URL
                    // if url::create(values.0, values.1, values.2, values.3) {
                    //     println!("URL has been created.");
                    //     process::exit(0);
                    // } else {
                    //     eprintln!("Error: unable to create the URL.");
                    //     process::exit(1);
                    // }
                }
                Some(("update", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);
                }
                Some(("print", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);
                }
                Some(("delete", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);


                    // let id = sub_matches.get_one::<String>("ID");
                    // let store = sub_matches.get_one::<String>("store");

                    // if url::delete(id, store) {
                    //     println!("URL has been deleted.");
                    //     process::exit(0);
                    // } else {
                    //     println!("Error: unable to delete the URL");
                    //     process::exit(1);
                    // }
                }
                Some(("search", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    println!("{}", params);
                }
                _ => cli::command_line().print_help().unwrap()
            }
        }

        _ => {
            unreachable!("Unsupported subcommand")
        }
    }

}