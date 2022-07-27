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
 * @brief	Main URL module
 */

//----- imports
use crate::database as database;
use crate::utils as utils;

//----- globals
// add a new url
const SQLITE_INSERT_URL: &str = "
    INSERT INTO bookmarks
    (url, description, tags)
    VALUES (?1, ?2, ?3)";

// delete a URL
const SQLITE_DELETE_URL: &str = "
    DELETE FROM bookmarks
    WHERE id=(?1)";


//----- functions

/* retrieve a string from an Option<&String>
 *
 * Args:
 *      option: the Option<&String> to get the value from
 *
 * Returns:
 *      The String if it's defined in the option, otherwise ""
 */
fn get_string(option: Option<&String>) -> String {
    if let Some(value) = option {
        return value.to_string();
    } else {
        return String::from("");
    }
}

/* create a string from a vector of tags
 *
 * Args:
 *      tags: a vector of String
 *
 * Returns:
 *      A string with all the element of the vector concatenate with ','.
 *      Returns an empty string if no tags are defined.
 */
fn get_tags(tags: Vec<&String>) -> String {
    let mut value = String::from("");
    for tag in tags {
        if value.is_empty() {
            value = tag.to_string();
        } else {
            value = value + ",";
            value = value + &tag.to_string();
        }
    }

    value
}

/* create a new URL in the database
 *
 * Args:
 *      The parameters for the function extracted from the command line
 *
 * Returns:
 *      true if the creation was successful
 */
pub fn create(params: utils::Params) -> bool {

    // retrieve the database name either from arguments or environment
    let db_name: String = utils::get_store_name(params.1);
    if db_name.is_empty() {
        eprintln!("Error: unable to determine the database name!");
        return false;
    }

    // retrive the url, description & tags
    let url = get_string(params.2);
    let description = get_string(params.3);
    let tags = get_tags(params.4);

    // create the connection to the DB
    match database::connect(&db_name) {
        Some(db) => {
            match db.execute(SQLITE_INSERT_URL, (&url, &description, &tags)) {
                Ok(_) => return true,
                Err(_) => return false
            }
        },
        None => return false
    }
}

// update an URL
pub fn update(id: Option<&String>, url: Option<&String>, descr: Option<&String>, tag_vector: Vec<&String>, store: Option<&String>) -> bool {
    false
}

// delete an URL
pub fn delete(id: Option<&String>, store: Option<&String>) -> bool {

    // retrieve the database name either from arguments or environment
    let db_name: String = utils::get_store_name(store);
    if db_name.is_empty() {
        eprintln!("Error: unable to determine the database name!");
        return false;
    }

    // retrieve the id
    let id = get_string(id);

    // create the connection to the DB
    match database::connect(&db_name) {
        Some(db) => {
            match db.execute(SQLITE_DELETE_URL, (&id,)) {
                Ok(_) => return true,
                Err(_) => return false
            }
        },
        None => return false
    }
}