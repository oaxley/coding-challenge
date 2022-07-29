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
 * @brief	Search an URL in the database
 */

//----- imports
use regex::Regex;

use crate::database as database;
use crate::utils as utils;
use crate::url::{get_string, get_tags};


//----- functions

/* match an URL against a regular expression
 *
 * Args:
 *      pattern: the regular expression to match the URL against
 *      url: the URL to test
 *
 * Returns:
 *      true if the URL is validated by the regexp
 */
fn match_url(pattern: &str, url: &str) -> bool {
    let re_url = Regex::new(pattern).unwrap();
    re_url.is_match(url)
}

/* match a Description against a regular expression
 *
 * Args:
 *      pattern: the regular expression to match the Description against
 *      descr: the description to test
 *
 * Returns:
 *      true if the description is validated by the regexp
 */
fn match_descr(pattern: &str, descr: &str) -> bool {
    let re_descr = Regex::new(pattern).unwrap();
    re_descr.is_match(descr)
}

/* match Tags against a list of tags
 *
 * Args:
 *      tags: the list of tags from the User
 *      bookmark: the tags from the bookmark
 *
 * Returns:
 *      true if all the tags are present
 */
fn match_tags(tags: &str, bookmark: &str) -> bool {
    let v_tags: Vec<&str> = bookmark.split(",").collect();

    let mut is_present = true;
    for tag in tags.split(',') {
        if !v_tags.contains(&tag) {
            is_present = false;
            break;
        }
    }

    is_present
}


/* search an URL in the database
 *
 * Args:
 *      The parameters for the function extracted from the command line
 *
 * Returns:
 *      true if the search was successful
 */
pub fn search(params: utils::Params) -> bool {

    if utils::is_params_empty(&params) {
        eprintln!("Error: no criteria specified for the search.");
        return false
    }

    // retrieve the parameters
    let url = get_string(params.2);
    let descr = get_string(params.3);
    let tags = get_tags(params.4);

    // retrieve the database name either from arguments or environment
    let db_name: String = utils::get_store_name(params.1);
    if db_name.is_empty() {
        eprintln!("Error: unable to determine the database name!");
        return false;
    }

    match database::connect(&db_name) {
        Some(db) => {

            // retrieve all the records
            let query = "SELECT * FROM bookmarks";
            match db.prepare(&query) {

                Ok(mut stmt) => {

                    // retrieve a list of records
                    match stmt.query_map([], |row| {

                        // create a new bookmark object for each rows
                        Ok(
                            database::Bookmark {
                                id: row.get(0).unwrap(),
                                url: row.get(1).unwrap(),
                                description: row.get(2).unwrap(),
                                tags: row.get(3).unwrap()
                            }
                        )})

                    {
                        // process the results through an iterator
                        Ok(rows) => {
                            for row in rows {

                                // bookmark object
                                let bookmark = row.unwrap();
                                let mut do_not_print_me = true;

                                // match the URL
                                if !url.is_empty() {
                                    do_not_print_me = !match_url(&url, &bookmark.url);
                                }

                                // match the description
                                if !descr.is_empty() && !bookmark.description.is_empty() {
                                    do_not_print_me = !match_descr(&descr, &bookmark.description);
                                }

                                // match all the tags
                                if !tags.is_empty() && !bookmark.tags.is_empty() {
                                    do_not_print_me = !match_tags(&tags, &bookmark.tags);
                                }

                                // check if this bookmark should be printed
                                if do_not_print_me {
                                    continue;
                                } else {
                                    println!("{}", bookmark);
                                }
                            }

                            return true
                        },

                        Err(_) => {
                            eprintln!("Error: unable to retrieve rows with the selected criterias");
                            return false
                        }
                    }
                },

                Err(err) => {
                    eprintln!("Error: unable to prepare the SQL statement: {}", err);
                    return false
                }
            }
        },

        None => {
            eprintln!("Error: unable to get a connection to database");
            return false
        }
    }
}