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
 * @brief	Helper functions
 */

//----- imports
use std::env;


//----- functions

// retrieve the store name
pub fn get_store_name(store: Option<&String>) -> String {
    if let Some(value) = store {
        return value.to_string();
    } else {
        match getenv_store() {
            Some(value) => return value,
            None => return String::from("")
        }
    }
}

// retrieve the database name from the environment variable
pub fn getenv_store() -> Option<String> {
    match env::var("DATABASE") {
        Ok(value) => return Some(value),
        Err(_) => return None
    }
}