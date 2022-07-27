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
 * @brief	Module to create a new URL in the database
 */

//----- imports
use crate::database as database;
use crate::utils as utils;
use crate::url::{get_string, get_tags};


//----- constants

const SQLITE_INSERT_URL: &str = "
    INSERT INTO bookmarks
    (url, description, tags)
    VALUES (?1, ?2, ?3)";


//----- functions

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
