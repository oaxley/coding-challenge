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
mod cli;


//----- main entry point
fn main() {

    // parse command line arguments
    let matches = cli::command_line().get_matches();

    match matches.subcommand() {
        Some(("store", sub_matches)) => {
            match sub_matches.subcommand() {
                Some(("new", sub_matches)) => {
                    let store_name = sub_matches.get_one::<String>("NAME");
                    println!("Store name = {:?}", store_name);
                }
                Some(("print", sub_matches)) => {
                    let store_name = sub_matches.get_one::<String>("NAME");
                    let regex = sub_matches.get_one::<String>("regex");
                    let format = sub_matches.get_one::<String>("format");

                    println!("Store name = {:?}", store_name);
                    println!("Regex = {:?}", regex);
                    println!("Format = {:?}", format);
                }
                Some(("export", sub_matches)) => {
                    let store_name = sub_matches.get_one::<String>("NAME");
                    let regex = sub_matches.get_one::<String>("regex");
                    let format = sub_matches.get_one::<String>("format");

                    println!("Store name = {:?}", store_name);
                    println!("Regex = {:?}", regex);
                    println!("Format = {:?}", format);
                }
                Some(("test", sub_matches)) => {
                    let store_name = sub_matches.get_one::<String>("NAME");
                    let regex = sub_matches.get_one::<String>("regex");
                    let format = sub_matches.get_one::<String>("format");

                    println!("Store name = {:?}", store_name);
                    println!("Regex = {:?}", regex);
                    println!("Format = {:?}", format);
                }
                _ => cli::command_line().print_help().unwrap()
            }
        }

        Some(("url", sub_matches)) => {
            match sub_matches.subcommand() {
                Some(("new", sub_matches)) => {
                    let url = sub_matches.get_one::<String>("URL");
                    let descr = sub_matches.get_one::<String>("descr");
                    let store = sub_matches.get_one::<String>("store");

                    let tags = sub_matches
                        .get_many::<String>("tags")
                        .into_iter()
                        .flatten()
                        .collect::<Vec<_>>();

                    println!("URL = {:?}", url);
                    println!("Description = {:?}", descr);
                    println!("Tags = {:?}", tags);
                    println!("Store = {:?}", store);
                }
                Some(("update", sub_matches)) => {
                    let id = sub_matches.get_one::<String>("ID");
                    let url = sub_matches.get_one::<String>("url");
                    let descr = sub_matches.get_one::<String>("descr");
                    let store = sub_matches.get_one::<String>("store");

                    let tags = sub_matches
                        .get_many::<String>("tags")
                        .into_iter()
                        .flatten()
                        .collect::<Vec<_>>();

                    println!("ID = {:?}", id);
                    println!("URL = {:?}", url);
                    println!("Description = {:?}", descr);
                    println!("Tags = {:?}", tags);
                    println!("Store = {:?}", store);
                }
                Some(("print", sub_matches)) => {
                    let id = sub_matches.get_one::<String>("ID");
                    let format = sub_matches.get_one::<String>("format");
                    let store = sub_matches.get_one::<String>("store");

                    println!("ID = {:?}", id);
                    println!("Format = {:?}", format);
                    println!("Store = {:?}", store);
                }
                Some(("delete", sub_matches)) => {
                    let id = sub_matches.get_one::<String>("ID");
                    let store = sub_matches.get_one::<String>("store");

                    println!("ID = {:?}", id);
                    println!("Store = {:?}", store);
                }
                Some(("search", sub_matches)) => {
                    let url = sub_matches.get_one::<String>("url");
                    let descr = sub_matches.get_one::<String>("descr");
                    let format = sub_matches.get_one::<String>("format");
                    let store = sub_matches.get_one::<String>("store");

                    let tags = sub_matches
                        .get_many::<String>("tags")
                        .into_iter()
                        .flatten()
                        .collect::<Vec<_>>();

                    println!("URL = {:?}", url);
                    println!("Description = {:?}", descr);
                    println!("Tags = {:?}", tags);
                    println!("Format = {:?}", format);
                    println!("Store = {:?}", store);
                }
                _ => cli::command_line().print_help().unwrap()
            }
        }

        _ => {
            unreachable!("Unsupported subcommand")
        }
    }

}