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
 * @brief	Module to handle sqlite communication
 */

//----- imports
use std::path::Path;
use rusqlite::{self, Connection};


//----- functions

// create a connection to an existing database
pub fn connect(store_name: &String) -> Option<Connection> {
    // check if the database exists
    if !Path::new(store_name).exists() {
        return None
    }

    match Connection::open(store_name) {
        Ok(cnx) => return Some(cnx),
        Err(_) => return None
    }
}
