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
/* print a success message and exit with code 0
 *
 * Args:
 *      message: the message to display before leaving
 *
 * Returns:
 *      This function never returns
 */
fn success(message: &str) {
    println!("{}", message);
    process::exit(0);
}

/* print a failure message and exit with code 1
 *
 * Args:
 *      message: the messa to display before leaving
 *
 * Returns:
 *      This function never returns
 */
fn failure(message: &str) {
    println!("{}", message);
    process::exit(1);
}


//----- main entry point
fn main() {

    // parse command line arguments
    let matches = cli::command_line().get_matches();

    match matches.subcommand() {
        Some(("store", sub_matches)) => {
            match sub_matches.subcommand() {
                Some(("new", sub_matches)) => {

                    let params = utils::retrieve_params(sub_matches);
                    match store::create(&params) {
                        true =>success("The new database has been created"),
                        false => failure("Error: unable to create the database.")
                    }
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
                    match url::create(params) {
                        true => success("URL has been created."),
                        false => failure("Error: unable to create the URL.")
                    }
                }
                Some(("update", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    match url::update(params) {
                        true => success("URL has been updated."),
                        false => failure("Error: unable to update the URL.")
                    }
                }
                Some(("print", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    match url::read(params) {
                        true => success(""),
                        false => failure("")
                    }
                }
                Some(("delete", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    match url::delete(params) {
                        true => success("URL has been deleted."),
                        false => failure("Error: unable to delete the URL.")
                    }
                }
                Some(("search", sub_matches)) => {
                    let params = utils::retrieve_params(sub_matches);
                    // match url::search(params) {
                    //     true => success(""),
                    //     false => failure("Error: unable to find the URL.")
                    // }
                }
                _ => cli::command_line().print_help().unwrap()
            }
        }

        _ => {
            unreachable!("Unsupported subcommand")
        }
    }

}