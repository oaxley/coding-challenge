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
 * @brief	Module to update an URL from the database
 */

//----- imports
use crate::database as database;
use crate::utils as utils;
use crate::url::{get_string, get_tags};


//----- functions

/* update a URL in the database
 *
 * Args:
 *      The parameters for the function extracted from the command line
 *
 * Returns:
 *      true if the update was successful
 */
pub fn update(params: utils::Params) -> bool {

    // retrieve the database name either from arguments or environment
    let db_name: String = utils::get_store_name(params.1);
    if db_name.is_empty() {
        eprintln!("Error: unable to determine the database name!");
        return false;
    }

    // prepare statement
    let mut statement = String::from("");
    let mut comma = "";

    if params.2.is_some() {
        let new_str = format!("url = '{}'", get_string(params.2));
        statement = statement + &new_str;
        comma = ", ";
    }

    if params.3.is_some() {
        statement = statement +
            &format!("{}description = '{}'", comma, get_string(params.3));
    }

    if !params.4.is_empty() {
        statement = statement +
            &format!("{}tags = '{}'", comma, get_tags(params.4));
    }

    if statement.is_empty() {
        // nothing to update
        return false
    } else {
        // finish the statement
        let statement = format!("UPDATE bookmarks SET {} WHERE id=(?1)", statement);
        let id = get_string(params.0);

        // execute the query
        match database::connect(&db_name) {
            Some(db) => {
                match db.execute(&statement, (&id,)) {
                    Ok(_) => return true,
                    Err(_) => return false
                }
            },
            None => return false
        }
    }
}