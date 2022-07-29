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
use std::fmt;
use rusqlite::{self, Connection};
use serde::{Serialize};


//----- structures

// bookmark structure for Read/Search operations
#[derive(Debug, Serialize)]
pub struct Bookmark {
    pub id: i32,
    pub url: String,
    pub description: String,
    pub tags: String
}

impl fmt::Display for Bookmark {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "ID\t\t: {}\nURL\t\t: {}\nDescription\t: {}\nTags\t\t: {}",
            self.id, self.url, self.description, self.tags)
    }
}


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
