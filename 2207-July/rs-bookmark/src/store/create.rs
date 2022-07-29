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
 * @brief	Module to create a new Database
 */

//----- imports
use std::path::Path;
use rusqlite::{self, Connection};

use crate::utils as utils;


//----- constants

const SQLITE_TABLE_CREATION: &str = "
    CREATE TABLE bookmarks (
        id INTEGER PRIMARY KEY,
        url TEXT NOT NULL UNIQUE,
        description TEXT,
        tags TEXT
    )";


//----- functions

// create a new SQLite database
pub fn create(params: &utils::Params) -> bool {

    let store_name = utils::get_store_name(params.1);

    // check if the database exist already
    if Path::new(&store_name).exists() {
        return true;
    }

    // create a new SQLite database
    match Connection::open(store_name) {
        Ok(cnx) => {
            // create the table(s)
            match cnx.execute(SQLITE_TABLE_CREATION, ()) {
                Ok(_) => return true,
                Err(_) => return false
            }
        },
        Err(_) => return false
    }
}
