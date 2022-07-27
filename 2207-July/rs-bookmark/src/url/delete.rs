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
 * @brief	Module to delete an URL from the database
 */

//----- imports
use crate::database as database;
use crate::utils as utils;
use crate::url::{get_string};


//----- constants

const SQLITE_DELETE_URL: &str = "
    DELETE FROM bookmarks
    WHERE id=(?1)";


//----- functions

/* delete a URL from the database
 *
 * Args:
 *      The parameters for the function extracted from the command line
 *
 * Returns:
 *      true if the deletion was successful
 */
pub fn delete(params: utils::Params) -> bool {

    // retrieve the database name either from arguments or environment
    let db_name: String = utils::get_store_name(params.1);
    if db_name.is_empty() {
        eprintln!("Error: unable to determine the database name!");
        return false;
    }

    // retrieve the id
    let id = get_string(params.0);

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
